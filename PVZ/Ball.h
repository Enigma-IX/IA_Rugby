#pragma once
#include "Entity.h"
#include "RugbyMan.h"

class Ball : public Entity
{
protected:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnCollision(Entity* pCollidedWith) override;

	float mSpeed = 1000.0f;

public:
	void OnShoot(sf::Vector2f direction);

	RugbyMan* mOwner;
	RugbyMan* mPreviousOwner;

};

