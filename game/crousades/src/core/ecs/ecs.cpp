#include "pch.h"
#include "ecs.h"

entity CreateEntity()
{
	static entity next = 0;
	return next++;
}
