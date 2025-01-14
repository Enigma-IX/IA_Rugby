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

	size_t indices[numPositions];
	for (size_t i = 0; i < numPositions; ++i)
		indices[i] = i;

	// Sort indices based on the y-coordinate of positions
	std::sort(indices, indices + numPositions, [&](size_t a, size_t b) {
		return positions[a].second < positions[b].second;
		});

	// Assign entities to areas based on sorted indices
	for (size_t i = 0; i < numPositions; ++i) {
		size_t index = indices[i];
		float x = positions[index].first;
		float y = positions[index].second;

		// Create entity and set its position
		pRugbyMan[index] = CreateEntity<RugbyMan>(rugbyManRadius, sf::Color::Green);
		pRugbyMan[index]->SetPosition(x * width, y * height);

		// Assign area based on sorted position
		if (i == 2) {
			pRugbyMan[index]->SetAreaIndex(0); // Area A (middle entity)
		}
		else if (i < 2) {
			pRugbyMan[index]->SetAreaIndex(1); // Area B (top entities)
		}
		else {
			pRugbyMan[index]->SetAreaIndex(2); // Area C (bottom entities)
		}
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


