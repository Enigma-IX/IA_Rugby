#include "RugbyScene.h"

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

	std::sort(indices, indices + numPositions, [&](size_t a, size_t b) {
		return positions[a].second < positions[b].second;
		});

	for (size_t i = 0; i < numPositions; ++i) {
		size_t index = indices[i];
		float x = positions[index].first;
		float y = positions[index].second;

		pRugbyMan[index] = CreateEntity<RugbyMan>(rugbyManRadius, sf::Color::Green);
		pRugbyMan[index]->SetPosition(x * width, y * height);
		mRugbyMen.push_back(pRugbyMan[index]);

		if (i == 2) {
			pRugbyMan[index]->SetAreaIndex(0); // Area A (middle entity)
			mAreas[0] = { 0, 1 * height / 4, width, 3 * height / 4 };
		}
		else if (i < 2) {
			pRugbyMan[index]->SetAreaIndex(1); // Area B (top entities)
			mAreas[1] = { 0, 0, width, height / 2 };
		}
		else {
			pRugbyMan[index]->SetAreaIndex(2); // Area C (bottom entities)
			mAreas[2] = { 0, height / 2 , width, height };
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
	/*if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	int index = GetClickedArea(event.mouseButton.x, event.mouseButton.y);

	if (index == -1)
		return;

	const AABB* clickedArea = &mAreas[index];

	int y = clickedArea->yMin + (clickedArea->yMax - clickedArea->yMin) / 2;*/

	if (event.type == sf::Event::MouseButtonPressed) {
		for (RugbyMan* player : mRugbyMen) {
			sf::Vector2f position = player->GetPosition();
			float dx = position.x - event.mouseButton.x;
			float dy = position.y - event.mouseButton.y;

			if (std::sqrt(dx * dx + dy * dy) <= player->GetRadius()) {
				mSelectedPlayer = player;
				break;
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		mSelectedPlayer = nullptr;
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (mSelectedPlayer != nullptr) {
			mSelectedPlayer->SetPosition(event.mouseMove.x, event.mouseMove.y);
		}
	}

}


