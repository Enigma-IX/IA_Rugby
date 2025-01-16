#pragma once

#include "GameManager.h"
#include "RugbyMan.h"
#include "Ball.h"


#include <list>

namespace sf
{
	class Event;
}

struct AABB
{
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

class RugbyScene : public Scene
{
public:
	enum Tag
	{
		RUGBYMAN,
		BALL,
		GOALLINE
	};

	int mLaneRugbyCount[4] = { 0, 0, 0 ,0 };
	AABB mAreas[3];
	AABB mGoals[2];

	std::vector<RugbyMan*> mRugbyMen;
	Ball* mBall;

private:
	int GetClickedArea(int x, int y) const;

	RugbyMan* mSelectedPlayer = nullptr;

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	std::vector<RugbyMan*> GetRugbyMen() const { return mRugbyMen; };
	Ball* GetBall() const { return mBall; };

	AABB* GetArea(int index) { return &mAreas[index]; };
	AABB* GetGoal(int index) { return &mGoals[index]; };

	void Reset();
};

