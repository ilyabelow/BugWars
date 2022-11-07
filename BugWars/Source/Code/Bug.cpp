#include "pch.h"
#include "Bug.h"
#include "Game.h"
#include "GameBase/Framework.h"
#include "GameBase/TankBase.h"

IMPLEMENT_RTTI(Bug);

void Bug::OnUpdate(float dt)
{
	if (g_Game->framework) { // false it tests
		Point ul = g_Game->tank->position - g_Game->framework->screenSize / 2;
		Point dr = ul + g_Game->framework->screenSize;
		visible = position.x + GetSize().x / 2 > ul.x &&
				  position.y + GetSize().y / 2 > ul.y &&
				  position.x - GetSize().x / 2 < dr.x &&
				  position.y - GetSize().y / 2 < dr.y;
	}
	if (g_Game->ShouldMoveBug(this)) {
		g_Game->RemoveBug(this);
		g_Game->AddBug(this);
	}
}


BugBase* Bug::FindBugToEat() const
{
	return g_Game->FindNearest(position, 
		[this](BugBase* bug) {
			return bug != this && bug->id < id;
		}
	);
}

void Bug::OnEat(BugBase& first, BugBase& second)
{
	if (first.id > second.id)
	{ // this eats other
		reinterpret_cast<Bug*>(&second)->Die();
	}
	else
	{ // other eats this
		Die();
	}
}

void Bug::Die()
{
	g_Game->RemoveBug(this);
	disabled = true;
	visible = false;
}
