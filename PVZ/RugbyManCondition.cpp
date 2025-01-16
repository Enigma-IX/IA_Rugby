#include "RugbyManCondition.h"

#include "RugbyScene.h"

bool RugbymanCondition_HasTheBall::OnTest(RugbyMan* pRugbyman)
{
	RugbyScene* pScene = pRugbyman->GetScene<RugbyScene>();

	bool condition = pScene->mBall->mOwner == pRugbyman;

	return condition;
}

bool RugbymanCondition_TeamHasBall::OnTest(RugbyMan* pRugbyman)
{
	RugbyScene* pScene = pRugbyman->GetScene<RugbyScene>();

	if (pScene->mBall->mOwner == nullptr)
	{
		return false;
	}

	bool condition = pScene->mBall->mOwner->mTeam == pRugbyman->mTeam && pScene->mBall->mOwner != pRugbyman;

	return condition;
}

bool RugbymanCondition_TeamHasNoBall::OnTest(RugbyMan* pRugbyman)
{
	RugbyScene* pScene = pRugbyman->GetScene<RugbyScene>();

	if (pScene->mBall->mOwner == nullptr)
	{
		return true;
	}

	bool condition = pScene->mBall->mOwner->mTeam != pRugbyman->mTeam;

	return condition;
}
