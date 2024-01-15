#include "stdheader.h"
#include "../UnitAbility.h"
#include "../BE_HEADERS.h"

be_CMasterDelayedMessage::~be_CMasterDelayedMessage(){
	msgStore.Clear();
/*
	be_CDelayedMessage* pDM = head;
	while ( pDM!=0 ) {
		head = head->next;
		delete pDM;
		pDM  = head;
	};
*/
};

// INTERNAL LOCAL VARIABLES
be_CMasterDelayedMessage g_beMasterDelayedMessage;


void g_beStoreDelayedMessage	( be_CMsgObject* msg );
void g_bePushBackDelayedMessage	( be_CDelayedMessage* msg );


void g_beSendMsg				( const char* name, const char* sender, const char* receiver, be_CSMARTOBJECT* pS/*=NULL*/, be_CSMARTOBJECT* pR/*=NULL*/ )
{
	be_CMsgObject msg;
	msg.name			= name;			//The name of the message
	msg.sender_id		= sender;		//The sender
	msg.receiver_id		= receiver;		//The receiver
	msg.delivery_time	= TrueTime;		//Send the message NOW

	msg.sender_ref.Set(pS);
	msg.receiver_ref.Set(pR);

	g_beRouteMessage( &msg );
};

void g_beSendDelayedMsg			( const char* name, int delay, const char* sender, const char* receiver, be_CSMARTOBJECT* pS/*=NULL*/, be_CSMARTOBJECT* pR/*=NULL*/ )
{
	be_CMsgObject msg;
	msg.name			= name;					//The name of the message
	msg.sender_id		= sender;				//The sender
	msg.receiver_id		= receiver;				//The receiver
	msg.delivery_time	= TrueTime + delay;		//Send the message at a future time

	msg.sender_ref.Set(pS);
	msg.receiver_ref.Set(pR);

	g_beRouteMessage( &msg );
};


void g_beRouteMessage			( be_CMsgObject* msg )
{
	void UnitAbilityTest();
	be_CSMARTOBJECT* so = NULL;
	if ( msg->receiver_ref.Get()!=NULL )	so=(be_CSMARTOBJECT*)(msg->receiver_ref.Get());
	else									so=g_beGetSMARTOBJECT(msg->receiver_id.pchar());
	if( so==NULL )	return;

	if( msg->delivery_time > TrueTime )
	{  //This message needs to be stored until its time to send it
		g_beStoreDelayedMessage( msg );
		return;
	};

	if ( so->ProcessStateMachine( so->stateName.pchar(), msg ) == false ){
		so->ProcessStateMachine( "ST_GLOBAL", msg );
	};
	
	// Check for a state change
	while( so->force_state_change )
	{	//Note: circular logic (state changes causing state changes)
		//could cause an infinite loop here - protect against this

		//Create a general msg for initializing and cleaning up the state change
		be_CMsgObject tempmsg;
		tempmsg.receiver_id = so->Name;
		tempmsg.sender_id	= so->Name;

		tempmsg.receiver_ref.Set(so);
		tempmsg.sender_ref.Set(so);

		so->force_state_change = false;

		//Let the last state clean-up
		tempmsg.name = "MSG_EXIT";
		so->ProcessStateMachine( so->stateName.pchar(), &tempmsg );

		//Set the new state
		so->stateName = so->next_stateName;

		//Let the new state initialize
		tempmsg.name = "MSG_ENTER";
		so->ProcessStateMachine( so->stateName.pchar(), &tempmsg );
	};

};


void g_beInitDelayedMessages	()
{
	g_beMasterDelayedMessage.msgStore.Clear();
//	g_beMasterDelayedMessage.head = NULL;
};

