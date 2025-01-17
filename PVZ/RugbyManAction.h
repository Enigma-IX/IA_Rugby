#pragma once

#include "Action.h"
#include "RugbyMan.h"

class RugbyManAction_HasBall : public Action<RugbyMan>
{
public:
	void OnStart(RugbyMan* pRugbyMan) override;
	void OnUpdate(RugbyMan* pRugbyMan) override;
	void OnEnd(RugbyMan* pRugbyMan) override {};

private:
	RugbyMan* FindClosestRugbyMan(RugbyMan* pRugbyMan);
};

class RugbyManAction_TeamHasBall : public Action<RugbyMan>
{
public:
	void OnStart(RugbyMan* pRugbyMan) override {};
	void OnUpdate(RugbyMan* pRugbyMan) override;
	void OnEnd(RugbyMan* pRugbyMan) override {};
};

class RugbyManAction_EnemyHasBall : public Action<RugbyMan>
{
public:
	void OnStart(RugbyMan* pRugbyMan) override {};
	void OnUpdate(RugbyMan* pRugbyMan) override;
	void OnEnd(RugbyMan* pRugbyMan) override {};
};


