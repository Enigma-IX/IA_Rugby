#pragma once

#include "Entity.h"
#include <SFML/Graphics/Color.hpp>


#include "StateMachine.h"
#include "RugbyManCondition.h"
#include "RugbyManAction.h"

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
	void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f) override;

	void OnCollision(Entity* pCollidedWith) override;

	const char* GetStateName(State state) const;

	void Shoot();

	int mAreaIndex;
	int mTeam;

protected:
	void OnInitialize() override;
	void OnUpdate() override;

private:

	void ClampPosition();

};
