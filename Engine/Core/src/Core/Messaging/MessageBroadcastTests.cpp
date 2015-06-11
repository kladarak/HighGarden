#include "MessageBroadcastTests.h"

#include "MessageBroadcaster.h"

namespace MessageBroadcastTests
{

class Message1
{
public:
	Message1(int inID) : mID(inID) { }
	int mID;
};

class Message2
{
public:
	Message2(const std::string& inID) : mID(inID) { }
	std::string mID;
};

class HandlerA
{
public:
	void Handle(const Message1& inMsg) { printf("HandlerA: Message1: ID: %i\n", inMsg.mID); }
};

class HandlerB
{
public:
	void Handle(const Message1& inMsg) { printf("HandlerB: Message1: ID: %i\n", inMsg.mID); }
	void Handle(const Message2& inMsg) { printf("HandlerB: Message2: ID: %s\n", inMsg.mID.c_str()); }
};

void DoTests()
{
	MessageBroadcaster broadcaster;
	HandlerA handlerA;
	HandlerB handlerB;

	broadcaster.Register<Message1>( [&] (const Message1& inMessage) { handlerA.Handle(inMessage); } );
	broadcaster.Register<Message2>( [&] (const Message2& inMessage) { handlerB.Handle(inMessage); } );
	MessageRegistrationHandle regHandler = broadcaster.Register<Message1>( [&] (const Message1& inMessage) { handlerB.Handle(inMessage); } );

	broadcaster.Broadcast( Message1(42) );
	broadcaster.Broadcast( Message2("Yo") );

	regHandler.Unregister();
	
	broadcaster.Broadcast( Message1(69) );
}

}
