#include "RugbyManAction.h"
#include "RugbyScene.h"

#include "Debug.h"

void RugbyManAction_HasBall::OnStart(RugbyMan* pRugbyMan)
{
	RugbyScene* pRugbyScene = pRugbyMan->GetScene<RugbyScene>();
	int witdh = pRugbyScene->GetWindowWidth();

	int goalDirection = pRugbyMan->mTeam;
	int newDirection = goalDirection == 0 ? witdh : 0;

	pRugbyMan->GoToDirection(newDirection, pRugbyMan->GetPosition().y, pRugbyMan->mbaseSpeed + 30.f);

	pRugbyMan->mTimeSinceLastShot = 0;
}

void RugbyManAction_HasBall::OnUpdate(RugbyMan* pRugbyMan)
{
	AABB* goal = pRugbyMan->GetScene<RugbyScene>()->GetGoal(pRugbyMan->mTeam);

	float percentage = 0.1f;

	if (pRugbyMan->mTeam != 0)
	{
		percentage = 0.9f;
	}

	if (pRugbyMan->GetPosition(percentage).x > goal->xMin && pRugbyMan->GetPosition(percentage).x < goal->xMax)
	{
		pRugbyMan->GetScene<RugbyScene>()->Reset(pRugbyMan->mTeam);
		return;
	}


	pRugbyMan->mTimeSinceLastShot += pRugbyMan->GetDeltaTime();


	if (pRugbyMan->mTimeSinceLastShot < 3.f)
		return;	

	pRugbyMan->SetSpeed(pRugbyMan->mbaseSpeed);


	std::vector<RugbyMan*> rugbyMen = pRugbyMan->GetScene<RugbyScene>()->GetRugbyMen();

	if (rugbyMen.empty()) {
		return;
	}

	float minDistance = 100;

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		if (rugbyMan->mTeam != pRugbyMan->mTeam)
		{
			float distance = std::sqrt(
				std::pow(rugbyMan->GetPosition().x - pRugbyMan->GetPosition().x, 2) +
				std::pow(rugbyMan->GetPosition().y - pRugbyMan->GetPosition().y, 2)
			);

			if (pRugbyMan->mTeam == 0)
			{
				if (rugbyMan->GetPosition().x < pRugbyMan->GetPosition().x)
					continue;
			}
			else
			{
				if (rugbyMan->GetPosition().x > pRugbyMan->GetPosition().x)
					continue;
			}

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

	int offset = pRugbyMan->mfollowerOffset;

	if (owner->mTeam == 0)
	{
		offset = -offset;
	}

	pRugbyMan->GoToPosition(ownerPosition.x + offset, pRugbyMan->GetPosition().y, pRugbyMan->mbaseSpeed);
}

void RugbyManAction_EnemyHasBall::OnUpdate(RugbyMan* pRugbyMan)
{
	Ball* ball = pRugbyMan->GetScene<RugbyScene>()->GetBall();

	if (ball == nullptr)
	{
		return;
	}

	sf::Vector2f ballPosition = ball->GetPosition();

	pRugbyMan->GoToPosition(ballPosition.x, ballPosition.y, pRugbyMan->mbaseSpeed);
}
