#include "ggw.h"
#include "ggwlog.h"
#include "version.h"
#include "ggwplugin.h"
#include "ggwsched.h"
#include "ggwcom.h"
//#include "ggwhttp.h"
// #include "ggwopts.h"

#define PROTO_ID				0x47534370	// Идентификатор протокола ('GSCp')
#define DATA_PORT				34000		// Порт данных (34000)
#define CC_PT_PING_REQUEST		0x16	// 
#define CC_PT_PING_REPLY		0x17	// 

// Global ping request
typedef struct CC_PK_GB_PING_REQ{
	u_long	m_lProto;				// Тип протокола = PROTO_ID
	u_short	m_uType;				// Тип пакета = CC_PT_PING_REQUEST
} *LPCC_PK_GB_PING_REQ;

// Global ping reply
typedef struct CC_PK_GB_PING_REP{
	u_long	m_lProto;				// Тип протокола = PROTO_ID
	u_short	m_uType;				// Тип пакета = CC_PT_PING_REPLY
	in_addr m_iaExAddr;				//
	u_short m_usExPort;				//
} *LPCC_PK_GB_PING_REP;

// Signal flags
bool sigterm_received=false;
bool sighup_received=false;
bool segerr_received=false;

// Global per process request information
u_char	glb_gvers=0;
u_char	glb_glang=0;

// Semaphores for preventing simultaneous requests to DB
int	sta_semid=-1;

struct sembuf sta_lock[2]={
    {0,0,0},
    {0,1,0}
};

struct sembuf sta_unlock[1]={
    {0,-1,0}
};

// Signals

int last_signal=-1;

char	*ppszSignals[]={
	"SIGNONE",				// 0
	"SIGHUP",				// 1
	"SIGINT",				// 2
	"SIGQUIT",				// 3
	"SIGILL",				// 4
	"SIGTRAP",				// 5
	"SIGABRT",				// 6
	"SIGBUS",				// 7
	"SIGFPE",				// 8
	"SIGKILL",				// 9
	"SIGBUS",				// 10
	"SIGSEGV"				// 11
	};

// SIGTERM (15) handler / STOP
void sigterm_handler(int)
{
    sigterm_received=true;    
	fl_syslog(LOG_INFO,"SIGTERM (15) signal received. Terminating");
}

// SIGHUP (1) handler / RESTART
void sighup_handler(int)
{
    sighup_received=true;
    fl_syslog(LOG_DEBUG,"SIGHUP (1) signal received. Restarting");
}

// SIGILL (4)				->Illegal Instruction
// SIGFPE (8)				->Floating point exception
// SIGSEGV (11)				->Invalid memory reference
// SIGBUS (7,10)			->Bus error (bad memory access)
// handler / error flag
void sigerr_handler(int signum)
{
	if(segerr_received)
	    fl_syslog(LOG_WARNING,
			"sigerr_handler() : previous signal %s (%i) not processed!",
			ppszSignals[last_signal],last_signal);
	else
		segerr_received=true;
	last_signal=signum;
    fl_syslog(	LOG_DEBUG,
				"sigerr_handler() : %s (%i) signal received. Processing",
				ppszSignals[signum],
				signum);
	exit(0);
}


