#include "RugbyManAction.h"
#include "RugbyScene.h"

void RugbyManAction_HasBall::OnStart(RugbyMan* pRugbyMan)
{
	RugbyScene* pRugbyScene = pRugbyMan->GetScene<RugbyScene>();
	int witdh = pRugbyScene->GetWindowWidth();

	int goalDirection = pRugbyMan->mTeam;
	int newDirection = goalDirection == 0 ? witdh : 0;

	pRugbyMan->GoToDirection(newDirection, pRugbyMan->GetPosition().y, 100.f);
}

void RugbyManAction_HasBall::OnUpdate(RugbyMan* pRugbyMan)
{
	std::vector<RugbyMan*> rugbyMen = pRugbyMan->GetScene<RugbyScene>()->GetRugbyMen();

	if (rugbyMen.empty()) {
		return;
	}

	RugbyMan* closestRugbyMan = nullptr;

	float minDistance = 100;

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		if (rugbyMan->mTeam != pRugbyMan->mTeam)
		{
			float distance = std::sqrt(
				std::pow(rugbyMan->GetPosition().x - pRugbyMan->GetPosition().x, 2) +
				std::pow(rugbyMan->GetPosition().y - pRugbyMan->GetPosition().y, 2)
			);

			if (distance < minDistance)
			{
				pRugbyMan->Shoot();
			}
		}
	}
}


void RugbyManAction_TeamHasBall::OnUpdate(RugbyMan* pRugbyMan)
{
	RugbyMan* owner = pRugbyMan->GetScene<RugbyScene>()->GetBall()->mOwner;

	if (owner == nullptr)
	{
		return;
	}

	sf::Vector2f ownerPosition = owner->GetPosition();

	int offset;

	if (owner->mTeam != 0)
	{
		offset = 200;
	}
	else
	{
		offset = -200;
	}

	pRugbyMan->GoToPosition(ownerPosition.x + offset, pRugbyMan->GetPosition().y, 100.f);
}

void RugbyManAction_EnemyHasBall::OnUpdate(RugbyMan* pRugbyMan)
{
	Ball* ball = pRugbyMan->GetScene<RugbyScene>()->GetBall();

	if (ball == nullptr)
	{
		return;
	}

	sf::Vector2f ballPosition = ball->GetPosition();

	pRugbyMan->GoToPosition(ballPosition.x, ballPosition.y, 100.f);
}
