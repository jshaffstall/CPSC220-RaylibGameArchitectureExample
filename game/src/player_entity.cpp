#include <iostream>
#include "raylib.h"
#include "player_entity.h"
#include "world.h"

#include <iostream>

PlayerEntity::PlayerEntity(float x, float y, int width, int height, EntityType type)
	: Entity(x, y, width, height, type)
{
	PubSub::subscribe("action", this);

	movementCountdown = 0;
	targetX = 0;
	targetY = 0;
	speedX = 0;
	speedY = 0;
}

void PlayerEntity::tick()
{
	if (movementCountdown > 0)
	{
		movementCountdown--;

		if (movementCountdown == 0)
		{
			setMoving(false);
			setX(targetX);
		}
		else
		{
			setX(getX() + speedX);
		}
	}

	speedY += 0.5;

	setY(getY() + speedY);
	setFalling(true);

	Vector2 position = { getX(), getY() };
	PubSub::publish("player", "location", &position);
}

void PlayerEntity::receiveMessage(string channel, string message, void* data)
{
	if (channel == "action" && message == "player")
	{
		PlayerAction* action = (PlayerAction*)data;

		if (*action == PlayerRight)
		{
			targetX = getX() + 32;
			targetY = getY();
			speedX = 32.0 / 60.0;
			setMoving(true);
			setFacing(Right);
			movementCountdown = 60;
		}

		if (*action == PlayerLeft)
		{
			targetX = getX() - 32;
			targetY = getY();
			speedX = -32.0 / 60.0;
			setMoving(true);
			setFacing(Left);
			movementCountdown = 60;
		}

		if (*action == PlayerJump)
		{
			speedY = -5;
		}

	}
}

bool PlayerEntity::handleCollisions()
{
	for (Entity* entity : collisions)
	{
		if (entity->getType() == Obstacle)
		{
			int xDistance = abs(getX() - entity->getX());
			int yDistance = abs(getY() - entity->getY());

			if (xDistance > yDistance)
			{
				setMoving(false);
				movementCountdown = 0;

				setX(getX() - speedX);
			}
			else
			{
				if (getFalling())
				{
					setFalling(false);
					setY(getY() - speedY);
					speedY = 0;
				}
			}
		}
	}

	collisions.clear();
	return false;
}
