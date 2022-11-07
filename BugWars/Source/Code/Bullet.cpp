#include "pch.h"
#include "Bullet.h"
#include "Game.h"
#include "Bug.h"
#include "Tank.h"

IMPLEMENT_RTTI(Bullet);

void Bullet::OnStart(Point)
{
}

void Bullet::OnUpdate(float dt)
{
	int cell_i = static_cast<int>(std::clamp(position.x, 0.f, max_size) / cell_size);
	int cell_j = static_cast<int>(std::clamp(position.y, 0.f, max_size) / cell_size);
	auto& cell = g_Game->GetCell(cell_i, cell_j);
	for (int i = 0; i < cell.size(); i++)
	{
		if (!cell[i]->disabled && cell[i]->position.Distance(position) < 32.f)
		{
			g_Game->tank->score++;
			cell[i]->Die();
			OnLifeEnd();
			return;
		}
	}
}

void Bullet::OnLifeEnd()
{
	disabled = true;
	visible = false;
}
