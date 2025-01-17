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

	FindClosestRugbyMan(pRugbyMan);

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


	if (pRugbyMan->mTimeSinceLastShot < 2.f)
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
				RugbyMan* closestRugbyMan = FindClosestRugbyMan(pRugbyMan);

				if (closestRugbyMan == nullptr)
					return;
				
				pRugbyMan->Shoot(closestRugbyMan);
			}
		}
	}
}


RugbyMan* RugbyManAction_HasBall::FindClosestRugbyMan(RugbyMan* pRugbyMan)
{
	RugbyMan* closestRugbyMan = nullptr;

	Ball* ball = pRugbyMan->GetScene<RugbyScene>()->GetBall();

	std::vector<RugbyMan*> rugbyMen = pRugbyMan->GetScene<RugbyScene>()->GetRugbyMen();
	std::vector<RugbyMan*> closestRugbyMen;

	for (RugbyMan* rugbyMan : rugbyMen)
	{
		if (ball->mOwner == nullptr)
			return nullptr;

		if (rugbyMan != pRugbyMan && rugbyMan->mTeam == ball->mOwner->mTeam)
		{
			if (pRugbyMan->mTeam == 0)
			{
				if (rugbyMan->GetPosition().x > pRugbyMan->GetPosition().x)
					continue;
			}
			else
			{
				if (rugbyMan->GetPosition().x < pRugbyMan->GetPosition().x)
					continue;
			}

			float distance = std::sqrt(
				std::pow(rugbyMan->GetPosition().x - pRugbyMan->GetPosition().x, 2) +
				std::pow(rugbyMan->GetPosition().y - pRugbyMan->GetPosition().y, 2)
			);

			if (rugbyMan == ball->mPreviousOwner)
				continue;

			// Ajoutez le joueur actuel à la liste
			closestRugbyMen.push_back(rugbyMan);
		}
	}

	// Triez les joueurs par distance croissante
	std::sort(closestRugbyMen.begin(), closestRugbyMen.end(), [pRugbyMan](RugbyMan* a, RugbyMan* b) {
		float distanceA = std::sqrt(
			std::pow(a->GetPosition().x - pRugbyMan->GetPosition().x, 2) +
			std::pow(a->GetPosition().y - pRugbyMan->GetPosition().y, 2)
		);
		float distanceB = std::sqrt(
			std::pow(b->GetPosition().x - pRugbyMan->GetPosition().x, 2) +
			std::pow(b->GetPosition().y - pRugbyMan->GetPosition().y, 2)
		);
		return distanceA < distanceB;
		});

	// Retenez uniquement les 3 plus proches joueurs (si plus de 3 joueurs sont trouvés)
	if (closestRugbyMen.size() > 3)
		closestRugbyMen.resize(3);



	// Parcourez tous les joueurs pour trouver celui qui est le moins entouré
	RugbyMan* leastSurroundedRugbyMan = nullptr;
	int minOpponentsCount = std::numeric_limits<int>::max(); // Initialisé à une valeur très élevée
	float maxAverageDistance = -std::numeric_limits<float>::max(); // Initialisé à une valeur très basse

	for (RugbyMan* candidate : closestRugbyMen)
	{
		Debug::DrawLine(pRugbyMan->GetPosition().x, pRugbyMan->GetPosition().y, candidate->GetPosition().x, candidate->GetPosition().y, sf::Color::White);

		int opponentsCount = 0;
		float totalDistance = 0.0f;

		// Parcourez tous les rugbymen pour trouver ceux qui ne sont pas de la même équipe
		for (RugbyMan* otherRugbyMan : rugbyMen)
		{
			if (otherRugbyMan->mTeam != candidate->mTeam)
			{
				float distance = std::sqrt(
					std::pow(otherRugbyMan->GetPosition().x - candidate->GetPosition().x, 2) +
					std::pow(otherRugbyMan->GetPosition().y - candidate->GetPosition().y, 2)
				);

				// Si un adversaire est dans un rayon de 150, augmentez le compteur
				if (distance <= 50.0f)
				{
					opponentsCount++;
					totalDistance += distance; // Ajoutez la distance au total
				}
			}
		}

		// Si ce joueur a moins d'adversaires autour, mettez à jour
		if (opponentsCount < minOpponentsCount)
		{
			minOpponentsCount = opponentsCount;
			maxAverageDistance = (opponentsCount > 0) ? totalDistance / opponentsCount : 0.0f;
			leastSurroundedRugbyMan = candidate;
		}
		// Si le nombre d'adversaires est égal, vérifiez la distance moyenne
		else if (opponentsCount == minOpponentsCount)
		{
			float averageDistance = (opponentsCount > 0) ? totalDistance / opponentsCount : 0.0f;
			if (averageDistance > maxAverageDistance)
			{
				maxAverageDistance = averageDistance;
				leastSurroundedRugbyMan = candidate;
			}
		}
	}

	if (leastSurroundedRugbyMan == nullptr)
		return nullptr;

	Debug::DrawLine(pRugbyMan->GetPosition().x, pRugbyMan->GetPosition().y, leastSurroundedRugbyMan->GetPosition().x, leastSurroundedRugbyMan->GetPosition().y, sf::Color::Magenta);
	return leastSurroundedRugbyMan;
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
