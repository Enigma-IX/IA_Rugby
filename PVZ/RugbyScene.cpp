#include "RugbyScene.h"

#include "Debug.h"

#define PLAN_COUNT 3

#include <array>
#include <numeric>

void RugbyScene::OnInitialize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();

	float rugbyManRadius = height * 0.04f;

	constexpr std::array<std::pair<float, float>, 5> positions = {
	std::make_pair(3 / 12.f, 6 / 12.f),
	std::make_pair(2 / 12.f, 2 / 12.f),
	std::make_pair(2 / 12.f, 10 / 12.f),
	std::make_pair(1 / 12.f, 1 / 12.f),
	std::make_pair(1 / 12.f, 11 / 12.f)
	};

	mAreas[0] = { 0, height / 4, width, 3 * height / 4 };
	mAreas[1] = { 0, 0, width, height / 2 };
	mAreas[2] = { 0, height / 2, width, height };

	constexpr size_t numPositions = positions.size();
	std::array<size_t, numPositions> indices;
	std::iota(indices.begin(), indices.end(), 0);

	std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
		return positions[a].second < positions[b].second;
		});

	auto createRugbyMan = [&](float x, float y, int team, const sf::Color& color, int areaIndex) {
		RugbyMan* rugbyMan = CreateEntity<RugbyMan>(rugbyManRadius, color);
		rugbyMan->SetTeam(team);
		rugbyMan->SetTag(RugbyScene::Tag::RUGBYMAN);
		rugbyMan->SetAreaIndex(areaIndex);
		rugbyMan->SetPosition(x * width, y * height);
		mRugbyMen.push_back(rugbyMan);
		return rugbyMan;
		};

	for (size_t i = 0; i < numPositions; ++i) {
		size_t index = indices[i];
		float x = positions[index].first;
		float y = positions[index].second;
		int areaIndex = (i == 2) ? 0 : (i < 2 ? 1 : 2);

		// Create Team1 rugbymen
		createRugbyMan(x, y, 1, sf::Color::Green, areaIndex);

		// Create Team2 rugbymen
		createRugbyMan(1.0f - x, y, 2, sf::Color::Red, areaIndex);
	}

	mBall = CreateEntity<Ball>(height * 0.02f, sf::Color::White);
	mBall->SetPosition(width / 2, height / 2);
}

void RugbyScene::OnUpdate()
{
	for (int i = 0; i < PLAN_COUNT; i++)
	{
		const AABB& aabb = mAreas[i];

		Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, sf::Color::Red);
	}

	const AABB& aabb = mAreas[0];
	Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, sf::Color::Yellow);

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
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Space) {
			if (mBall->mOwner == nullptr)
				return;
			mBall->mOwner->Shoot();
		}
	}

}


