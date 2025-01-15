#include "RugbyMan.h"

#include <string>
#include <algorithm>

#include "Utils.h"

#include "RugbyScene.h"
#include "Projectile.h"

#include "StateMachine.h"

#include "Debug.h"

void RugbyMan::OnInitialize()
{

	mpStateMachine = new StateMachine<RugbyMan>(this, State::Count);

	mAreaIndex = -1;

	//SetTag(RugbyMan::Tag::PLANT);

	//IDLE
	{
		Action<RugbyMan>* pIdle = mpStateMachine->CreateAction<PlantAction_Idle>(State::Idle);

		//-> SHOOTING
		{
			auto transition = pIdle->CreateTransition(State::Shooting);

			auto condition = transition->AddCondition<PlantCondition_ZombieOnLane>();
		}

		//-> RELOADING
		{
			auto transition = pIdle->CreateTransition(State::Reloading);

			transition->AddCondition<PlantCondition_FullAmmo>(false);
			transition->AddCondition<PlantCondition_ZombieOnLane>(false);
		}
	}

	//SHOOTING
	{
		Action<RugbyMan>* pShooting = mpStateMachine->CreateAction<PlantAction_Shooting>(State::Shooting);

		//-> IDLE
		{
			auto transition = pShooting->CreateTransition(State::Idle);

			transition->AddCondition<PlantCondition_ZombieOnLane>(false);
		}

		//-> RELOADING
		{
			auto transition = pShooting->CreateTransition(State::Reloading);

			transition->AddCondition<PlantCondition_NoAmmo>();
		}
	}

	//RELOADING
	{
		Action<Plant>* pShooting = mpStateMachine->CreateAction<PlantAction_Reloading>(State::Reloading);
	}

	mpStateMachine->SetState(State::Idle);
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

// void RugbyMan::Shoot()
// {
// 	std::vector<RugbyMan*> rugbyMen = GetScene<RugbyScene>()->GetRugbyMen();
// 
// 	RugbyMan* closestRugbyMan = rugbyMen[0];
// 
// 	Ball* ball = GetScene<RugbyScene>()->GetBall();
// 
// 	for (RugbyMan* rugbyMan : rugbyMen)
// 	{
// 		//si le rugbyMan est different de this
// 		if (rugbyMan != this)
// 		{
// 			//calculer la distance entre this et rugbyMan			
// 			float distance = std::sqrt(std::pow(rugbyMan->GetPosition().x - GetPosition().x, 2) + std::pow(rugbyMan->GetPosition().y - GetPosition().y, 2));
// 			float distance2 = std::sqrt(std::pow(closestRugbyMan->GetPosition().x - GetPosition().x, 2) + std::pow(closestRugbyMan->GetPosition().y - GetPosition().y, 2));
// 			//si la distance est inferieur a la distance minimale
// 			if (distance < distance2 && rugbyMan->mTeam == ball->mOwner->mTeam)
// 			{
// 				closestRugbyMan = rugbyMan;
// 			}			
// 		}
// 	}
// 
// 	// Appeler OnShoot de la ball avec la direction du joueur le plus proche
// 	ball->OnShoot(closestRugbyMan->GetPosition());
// }

void RugbyMan::Shoot()
{
	std::vector<RugbyMan*> rugbyMen = GetScene<RugbyScene>()->GetRugbyMen();

	if (rugbyMen.empty()) {
		return; // Si aucun joueur, on sort de la fonction
	}

	RugbyMan* closestRugbyMan = nullptr;
	Ball* ball = GetScene<RugbyScene>()->GetBall();

	float minDistance = std::numeric_limits<float>::max();

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		// Si le rugbyMan est différent de this
		if (rugbyMan != this && rugbyMan->mTeam == ball->mOwner->mTeam)
		{
			// Calculer la distance
			float distance = std::sqrt(
				std::pow(rugbyMan->GetPosition().x - GetPosition().x, 2) +
				std::pow(rugbyMan->GetPosition().y - GetPosition().y, 2)
			);

			// Si la distance est inférieure à la distance minimale
			if (distance < minDistance)
			{
				minDistance = distance;
				closestRugbyMan = rugbyMan;
			}
		}
	}

	// Vérifier si un joueur proche a été trouvé
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

