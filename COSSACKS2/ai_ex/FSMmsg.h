#ifndef __FSM_MESSAGE__
#define __FSM_MESSAGE__


/*
*************** MESSAGE NAME *************** 
*	MSG_NULL,
*	MSG_ENTER,	//Don't send this message - the system sends this when a state is first entered - Use OnInitialize to listen for it
*	MSG_EXIT,	//Don't send this message - the system sends this when a state is exited - Use OnExit to listen for it
*	MSG_UPDATE,	//Don't send this message - the system sends this when a game tick goes by - Use OnUpdate to listen for it
********************************************
*/

/*
*************** MESSAGE CLASS ***************
*/
class	be_CSMARTOBJECT;
class	be_CMsgObject : public BaseClass
{
public:
	be_CMsgObject()	{ sender_ref.Set(NULL); receiver_ref.Set(NULL); };

	_str		name;			//name of message (an enumerated type works well)
	_str		sender_id;		//unique id of sender
	_str		receiver_id;	//unique id of receiver
	int			delivery_time;	//deliver message at this time

	// For incress speed of get SO
	ClassRef<be_CSMARTOBJECT>	sender_ref;
	ClassRef<be_CSMARTOBJECT>	receiver_ref;

	virtual	int	GetType() { return 0; };

	SAVE(be_CMsgObject);
		REG_AUTO(name);
		REG_AUTO(sender_id);
		REG_AUTO(receiver_id);
		REG_MEMBER(_int,delivery_time);
	ENDSAVE;
};	

class	 be_CDelayedMessage : public BaseClass
{
public:
	be_CMsgObject		msg;
	bool				bSent;

//	be_CDelayedMessage*	next;

	SAVE(be_CDelayedMessage);
		REG_AUTO(msg);
		REG_MEMBER(_bool,bSent);
	ENDSAVE;
};

class	be_CMasterDelayedMessage : public BaseClass
{
public:
	~be_CMasterDelayedMessage();
	ClonesArray<be_CDelayedMessage>	msgStore;
//	be_CDelayedMessage*		head;
	SAVE(be_CMasterDelayedMessage);
		REG_AUTO(msgStore);
	ENDSAVE;
};

extern	be_CMasterDelayedMessage g_beMasterDelayedMessage;
/*
*************** MESSAGE ROUTER ***************
*/
void g_beSendMsg			( const char* name, const char* sender, const char* receiver, be_CSMARTOBJECT* pS=NULL, be_CSMARTOBJECT* pR=NULL );
void g_beSendDelayedMsg		( const char* name, int delay, const char* sender, const char* receiver, be_CSMARTOBJECT* pS=NULL, be_CSMARTOBJECT* pR=NULL );
void g_beSendDelayedMessages();
void g_beRouteMessage		( be_CMsgObject* msg );
void g_beInitDelayedMessages();

/*
**************** REGESTRATION ****************
*/
void g_beREG_MSG_ROUTER_CLASS();

#endif//__FSM_MESSAGE__






























