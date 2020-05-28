#pragma once

#include "platform.h"

typedef uint32_t entity;

static const entity INVALID_ENTITY = 0;

entity CreateEntity();

struct position 
{
	float x, y;
};

template<typename Component>
class ComponentManager
{
private:
	std::vector<Component> components;
	std::vector<entity> entities;
	std::unordered_map<entity, size_t> lookup;

public:
	bool Contains(entity entity_id)
	{
		return lookup.find(entity_id) != lookup.end();
	}

	const size_t Size() const { return components.size(); }
	const size_t GetEntity(size_t index) { return entities[index]; }
	Component& operator[] (size_t index) { return components[index]; }

	Component& Create(entity entity_id)
	{
		assert(entity_id != INVALID_ENTITY);
		assert(!Contains(entity_id));

		//check for what?
		assert(entities.size() == components.size());
		assert(lookup.size() == components.size());

		lookup[entity_id] = components.size();
		components.push_back(Component());
		entities.push_back(entity);

		return components.back();
	}

	Component& GetComponent(entity entity_id)
	{
		auto it = lookup[entity_id].find(entity_id);

		if (it != lookup.end())
		{
			return &components[it->second];
		}

		return nullptr;
	}

	void Remove(entity entity_id)
	{
		// todo: what do if id don't exist?

		assert(Contains(entity_id));

		auto it = lookup.find(entity_id);
		if (it != lookup.end()) 
		{
			size_t index = it->second;
			
			if (index < components.size() - 1)
			{
				// todo: google to move semantic!!
				components[index] = std::move(components.back());
				entities[index] = entities.back();

				lookup[entities[index]] = index;
			}
		}

		components.pop_back();
		entities.pop_back();
		lookup.erase(entity_id);
	}
};
