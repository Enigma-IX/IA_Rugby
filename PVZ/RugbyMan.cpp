#include "RugbyMan.h"

#include <string>

#include "RugbyScene.h"
#include "Projectile.h"

#include "StateMachine.h"

#include "Debug.h"

void RugbyMan::OnInitialize()
{
	//mpStateMachine = new StateMachine<Plant>(this, State::Count);

	//mAreaIndex = -1;
	//mAmmo = mMaxAmmo;

	//SetTag(PVZScene::Tag::PLANT);

	////IDLE
	//{
	//	Action<Plant>* pIdle = mpStateMachine->CreateAction<PlantAction_Idle>(State::Idle);

	//	//-> SHOOTING
	//	{
	//		auto transition = pIdle->CreateTransition(State::Shooting);

	//		auto condition = transition->AddCondition<PlantCondition_ZombieOnLane>();
	//	}

	//	//-> RELOADING
	//	{
	//		auto transition = pIdle->CreateTransition(State::Reloading);

	//		transition->AddCondition<PlantCondition_FullAmmo>(false);
	//		transition->AddCondition<PlantCondition_ZombieOnLane>(false);
	//	}
	//}

	////SHOOTING
	//{
	//	Action<Plant>* pShooting = mpStateMachine->CreateAction<PlantAction_Shooting>(State::Shooting);

	//	//-> IDLE
	//	{
	//		auto transition = pShooting->CreateTransition(State::Idle);

	//		transition->AddCondition<PlantCondition_ZombieOnLane>(false);
	//	}

	//	//-> RELOADING
	//	{
	//		auto transition = pShooting->CreateTransition(State::Reloading);

	//		transition->AddCondition<PlantCondition_NoAmmo>();
	//	}
	//}

	////RELOADING
	//{
	//	Action<Plant>* pShooting = mpStateMachine->CreateAction<PlantAction_Reloading>(State::Reloading);
	//}

	//mpStateMachine->SetState(State::Idle);
}



void RugbyMan::OnUpdate()
{
	//const sf::Vector2f& position = GetPosition();
	//const char* stateName = GetStateName((Plant::State)mpStateMachine->GetCurrentState());

	//std::string ammo = std::to_string(mAmmo) + "/" + std::to_string(mMaxAmmo);

	//Debug::DrawText(position.x, position.y - 50, stateName, 0.5f, 0.5f, sf::Color::Red);
	//Debug::DrawText(position.x, position.y, ammo, 0.5f, 0.5f, sf::Color::Blue);

	//mpStateMachine->Update();
}

void RugbyMan::Shoot()
{
	// Tirer la ball en direction du joueur le plus proche
	//std::vector<Zombie*> zombies = GetScene<RugbyScene>()->GetZombies();

	std::vector<RugbyMan*> rugbyMen = GetScene<RugbyScene>()->GetRugbyMen();

	RugbyMan* closestRugbyMan = rugbyMen[0];

	Ball* ball = GetScene<RugbyScene>()->GetBall();

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		//si le rugbyMan est different de this
		if (rugbyMan != this)
		{
			//calculer la distance entre this et rugbyMan			
			float distance = std::sqrt(std::pow(rugbyMan->GetPosition().x - GetPosition().x, 2) + std::pow(rugbyMan->GetPosition().y - GetPosition().y, 2));
			float distance2 = std::sqrt(std::pow(closestRugbyMan->GetPosition().x - GetPosition().x, 2) + std::pow(closestRugbyMan->GetPosition().y - GetPosition().y, 2));
			//si la distance est inferieur a la distance minimale
			if (distance < distance2 && rugbyMan->mTeam == ball->mOwner->mTeam)
			{
				closestRugbyMan = rugbyMan;
			}			
		}
	}

	// Appeler OnShoot de la ball avec la direction du joueur le plus proche
	ball->OnShoot(closestRugbyMan->GetPosition());
}

bool RugbyMan::IsMouseOver(const sf::Vector2i& mousePos) const
{
	sf::Vector2f pos = GetPosition();
	float distance = std::sqrt(std::pow(mousePos.x - pos.x, 2) + std::pow(mousePos.y - pos.y, 2));
	return distance <= GetRadius();
}

