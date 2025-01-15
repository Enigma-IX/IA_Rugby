#pragma once

#include "Entity.h"
#include "RugbyMan.h"

class Projectile : public Entity
{
protected:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnCollision(Entity* pCollidedWith) override;


private:

	RugbyMan* mOwner;
};

