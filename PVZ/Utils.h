#pragma once

#include <SFML/System/Vector2.hpp>

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);
	float RandomFloat(float min, float max);
}


template<typename T>
constexpr const T& Clamp(const T& value, const T& min, const T& max) {
	return (value < min) ? min : (value > max ? max : value);
}
