#include "Ball.h"
#include "RugbyScene.h"

void Ball::OnInitialize()
{
	
}

void Ball::OnUpdate()
{
	if (mOwner) {
		SetSpeed(0);
		sf::Vector2f position = mOwner->GetPosition();
		SetPosition(position.x, position.y);
	}
	else {
		SetSpeed(1000);
	}
}

void Ball::OnCollision(Entity* pCollidedWith)
{
	if (mOwner)
		return;

	if (pCollidedWith->IsTag(RugbyScene::Tag::RUGBYMAN))
	{
		auto owner = dynamic_cast<RugbyMan*>(pCollidedWith);

		if (owner == mPreviousOwner)
			return;

		mOwner = owner;
	}
}

void Ball::OnShoot(sf::Vector2f direction)
{
	GoToDirection(direction.x, direction.y, 1000.f);
	mPreviousOwner = mOwner;
	mOwner = nullptr;
}
