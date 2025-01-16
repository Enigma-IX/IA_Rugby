#pragma once

#include "Condition.h"
#include "RugbyMan.h"

class RugbymanCondition_HasTheBall : public Condition<RugbyMan>
{
public:
	bool OnTest(RugbyMan* owner) override;
};

class RugbymanCondition_TeamHasBall : public Condition<RugbyMan>
{
public:
	bool OnTest(RugbyMan* owner) override;
};

class RugbymanCondition_TeamHasNoBall : public Condition<RugbyMan>
{
public:
	bool OnTest(RugbyMan* owner) override;
};

