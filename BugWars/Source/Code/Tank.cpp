#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

BugBase* Tank::GetBugToShoot() const
{
	return g_Game->FindNearest(position);
}

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	// I spent a lot of time on the exact formula, but couldn't make it work =(
	// so I came up with an iterative approach that is fast and accurate enough
	Point C = target_pos;
	float t = C.Distance(position) / bullet_vel;
	for (int i = 0; i < 4; i++) {
		C = target_pos + target_dir * target_vel * t;
		t = C.Distance(position) / bullet_vel;
	}
	return C - position;
}
