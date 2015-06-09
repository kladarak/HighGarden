#include <EntityComponentSystem/Tests/EntityComponentSystemTests.h>
#include <Core/Messaging/MessageBroadcastTests.h>


int main()
{
	MessageBroadcastTests::DoTests();
	EntityComponentSystemTests::DoTests();

	while (true);
	return 0;
}
