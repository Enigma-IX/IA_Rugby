#include "RugbyMan.h"

#include <string>
#include <algorithm>

#include "Utils.h"

#include "RugbyScene.h"

#include "StateMachine.h"
#include "RugbyManCondition.h"
#include "RugbyManAction.h"

#define RUGBYMAN_MAX_SPEED 120.f
#define RUGBYMAN_MIN_SPEED 90.f

#define RUGBYMAN_MAX_FOLLOWOFFSET 110.f
#define RUGBYMAN_MIN_FOLLOWOFFSET 50.f

#include "Debug.h"

void RugbyMan::OnInitialize()
{
	mpStateMachine = new StateMachine<RugbyMan>(this, State::Count);	

	mbaseSpeed = Utils::RandomFloat(RUGBYMAN_MIN_SPEED, RUGBYMAN_MAX_SPEED);
	mfollowerOffset = Utils::RandomFloat(RUGBYMAN_MIN_FOLLOWOFFSET, RUGBYMAN_MAX_FOLLOWOFFSET);

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

bool RugbyMan::GoToPosition(int x, int y, float speed)
{
	sf::Vector2f position = GetPosition();

	AABB* pAssignedArea = GetScene<RugbyScene>()->GetArea(mAreaIndex);

	float clampedX = Clamp<float>(x, pAssignedArea->xMin, pAssignedArea->xMax);
	float clampedY = Clamp<float>(y, pAssignedArea->yMin, pAssignedArea->yMax);

	if (GoToDirection(static_cast<int>(clampedX), static_cast<int>(clampedY), speed) == false) {
		return false;
	}

	mTarget.position = { static_cast<int>(clampedX), static_cast<int>(clampedY) };
	mTarget.distance = Utils::GetDistance(position.x, position.y, clampedX, clampedY);
	mTarget.isSet = true;

	return true;
}

void RugbyMan::OnCollision(Entity* pCollidedWith)
{

	if (pCollidedWith->IsTag(RugbyScene::Tag::RUGBYMAN))
	{
		Ball* ball = GetScene<RugbyScene>()->GetBall();
		RugbyMan* rugbyMan = dynamic_cast<RugbyMan*>(pCollidedWith);

		if (rugbyMan->mTimeSinceLastShot < 2.f)
			return;

		if (ball->mOwner != rugbyMan)
			return;

		if (rugbyMan->mTeam != mTeam)
		{
			ball->mOwner = this;
			ball->mPreviousOwner = nullptr;
		}
	}
	
}


void RugbyMan::Shoot(RugbyMan* target)
{
	Ball* ball = GetScene<RugbyScene>()->GetBall();

	if (target)
	{
		ball->OnShoot(target->GetPosition());
	}
}


bool RugbyMan::IsMouseOver(const sf::Vector2i& mousePos) const
{
	sf::Vector2f pos = GetPosition();
	float distance = std::sqrt(std::pow(mousePos.x - pos.x, 2) + std::pow(mousePos.y - pos.y, 2));
	return distance <= GetRadius();
}