int main(int argc, char **argv)
{
    // Local variables

    MYSQL mysql;
	
	pid_t	sched_pid=-1/*,
			httpd_pid=-1*/;

	list<pid_t> child_list;
	
    sockaddr_in loc_addr;
    sockaddr_in rmt_addr;

    sockaddr_in			pg_addr;
	socklen_t			pg_len;
	CC_PK_GB_PING_REQ	pg_req;
	CC_PK_GB_PING_REP	pg_rep;

    int bytes_send=0;
    int ls_socket=-1;
    int cl_socket=-1;

    int pg_socket=-1;

	float ratio;
	
    ggwt_command pl_command;
    socklen_t cl_addrlen;
    time_t	start_time,
			end_time,
			start_recv_wait,
			end_recv_wait;
    char	cmd_buf[RECV_BUFFER_SIZE],
			rcv_buf[RECV_BUFFER_SIZE],
			out_buf[RECV_BUFFER_SIZE],
			unc_buf[RECV_BUFFER_SIZE],
			pck_buf[RECV_BUFFER_SIZE];
	u_long	rcv_tail;
    char rcv_command[255];
    int bytes_recv=0;
	int unc_status;
//    int strange_size_pkts=0,strange_type_pkts=0;
    bool session_active=true;
    int sruaval=1;
    int child_count=0;
    pid_t child_exit_pid,run_pid;
    int inet_res=-1;
    char szWelcome[255];
    char szHostName[255];
    gethostname(szHostName,255);
    DWORD out_size;
    WORD out_commands;
	DWORD dwUncSize,dwCmpSize;
//	SERVER_CONFIG ServerConfig;

//	ggwProcessOptionsAndConfig(argc, argv, ServerConfig);
    
    sprintf(szWelcome,	"GSC Virtual World Daemon Version %i.%i.%i [%s]\n",
		MAJOR_VERSION,MINOR_VERSION,BUILD_VERSION,szHostName);
	
    for(int ic=1; ic<argc; ic++)
		if(strcmp(argv[ic],"-V")==0){
			printf("%s",szWelcome);
			printf("Config file	: %s\n",CFGFILE);
			printf("Log file	: %s\n",LOGFILE);
			printf("Pid file	: %s\n",PIDFILE);
			printf("Var dir		: %s\n",VARDIR);	    
			return 0;
		}else
	if(strcmp(argv[ic],"-D")==0){}
		else
	if(strcmp(argv[ic],"-H")==0){};
	// Starting daemon
	int fd;
	struct rlimit flim;
	if(getppid()!=1){
		// Ignoring TTY's signals
		signal(SIGTTOU,SIG_IGN);
		signal(SIGTTIN,SIG_IGN);
		signal(SIGTSTP,SIG_IGN);
		// New process, group and session
		if(fork()!=0)
			exit(0);
		// Group leader and no terminal	    
		setsid();
	};
	// Closing all files
	getrlimit(RLIMIT_NOFILE,&flim);
	for(fd=0; (u_int)fd<flim.rlim_max; fd++)
		close(fd);
	// Determining startup location
	// We'll need it for plugin lookup
	char szPluginDir[255];
	strcpy(szPluginDir,argv[0]);
	*(strrchr(szPluginDir,'/')+1)='\0';
	strcat(szPluginDir,"plugins");
	// Saving PID file
	int pid_handle=open(	PIDFILE, O_WRONLY | O_CREAT,
							S_IRWXU | S_IRWXG | S_IRWXO);
	char pid_value[255];
	sprintf(pid_value,"%i\n",getpid());
	write(pid_handle,pid_value,strlen(pid_value));
	close(pid_handle);
	// Creating locking semaphore for synch operations
	sta_semid=semget(	IPC_PRIVATE,
						1,
						S_IRWXU | IPC_CREAT);
	// Opening local file log    
	fl_openlog(DNAME,LOG_PID,LOG_DAEMON);
	fl_syslog(LOG_INFO,"%s",szWelcome);
	fl_syslog(LOG_INFO,"   Config file     : %s",CFGFILE);
	fl_syslog(LOG_INFO,"   Log file        : %s",LOGFILE);
	fl_syslog(LOG_INFO,"   Pid file        : %s",PIDFILE);
	fl_syslog(LOG_INFO,"   Var dir         : %s",VARDIR);    
	// Plugin lookup
	fl_syslog(LOG_INFO," Plugins :");
	load_plugins();
	// Changing current directory to root
	chdir("/");
	// Parsing configuration file
	u_int16_t	local_port=LISTEN_PORT;
	int		backlog=LISTEN_BACKLOG;
	// Checking semaphore creating result
	if(sta_semid==-1){
		fl_syslog(LOG_CRIT,
			"Create semaphore failed! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Using default configuration
	// Creating socket
	ls_socket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(ls_socket==-1){
		fl_syslog(LOG_CRIT,
			"Unable to create socket! [%s]",strerror(errno));
		goto parent_process_exit;	
	};
	// Setting SO_REUSEADDR for socket
	inet_res=setsockopt(ls_socket,SOL_SOCKET,SO_REUSEADDR,&sruaval,sizeof(int));
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to set socket parameters! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Preparing address for binding
	memset(&loc_addr,0,sizeof(sockaddr_in));
	loc_addr.sin_family=AF_INET;
	loc_addr.sin_port=htons(local_port);
	loc_addr.sin_addr.s_addr=INADDR_ANY;
	// Binding to all local interfaces
	inet_res=bind(ls_socket,(sockaddr*)&loc_addr,sizeof(sockaddr_in));
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to bind socket! [%s]",strerror(errno));
			goto parent_process_exit;
	};

	// Creating ping mirrow socket
	pg_socket=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(pg_socket==-1){
		fl_syslog(LOG_CRIT,
			"Unable to create ping socket! [%s]",strerror(errno));
		goto parent_process_exit;	
	};
	// Setting SO_REUSEADDR for socket
	inet_res=setsockopt(pg_socket,SOL_SOCKET,SO_REUSEADDR,&sruaval,sizeof(int));
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to set ping socket parameters! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Preparing address for binding
	memset(&loc_addr,0,sizeof(sockaddr_in));
	loc_addr.sin_family=AF_INET;
	loc_addr.sin_port=htons(DATA_PORT);
	loc_addr.sin_addr.s_addr=INADDR_ANY;
	// Binding to all local interfaces
	inet_res=bind(pg_socket,(sockaddr*)&loc_addr,sizeof(sockaddr_in));
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to bind ping socket! [%s]",strerror(errno));
			goto parent_process_exit;
	};

	// Creating lisening queue
	inet_res=listen(ls_socket,backlog);
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to listen socket! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Setting socket to non-blocking mode
	inet_res=fcntl(ls_socket,F_SETFL,O_NONBLOCK);    
	if(inet_res==-1){
		fl_syslog(LOG_CRIT,
			"Unable to switch socket to non-blocking mode! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Setting SIGTERM & SIGHUP signal handlers
	signal(SIGTERM,sigterm_handler);
	signal(SIGHUP,sighup_handler);
	// Setting error handlers
	signal(SIGILL,sigerr_handler);
	signal(SIGFPE,sigerr_handler);
	signal(SIGSEGV,sigerr_handler);
	signal(SIGBUS,sigerr_handler);
	// Starting sched process
	if((sched_pid=start_scheds())==-1){
		fl_syslog(LOG_CRIT,
			"Unable to fork() into sched! [%s]",strerror(errno));
		goto parent_process_exit;
	};
	// Starting http process
//	if((httpd_pid=start_httpd())==-1){
//		fl_syslog(LOG_CRIT,
//			"Unable to fork() into httpd! [%s]",strerror(errno));
//		goto parent_process_exit;
//	};
	// Waiting for clients until SIGTERM received
	while(!sigterm_received){
		// Kill any zomby child
		child_exit_pid=waitpid(-1,NULL,WNOHANG);
		if(child_exit_pid>0)
			if((child_exit_pid==sched_pid)/*||(child_exit_pid==httpd_pid)*/){
				// We have a problem with sched process
			}else{
				fl_syslog(LOG_DEBUG,
					"Zomby child [%i] killed successfully. Now [%i] child(s) left",child_exit_pid,--child_count);
				child_list.erase(	find(	child_list.begin(),
											child_list.end(),
											child_exit_pid));
			};
		// Check pinger socket
		pg_len=sizeof(sockaddr_in);
		inet_res=recvfrom(pg_socket,(char*)&pg_req,sizeof(CC_PK_GB_PING_REQ),MSG_DONTWAIT,&pg_addr,&pg_len);
		if(inet_res!=-1){
			// Reply
			pg_rep.m_lProto=PROTO_ID;
			pg_rep.m_uType=CC_PT_PING_REPLY;
			pg_rep.m_iaExAddr=;
			pg_rep.m_usExPort=;

            sendto(pg_socket,(char*)&pg_rep,sizeof(CC_PK_GB_PING_REP),MSG_DONTWAIT,&pg_addr,sizeof(sockaddr_in));
		};

		// Check for restart flag
		if(sighup_received){
			// Rotate logs
			// fl_rotate();
			// We have to send the same signal to child sched process
			if(sched_pid!=-1)
				if(kill(sched_pid,SIGHUP)==-1)
					fl_syslog(LOG_CRIT,
						"Unable to send kill() to sched process! [%s]",strerror(errno));
			// We have to send the same signal to httpd sched process
			//if(httpd_pid!=-1)
			//	if(kill(httpd_pid,SIGHUP)==-1)
			//		fl_syslog(LOG_CRIT,
			//			"Unable to send kill() to httpd process! [%s]",strerror(errno));
			// We have to reload plugins right now
			fl_syslog(LOG_INFO,"Parent - Reloading plugins :");
			// Clear sighup_recieved flag
			sighup_received=false;
			// Unloading plugin modules
			free_plugins();
			// Reloading plugin modules
			load_plugins();
		}; // (sighup_received)
		// Waiting for incoming connection
		cl_addrlen=sizeof(sockaddr_in);
		memset(&rmt_addr,0x00,sizeof(sockaddr_in));
		cl_socket=accept(ls_socket,(sockaddr*)&rmt_addr,&cl_addrlen);
		if(cl_socket==-1){
			if(errno!=EAGAIN)
				fl_syslog(LOG_ERR,"Unable to accept incoming connection! [%s]",strerror(errno));
			else
				usleep(10000);
		}else{
			if((run_pid=fork())==0){
				// Start child process
				fl_syslog(LOG_INFO,
					"Accepted incoming connection from %s:%i",
					inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port));
				// Staring time counter
				start_time=time(NULL);
				// Closing parent's listen socket
				close(ls_socket);
				// Checking for the number of simultaneous connections (by voland)
				// TODO : Important
				// Checking for the GGW_MAX_CLIENTS child_count limit
				if(child_count>=GGW_MAX_CLIENTS){
					fl_syslog(LOG_WARNING,
						"Maximum client number [%i] reached. Connection terminated!",child_count);
					goto client_exit_socket;
				};
				// Starting MySQL session
//				if(mysql_connect(&mysql,NULL,NULL,NULL)==NULL){
//					fl_syslog(LOG_ERR,
//						"Unable to establish connection to MySQL server! [%s]",mysql_error(&mysql));
//					goto client_exit_socket;
//				};
//				// Selecting MySQL database
//				if(mysql_select_db(&mysql,"ggw")==-1){
//					fl_syslog(LOG_ERR,
//						"Unable to select MySQL database! [%s]",mysql_error(&mysql));
//					goto client_exit_mysql;
//				};
				// Initializing MYSQL handle
				mysql_init(&mysql);
				// Establishing connection to MySQL server
				// TODO: Check for command line options!
				if(mysql_real_connect(&mysql,NULL,"apache","Rfdfcfrb","ggw",0,MY_SOCK,0)==NULL){
					fl_syslog(LOG_ERR,
						"Unable to establish connection to MySQL server! [%s]",mysql_error(&mysql));
					goto client_exit_socket;				
				};
				// Waiting a bit to accept connection
				usleep(300000);
				// Starting dialog session
				rcv_tail=0;
				while(session_active){
					// Waiting for a command
					start_recv_wait=time(NULL);
receive_again:
					inet_res=recv(cl_socket,rcv_buf+rcv_tail,RECV_BUFFER_SIZE,MSG_DONTWAIT);
					if(inet_res==-1){
						if(errno==EAGAIN){
							end_recv_wait=time(NULL);
							if((end_recv_wait-start_recv_wait)>GGW_CLIENT_TIMEOUT){
								fl_syslog(LOG_WARNING,
									"Client %s:%i disconnected due to timeout!",
									inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port));
									goto client_exit_mysql;
							};
							usleep(10000);
							goto receive_again;
						}else{
							fl_syslog(LOG_ERR,
								"Unable to receive message from %s:%i! [%s]",
								inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port),strerror(errno));
							goto client_exit_mysql;
						};			    
					}else
						if(inet_res==0){
							// It seems that connection have been closed by client
							fl_syslog(LOG_INFO,
								"Connection from %s:%i closed by client!",
								inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port));
								goto client_exit_mysql;
						};
					//
					rcv_tail+=inet_res;
					bytes_recv+=inet_res;
					// Check if we have at least one entire frame 
					while(rcv_tail>=*((LPDWORD)(rcv_buf+4))){
						// Copying entire request into the command buffer
						memcpy(cmd_buf,rcv_buf,*((LPDWORD)(rcv_buf+4)));
						// Adjusting buffers
						memmove(	rcv_buf,
									rcv_buf+*((LPDWORD)(cmd_buf+4)),
									rcv_tail-*((LPDWORD)(cmd_buf+4)));
						rcv_tail-=*((LPDWORD)(cmd_buf+4));
						// Uncompressing
						dwUncSize=*((LPDWORD)(cmd_buf+8));
						unc_status=uncompress(	
									(unsigned char*)(unc_buf+12),
									&dwUncSize,
									(unsigned char*)(cmd_buf+12),
									*((LPDWORD)(cmd_buf+4))-12);
//									inet_res-12);		// Check this !!!!!!!!!!!!!!
						if(unc_status!=Z_OK){
							// Uncompression error / invalid source data / CRC
							fl_syslog(LOG_ERR,
								"Invalid data received! [%i]",unc_status);
							goto client_exit_mysql;									
						};
						// Check for errors and size mismatch
						if(dwUncSize!=*((LPDWORD)(cmd_buf+8))){
							// Packed/unpacked data size mismatch
							fl_syslog(LOG_ERR,
								"Bad data received! [Unp/Fmt Size=%lu/%lu]",
								dwUncSize,*((LPDWORD)(cmd_buf+8)));		
							goto client_exit_mysql;									
						};
						// Copying header
						memcpy(unc_buf,cmd_buf,12);
						// IN ratio
						ratio=(100*((float)*((LPDWORD)(cmd_buf+4))-12))/(float)(dwUncSize);
						fl_syslog(LOG_INFO,"In compression ratio : %2.2f%%",ratio);
						// Processing command
						strncpy(rcv_command,(char*)(unc_buf+15),*((LPBYTE)(unc_buf+14)));
						rcv_command[*((LPBYTE)(unc_buf+14))]='\0';
						fl_syslog(LOG_DEBUG,"Command [%s] received",rcv_command);
						// Checking for appropriate plugin
						pl_command=get_command(rcv_command);
						if(pl_command!=NULL){
							// Setting global session variables
							glb_gvers=GAME_VERSION(unc_buf);
							glb_glang=LANG_VERSION(unc_buf);
							out_size=0xABCDABCD;
							if((*pl_command)(unc_buf,out_buf,&mysql,&out_size,&out_commands,&rmt_addr)!=-1){
								if(out_size==0xABCDABCD){
									fl_syslog(LOG_ERR,"Command [%s] crashed",rcv_command);
									goto client_exit_mysql;
								};
								fl_syslog(LOG_DEBUG,"Command [%s] processed",rcv_command);
								if(out_size==0)
									continue;
								// Commands
								*((LPWORD)(out_buf+12))=out_commands;
								// Packing data
								dwCmpSize=RECV_BUFFER_SIZE;
								compress(	(unsigned char*)(pck_buf+12),
											&dwCmpSize,
											(unsigned char*)(out_buf+12),
											out_size+2);
								// Marker
								*((LPWORD)(pck_buf+0))=*((LPWORD)(cmd_buf+0));
								// Flags
								*((LPWORD)(pck_buf+2))=*((LPWORD)(cmd_buf+2));
								// Size
								*((LPDWORD)(pck_buf+4))=12+dwCmpSize;
								// CRC32
								*((LPDWORD)(pck_buf+8))=out_size+2;
								// OUT ratio
								ratio=(100*(float)dwCmpSize)/((float)out_size+2);
								fl_syslog(LOG_INFO,"Out compression ratio : %2.2f%%",ratio);
								// Send reply
								inet_res=send(cl_socket,pck_buf,12+dwCmpSize,0);
								if(inet_res==-1){
									fl_syslog(LOG_ERR,
										"Unable to send message to %s:%i! [%s]",
										inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port),strerror(errno));
									goto client_exit_mysql;
								};
								bytes_send+=inet_res;
							}else{
								fl_syslog(LOG_DEBUG,"Command [%s] return error",rcv_command);
								goto client_exit_mysql;
							}
						}else{
							fl_syslog(LOG_ERR,"Command [%s] could not be bind",rcv_command);
							goto client_exit_mysql;
						};
					}; // commands_in_buffer
				}; // (session_active)
client_exit_mysql:		
				// Closing MySQL connection
				mysql_close(&mysql);
client_exit_socket:		
				// Closing socket
				close(cl_socket);

				end_time=time(NULL);
				// Final log
				fl_syslog(LOG_INFO,
					"Connection from %s:%i [in/out=%i/%i] served for %i second(s)",
					inet_ntoa(rmt_addr.sin_addr),ntohs(rmt_addr.sin_port),
					bytes_recv,bytes_send,(end_time-start_time));
					// Exit child process
					exit(0);
			}else{	// Parent process
				// We don't need client socket
				close(cl_socket);
				// We have one more child
				child_count++;
				// Adding child PID to the list
				child_list.push_back(run_pid);
			};
		};
	}; // (!sigterm_received)
	
