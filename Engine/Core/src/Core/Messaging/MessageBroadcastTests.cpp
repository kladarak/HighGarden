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

	broadcaster.Register<Message1, HandlerA, &HandlerA::Handle>(&handlerA);
	broadcaster.Register<Message1, HandlerB, &HandlerB::Handle>(&handlerB);
	broadcaster.Register<Message2, HandlerB, &HandlerB::Handle>(&handlerB);

	broadcaster.Broadcast( Message1(42) );
	broadcaster.Broadcast( Message2("Yo") );

	broadcaster.Unregister<Message1>(&handlerB);
	
	broadcaster.Broadcast( Message1(69) );
}

}
