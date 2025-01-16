#include "RugbyManAction.h"
#include "RugbyScene.h"

void RugbyManAction_HasBall::OnStart(RugbyMan* pRugbyMan)
{
	RugbyScene* pRugbyScene = pRugbyMan->GetScene<RugbyScene>();

	//AABB* goal = pRugbyMan->mGoal;

	// rugby man go to position
	//pRugbyMan->GoToPosition(goal->xMax/2, pRugbyMan->GetPosition().y, 100.f);
}

