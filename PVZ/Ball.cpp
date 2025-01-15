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
		auto truc = dynamic_cast<RugbyMan*>(pCollidedWith);

		if (truc == mPreviousOwner)
			return;

		mOwner = truc;
	}
}

void Ball::OnShoot(sf::Vector2f direction)
{
	GoToPosition(direction.x, direction.y, 1000.f);
	mPreviousOwner = mOwner;
	mOwner = nullptr;
}
