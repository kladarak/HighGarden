#include <cstdio>

#include <Core/Messaging/MessageBroadcastTests.h>
#include <EntityComponentSystem/Tests/EntityComponentSystemTests.h>

int main()
{
	MessageBroadcastTests::DoTests();
	EntityComponentSystemTests::DoTests();

	while (true);
	return 0;
}