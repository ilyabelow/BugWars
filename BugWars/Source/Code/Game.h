#pragma once
#include "GameBase/GameBase.h"
#include "Globals.h"
#include "Bug.h"

static float max_size = 9600.f; // bug->GetRadius() * 3 * 100
static float cell_size = 960.f/2.f; //somewhat optimal
static float lattice_sizef = std::ceil(max_size / cell_size);
static int lattice_size = static_cast<int>(lattice_sizef);
static int lattice_size2 = lattice_size* lattice_size;

struct Tank;

struct SearchStatus {
	BugBase* bug = nullptr;
	float min_dist = std::numeric_limits<float>::max();
};

struct Game : public GameBase
{
	Game();

	virtual void OnUpdate(float dt) override;
	virtual void OnRender() const override;
	virtual void AddObject(GameObject* object) override;

	virtual void OnBugsSpawned() override;

	// Owning containers
	std::vector<GameObject*> objects;


	// Not owning! no need to clean
	std::vector<std::vector<Bug*>> lattice;
	std::vector<int> latticeMask;

	std::vector<Bug*>& GetCell(int i, int j);

	bool ShouldMoveBug(Bug* bug);
	void RemoveBug(Bug* bug);
	void AddBug(Bug* bug);

	void CheckMask(int i, int j);
	bool IsCellChecked(int i, int j);
	void ClearMask();

	BugBase* FindNearest(Point pos, const std::function<bool(BugBase*)>& checker = [](BugBase* bug) {return true; });
	bool FindNearestInCell(Point pos, float r2, int cell_i, int cell_j, SearchStatus& st, const std::function<bool(BugBase*)>& checker);

	~Game() override;
};