parent_process_exit:
    // Unloading schedulers
	if(sched_pid!=-1)
		// Sending SIGTERM signal
		if(kill(sched_pid,SIGTERM)!=-1){
			// Waiting for the child to exit
			start_time=time(NULL);
			while(true){
				child_exit_pid=waitpid(sched_pid,NULL,WNOHANG);
				if(child_exit_pid>0)
					break;
				end_time=time(NULL);
				if((end_time-start_time)>WAIT_CHILD_TIME){
					fl_syslog(LOG_CRIT,"Timeout waiting for sched process to exit! [%lu]",WAIT_CHILD_TIME);
					break;
				};
				usleep(50000);
			};
		}else
			fl_syslog(LOG_CRIT,"Unable to send kill() to sched process! [%s]",
				strerror(errno));
    // Unloading httpd
//	if(httpd_pid!=-1)
//		// Sending SIGTERM signal
//		if(kill(httpd_pid,SIGTERM)!=-1){
//			// Waiting for the child to exit
//			start_time=time(NULL);
//			while(true){
//				child_exit_pid=waitpid(httpd_pid,NULL,WNOHANG);
//				if(child_exit_pid>0)
//					break;
//				end_time=time(NULL);
//				if((end_time-start_time)>WAIT_CHILD_TIME){
//					fl_syslog(LOG_CRIT,"Timeout waiting for httpd process to exit! [%lu]",WAIT_CHILD_TIME);
//					break;
//				};
//				usleep(50000);
//			};
//		}else
//			fl_syslog(LOG_CRIT,"Unable to send kill() to httpd process! [%s]",
//				strerror(errno));
	// Unloading and free plugin modules
    free_plugins();
	// Deleting PID file
    unlink(PIDFILE);
	// Closing Log file    
    fl_closelog();
	// Closing listen socket;
    if(ls_socket!=-1)
		close(ls_socket);
	// Closing ping socket;
    if(pg_socket!=-1)
		close(pg_socket);
	// Exiting    
    exit(0);
}

