#include <cstdio>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponentSystem/Components/PositionComponent.h>

int main()
{
	World world;

	auto entity1 = EntityBuilder(world)
					.AddComponent<PositionComponent>(2.0f)
					.AddComponent<VelocityComponent>(-1.0f, 1.0f, 2.0f)
					.Create();

	const std::string& posStr = entity1.GetComponent<PositionComponent>()->ToString();
	const std::string& velStr = entity1.GetComponent<VelocityComponent>()->ToString();
	printf("%s\n", posStr.c_str());
	printf("%s\n", velStr.c_str());

	entity1.RemoveComponent<PositionComponent>();
	if (entity1.HasComponent<PositionComponent>())
	{
		printf("Still has PositionComponent!\n");
	}
	else
	{
		printf("No longer has PositionComponent\n");
	}

	entity1.Kill();

	if (entity1.IsAlive())
	{
		printf("Still Alive!\n");
	}
	else
	{
		printf("No longer alive.\n");
	}

	if (entity1.HasComponent<VelocityComponent>())
	{
		printf("Still has VelocityComponent!\n");
	}
	else
	{
		printf("No longer has VelocityComponent\n");
	}

	while (true);
	return 0;
}