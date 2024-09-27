#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "entity.h"

using namespace std;

enum PlayerAction {CreateEnemy, PlayerUp, PlayerDown, PlayerRight, PlayerLeft};

class World
{
private:
	vector<Entity*> entities;

public:
	World();
	void clearWorld();
	void addEntity(int x, int y, int width, int height, EntityType type);
	void addPlayer(int x, int y, int width, int height, EntityType type);

	void tick();
};

#endif
