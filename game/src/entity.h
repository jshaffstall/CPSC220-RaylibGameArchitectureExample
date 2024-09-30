#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

using namespace std;

enum EntityType {Cat, Obstacle};
enum EntityFacing {Up, Down, Left, Right};

class Entity
{
private:
	float x;
	float y;
	int width;
	int height;
	EntityType type;
	EntityFacing facing;
	bool moving;

protected:
	vector<Entity*> collisions;

public:
	Entity(float x, float y, int width, int height, EntityType type, EntityFacing facing = Right);

	float getX();
	float getY();
	int getWidth();
	int getHeight();
	EntityType getType();
	EntityFacing getFacing();
	bool getMoving();

	void setX(float x);
	void setY(float y);
	void setFacing(EntityFacing facing);
	void setMoving(bool moving);

	virtual void tick();

	void addCollision(Entity* entity);
	virtual bool handleCollisions();	
};

#endif
