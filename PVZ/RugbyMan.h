#pragma once

#include "Entity.h"
#include <SFML/Graphics/Color.hpp>

class RugbyMan : public Entity
{
	int mLane = -1;

public:
	void SetLane(int lane) { mLane = lane; }
	void SetAreaIndex(int index) { mAreaIndex = index; }
	bool IsMouseOver(const sf::Vector2i& mousePos) const;


	int mAreaIndex;

protected:
	void OnInitialize() override;
	void OnUpdate() override;

};
