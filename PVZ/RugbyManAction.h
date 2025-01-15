#pragma once

#include "Action.h"
#include "RugbyMan.h"
class RugbyManAction : public Action<RugbyMan>
	{
	public:
		void OnStart(RugbyMan* pRugbyMan) override {}
		void OnUpdate(RugbyMan* pRugbyMan) override {}
		void OnEnd(RugbyMan* pRugbyMan) override {}
	};

	class RugbyManAction_Shooting : public Action<RugbyMan>
	{

	public:
		void OnStart(RugbyMan* pRugbyMan) override;
		void OnUpdate(RugbyMan* pRugbyMan) override;
		void OnEnd(RugbyMan* pRugbyMan) override {};
	};

	class RugbyManAction_Running : public Action<RugbyMan>
	{
		float speed = 0.5f;

	public:
		void OnStart(RugbyMan* pRugbyMan) override;
		void OnUpdate(RugbyMan* pRugbyMan) override;
		void OnEnd(RugbyMan* pRugbyMan) override {};
	};


