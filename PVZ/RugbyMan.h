#pragma once

#include "Entity.h"
#include <SFML/Graphics/Color.hpp>



template<typename T>
class StateMachine;

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
	bool GoToPosition(int x, int y, float speed = -1.f) override;

	void OnCollision(Entity* pCollidedWith) override;

	const char* GetStateName(State state) const;

	sf::Vector2f GetInitialPosition() const { return *mInitialPosition; }
	void SetInitialPosition(float x, float y) {	mInitialPosition = new sf::Vector2f(x,y); }

	void Shoot(RugbyMan* target);

	int mAreaIndex;
	int mTeam;

	float mTimeSinceLastShot = 0.f;

	int mfollowerOffset;
	float mbaseSpeed;

protected:
	void OnInitialize() override;
	void OnUpdate() override;

	friend class RugbyManAction_HasBall;
	friend class RugbyManAction_TeamHasBall;
	friend class RugbyManAction_EnemyHasBall;

	friend class RugbymanCondition_HasTheBall;
	friend class RugbymanCondition_TeamHasBall;
	friend class RugbymanCondition_TeamHasNoBall;

private:

	void ClampPosition();
	sf::Vector2f* mInitialPosition;


};
