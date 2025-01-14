#include "RugbyScene.h"
#include "RugbyMan.h"

#include "Debug.h"

#define PLAN_COUNT 3

void RugbyScene::OnInitialize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();

	float rugbyManRadius = height * 0.04f;

	const std::pair<float, float> positions[] = {
		{3/12.f, 6/12.f},
		{2/12.f, 2/12.f},
		{2/12.f, 10/12.f},
		{1/12.f, 1/12.f},
		{1/12.f, 11/12.f}
	};

	constexpr size_t numPositions = sizeof(positions) / sizeof(positions[0]);
	RugbyMan* pRugbyMan[numPositions];

	for (size_t i = 0; i < numPositions; ++i) {
		float x = positions[i].first;
		float y = positions[i].second;
		pRugbyMan[i] = CreateEntity<RugbyMan>(rugbyManRadius, sf::Color::Green);
		pRugbyMan[i]->SetPosition(x*width, y*height);

		//pRugbyMan[i]->SetAreaIndex(i);
	}
}

void RugbyScene::OnUpdate()
{
	for (int i = 0; i < PLAN_COUNT; i++)
	{
		const AABB& aabb = mAreas[i];

		Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, sf::Color::Red);
	}
}

int RugbyScene::GetClickedArea(int x, int y) const
{
	for (int i = 0; i < PLAN_COUNT; i++)
	{
		const AABB* aabb = &mAreas[i];

		if (x >= aabb->xMin && x <= aabb->xMax && y >= aabb->yMin && y <= aabb->yMax)
			return i;
	}

	return -1;
}

void RugbyScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	int index = GetClickedArea(event.mouseButton.x, event.mouseButton.y);

	if (index == -1)
		return;

	const AABB* clickedArea = &mAreas[index];

	int y = clickedArea->yMin + (clickedArea->yMax - clickedArea->yMin) / 2;

}


