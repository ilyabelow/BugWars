#pragma once
#include "GameBase/BugBase.h"

struct Bug : public BugBase
{
	DEFINE_RTTI;

	virtual void OnUpdate(float dt) override;
	virtual BugBase* FindBugToEat() const override;
	virtual void OnEat(BugBase& first, BugBase& second) override;

	void Die();

	// Grid position
	int i = -1;
	int j = -1;
	int k = -1;
};