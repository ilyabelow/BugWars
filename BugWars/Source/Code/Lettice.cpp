#include "pch.h"
#include "Game.h"

std::vector<Bug*>& Game::GetCell(int i, int j) {
	return lattice[i * lattice_size + j];
}

bool Game::ShouldMoveBug(Bug* bug)
{
	Point ul = { bug->i * cell_size,  bug->j * cell_size };
	Point dr = ul + Point(1, 1) * cell_size;
	return bug->position.x < ul.x || bug->position.y < ul.y || bug->position.x > dr.x || bug->position.y > dr.y;
}

void Game::RemoveBug(Bug* bug)
{
	auto& cell = GetCell(bug->i, bug->j);
	cell[bug->k] = cell[cell.size() - 1];
	cell[bug->k]->k = bug->k;
	cell.pop_back();
}

void Game::AddBug(Bug* bug)
{
	int cell_i = static_cast<int>(std::clamp(bug->position.x, 0.f, max_size) / cell_size);
	int cell_j = static_cast<int>(std::clamp(bug->position.y, 0.f, max_size) / cell_size);
	bug->i = cell_i;
	bug->j = cell_j;
	auto& cell = GetCell(bug->i, bug->j);
	bug->k = static_cast<int>(cell.size());
	cell.push_back(bug);
}
