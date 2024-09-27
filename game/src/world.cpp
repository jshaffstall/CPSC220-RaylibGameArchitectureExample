#include <iostream>
#include <cstdlib>
#include <ctime>
#include "world.h"
#include "player_entity.h"
#include "pubsub.h"
#include "raylib.h"

World::World()
{
	srand(time(NULL));
}

void World::tick()
{
	for (Entity *entity : entities)
		entity->tick();
}

void World::clearWorld()
{
	for (Entity* entity : entities)
	{
		// Let everyone know we're deleting this entity
		PubSub::publish("entity", "delete", entity);
		delete entity;
	}

	entities.clear();
}

void World::addEntity(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new Entity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

void World::addPlayer(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new PlayerEntity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

