#include "pch.h"
#include "Game.h"


BugBase* Game::FindNearest(Point pos, const std::function<bool(BugBase*)>& checker)
{
	Point pos_as_cell = pos / cell_size;
	float r = 0.7f;
	// circle bounding box
	int min_i = static_cast<int>(std::max(pos_as_cell.x - r, 0.f));
	int max_i = static_cast<int>(std::min(pos_as_cell.x + r, lattice_sizef - 1.f));
	int min_j = static_cast<int>(std::max(pos_as_cell.y - r, 0.f));
	int max_j = static_cast<int>(std::min(pos_as_cell.y + r, lattice_sizef - 1.f));
	r *= cell_size;
	SearchStatus st;
	ClearMask();
	float max_r = max_size * 1.5f;
	while (!st.bug && r < max_r)
	{
		float r2 = r * r;
		for (int i = min_i; i <= max_i; i++)
		{
			for (int j = min_j; j <= max_j; j++)
			{
				if (IsCellChecked(i, j))
				{ // discard cells where we already checked everything
					continue;
				}
				if (FindNearestInCell(pos, r2, i, j, st, checker))
				{
					CheckMask(i, j);
				}
			}
		}
		min_i = std::max(min_i - 1, 0);
		min_j = std::max(min_j - 1, 0);
		max_i = std::min(max_i + 1, lattice_size - 1);
		max_j = std::min(max_j + 1, lattice_size - 1);
		r += cell_size;
	}
	return st.bug;
}

bool Game::FindNearestInCell(Point pos, float r2, int cell_i, int cell_j, SearchStatus& st, const std::function<bool(BugBase*)>& checker)
{
	int to_check = static_cast<int>(GetCell(cell_i, cell_j).size());
	for (auto bug: GetCell(cell_i, cell_j)) {
		if (!checker(bug))
		{
			to_check--;
			continue;
		}
		auto cur_dist = bug->position.Distance2(pos);
		if (cur_dist > r2)
		{
			continue; // check it later
		}
		if (cur_dist > st.min_dist)
		{
			to_check--;
			continue;
		}
		st.min_dist = cur_dist;
		st.bug = bug;
	}
	return to_check == 0; // If all objects in bucket were inside the circle, don't check the cell again
}


void Game::CheckMask(int i, int j)
{
	latticeMask[lattice_size * i + j] = 1;
}

void Game::ClearMask()
{
	std::fill(latticeMask.begin(), latticeMask.end(), 0);
}

bool Game::IsCellChecked(int i, int j)
{
	return latticeMask[lattice_size * i + j];
}
