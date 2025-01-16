#include "Ball.h"
#include "RugbyScene.h"

#include "Debug.h"

void Ball::OnInitialize()
{
	
}

void Ball::OnUpdate()
{
	if (mOwner) {
		SetSpeed(0);
		sf::Vector2f position = mOwner->GetPosition();
		SetPosition(position.x, position.y);

		Debug::DrawText(position.x, position.y, "Owner", 0.5f, 0.5f, sf::Color::Green);

	}
	else if (mPreviousOwner) {

		sf::Vector2f position = mPreviousOwner->GetPosition();
		Debug::DrawText(position.x, position.y, "Previous Owner", 0.5f, 0.5f, sf::Color::Green);
	}
	else {
		SetSpeed(mSpeed);
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
	//GoToDirection(direction.x, direction.y, 1000.f); // La balle peut sortir de l'écran avec ca
	GoToPosition(direction.x, direction.y, mSpeed); // La balle s'arretera a la pos du joueur
	mPreviousOwner = mOwner;
	mOwner = nullptr;
}
