#pragma once

#include "Entity.h"
#include <SFML/Graphics/Color.hpp>

class RugbyMan : public Entity
{

	StateMachine<RugbyMan>* mpStateMachine;

	enum State
	{
		HavingTheBall,
		TeamMateWithBall,
		EnemyWithBall,

		Count
	};

	int mLane = -1;

public:
	void SetLane(int lane) { mLane = lane; }
	void SetAreaIndex(int index) { mAreaIndex = index; }
	void SetTeam(int team) { mTeam = team; }
	bool IsMouseOver(const sf::Vector2i& mousePos) const;

	void Shoot();

	int mAreaIndex;
	int mTeam;

protected:
	void OnInitialize() override;
	void OnUpdate() override;

private:


};
