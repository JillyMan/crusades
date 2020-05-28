#include "pch.h"

#ifdef TEST 

#include "core/ecs/ecs.h

int main() 
{
	auto entity_id = CreateEntity();

	ComponentManager<position> component_manager;
	component_manager.Contains(entity_id);


	return (0);
}

#endif