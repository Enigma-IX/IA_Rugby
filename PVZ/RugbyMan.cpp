#include "RugbyMan.h"

#include <string>
#include <algorithm>

#include "Utils.h"

#include "RugbyScene.h"

#include "StateMachine.h"
#include "RugbyManCondition.h"
#include "RugbyManAction.h"


#include "Debug.h"

void RugbyMan::OnInitialize()
{
	mpStateMachine = new StateMachine<RugbyMan>(this, State::Count);

	mAreaIndex = -1;

	SetTag(RugbyScene::Tag::RUGBYMAN);

	//-> HAVING THE BALL
	{
		Action<RugbyMan>* pHavingTheBall = mpStateMachine->CreateAction<RugbyManAction_HasBall>(State::HavingTheBall);

		//-> TEAMMATE WITH BALL
		{
			auto transition = pHavingTheBall->CreateTransition(State::TeamMateWithBall);

			auto condition = transition->AddCondition<RugbymanCondition_TeamHasBall>();
		}

		//-> ENEMY WITH BALL
		{
			auto transition = pHavingTheBall->CreateTransition(State::EnemyWithBall);

			auto condition = transition->AddCondition<RugbymanCondition_TeamHasNoBall>();
		}
	}	

	//-> TEAMMATE WITH BALL
	{
		Action<RugbyMan>* pTeamMateWithBall = mpStateMachine->CreateAction<RugbyManAction_TeamHasBall>(State::TeamMateWithBall);


		//-> HAVING THE BALL
		{
			auto transition = pTeamMateWithBall->CreateTransition(State::HavingTheBall);

			auto condition = transition->AddCondition<RugbymanCondition_HasTheBall>();
		}
		//-> ENEMY WITH BALL
		{
			auto transition = pTeamMateWithBall->CreateTransition(State::EnemyWithBall);

			auto condition = transition->AddCondition<RugbymanCondition_TeamHasNoBall>();
		}
	}

	//-> ENEMY WITH BALL
	{
		Action<RugbyMan>* pEnemyWithBall = mpStateMachine->CreateAction<RugbyManAction_EnemyHasBall>(State::EnemyWithBall);

		//-> HAVING THE BALL
		{
			auto transition = pEnemyWithBall->CreateTransition(State::HavingTheBall);

			auto condition = transition->AddCondition<RugbymanCondition_HasTheBall>();
		}
		//-> TEAMMATE WITH BALL
		{
			auto transition = pEnemyWithBall->CreateTransition(State::TeamMateWithBall);

			auto condition = transition->AddCondition<RugbymanCondition_TeamHasBall>();
		}
	}

	mpStateMachine->SetState(State::EnemyWithBall);
}



void RugbyMan::OnUpdate()
{
	const sf::Vector2f& position = GetPosition();
	const char* stateName = GetStateName((RugbyMan::State)mpStateMachine->GetCurrentState());

	Debug::DrawText(position.x, position.y - 50, stateName, 0.5f, 0.5f, sf::Color::Red);

	mpStateMachine->Update();
}

const char* RugbyMan::GetStateName(State state) const
{
	switch (state)
	{
	case EnemyWithBall: return "EnemyWithBall";
	case HavingTheBall: return "HavingTheBall";
	case TeamMateWithBall: return "TeamMateWithBall";
	default: return "Unknown";
	}
}

void RugbyMan::ClampPosition() {
	sf::Vector2f position = GetPosition();

	AABB* pAssignedArea = GetScene<RugbyScene>()->GetArea(mAreaIndex);

	float clampedX = Clamp<float>(position.x, pAssignedArea->xMin, pAssignedArea->xMax);
	float clampedY = Clamp<float>(position.y, pAssignedArea->yMin, pAssignedArea->yMax);

	Entity::SetPosition(clampedX, clampedY);
}

void RugbyMan::SetPosition(float x, float y, float ratioX, float ratioY) {
	Entity::SetPosition(x, y); // Appelle la méthode de base pour changer la position
	ClampPosition();          // Restreint ensuite dans l'area
}

void RugbyMan::OnCollision(Entity* pCollidedWith)
{	
	

	if (pCollidedWith->IsTag(RugbyScene::Tag::RUGBYMAN))
	{
		RugbyMan* rugbyMan = dynamic_cast<RugbyMan*>(pCollidedWith);

		if (rugbyMan->mTeam != mTeam)
		{
			Ball* ball = GetScene<RugbyScene>()->GetBall();

			if (ball->mOwner == rugbyMan)
			{
				ball->mOwner = this;
				ball->mPreviousOwner = nullptr;
			}
		}
	}
	
}


void RugbyMan::Shoot()
{
	std::vector<RugbyMan*> rugbyMen = GetScene<RugbyScene>()->GetRugbyMen();

	if (rugbyMen.empty()) {
		return;
	}

	RugbyMan* closestRugbyMan = nullptr;
	Ball* ball = GetScene<RugbyScene>()->GetBall();

	float minDistance = std::numeric_limits<float>::max();

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		if (rugbyMan != this && rugbyMan->mTeam == ball->mOwner->mTeam)
		{
			float distance = std::sqrt(
				std::pow(rugbyMan->GetPosition().x - GetPosition().x, 2) +
				std::pow(rugbyMan->GetPosition().y - GetPosition().y, 2)
			);

			if (distance < minDistance)
			{
				minDistance = distance;
				closestRugbyMan = rugbyMan;
			}
		}
	}

	if (closestRugbyMan)
	{
		ball->OnShoot(closestRugbyMan->GetPosition());
	}
}


bool RugbyMan::IsMouseOver(const sf::Vector2i& mousePos) const
{
	sf::Vector2f pos = GetPosition();
	float distance = std::sqrt(std::pow(mousePos.x - pos.x, 2) + std::pow(mousePos.y - pos.y, 2));
	return distance <= GetRadius();
}