void g_beStoreDelayedMessage	( be_CMsgObject* msg )
{
	//Store this message (in some data structure) for later routing

	//A priority queue would be the ideal data structure (but not required)
	//to store the delayed messages.

	//Note: In main game loop call SendDelayedMessages() every game 
	//      tick to check if its time to send the stored messages

	be_CDelayedMessage* newDelayedMessage		= new be_CDelayedMessage;

	newDelayedMessage->msg.name				= msg->name;
	newDelayedMessage->msg.receiver_id		= msg->receiver_id;
	newDelayedMessage->msg.sender_id		= msg->sender_id;

	newDelayedMessage->msg.receiver_ref.Set( msg->receiver_ref.Get() );
	newDelayedMessage->msg.sender_ref.Set( msg->sender_ref.Get() );

	newDelayedMessage->msg.delivery_time	= msg->delivery_time;
//	newDelayedMessage->next					= NULL;
	newDelayedMessage->bSent				= false;

	g_bePushBackDelayedMessage( newDelayedMessage );

};

void g_beSendDelayedMessages	()
{  //This function is called every game tick

	int msgN	= g_beMasterDelayedMessage.msgStore.GetAmount();
	int	curTime = TrueTime;
	be_CDelayedMessage*	cur_msg = NULL;
	while (msgN--) {
		cur_msg = g_beMasterDelayedMessage.msgStore[msgN];
		if( cur_msg!=NULL && cur_msg->msg.delivery_time <= TrueTime )
		{
			cur_msg->bSent = true;
			g_beRouteMessage( &cur_msg->msg );
			g_beMasterDelayedMessage.msgStore.DelElement(msgN);
		};
	};

/*
	be_CDelayedMessage*	cur_msg	= g_beMasterDelayedMessage.head;
	be_CDelayedMessage*	last	= NULL;
	int					curTime = TrueTime;

	while( cur_msg != 0 )
	{
		if( cur_msg->msg.delivery_time <= TrueTime )
		{
			cur_msg->bSent = true;
			g_beRouteMessage( &cur_msg->msg );
			if( last != NULL ) {
				last->next = cur_msg->next;
				delete cur_msg;
				cur_msg = last->next;
			}else{
				g_beMasterDelayedMessage.head = cur_msg->next;
				delete cur_msg;
				cur_msg = g_beMasterDelayedMessage.head;
			};
		}else if( cur_msg != 0 ) {
			last	= cur_msg;
			cur_msg = cur_msg->next;
		};
	};
*/
};

void g_bePushBackDelayedMessage	( be_CDelayedMessage* msg )
{
	int msgN = g_beMasterDelayedMessage.msgStore.GetAmount();
	be_CDelayedMessage* curDelayedMessage = NULL;
	while (msgN--) {
		curDelayedMessage = g_beMasterDelayedMessage.msgStore[msgN];
		if (curDelayedMessage!=NULL) {
			if( curDelayedMessage->bSent			== false				&&
				curDelayedMessage->msg.name			== msg->msg.name		&&
				curDelayedMessage->msg.receiver_id	== msg->msg.receiver_id &&
				curDelayedMessage->msg.sender_id	== msg->msg.sender_id	 )
			{
				//Already on queue - don't add
				return;
			};
		}
	};
	g_beMasterDelayedMessage.msgStore.Add(msg);
/*
	be_CDelayedMessage* curDelayedMessage = g_beMasterDelayedMessage.head;

	if( curDelayedMessage != NULL ){
		while( curDelayedMessage->next != NULL )
		{
			if( curDelayedMessage->bSent			== false				&&
				curDelayedMessage->msg.name			== msg->msg.name		&&
				curDelayedMessage->msg.receiver_id	== msg->msg.receiver_id &&
				curDelayedMessage->msg.sender_id	== msg->msg.sender_id	 )
			{
				//Already on queue - don't add
				return;
			};
			curDelayedMessage = curDelayedMessage->next;
		}

		curDelayedMessage->next = msg;
	}else{
		g_beMasterDelayedMessage.head = msg;
	};
*/
};
//////////////////////////////////////////////////////////////////////////

/*
**************** REGESTRATION ****************
*/
void g_beREG_MSG_ROUTER_CLASS(){
	REG_CLASS(be_CMsgObject);	
	REG_CLASS(be_CDelayedMessage);
	REG_CLASS(be_CMasterDelayedMessage);
};

//////////////////////////////////////////////////////////////////////////


































