#pragma once

#include "GameManager.h"

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
		PLAYER,
		BALL
	};

	int mLaneRugbyCount[4] = { 0, 0, 0 ,0 };
	AABB mAreas[4];

private:
	int GetClickedArea(int x, int y) const;

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

};

