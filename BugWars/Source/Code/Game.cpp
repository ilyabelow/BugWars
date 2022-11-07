#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"

Game* g_Game;

template <typename T>
T* fixBuggyNew(T* obj) {
	obj->disabled = false;
	return obj;
}

Game::Game()
	: GameBase({ [] {return fixBuggyNew(new Tank); },
				 [] {return fixBuggyNew(new Bug); },
				 [] {return fixBuggyNew(new Bullet); } })
{
	g_Game = this;
	lattice.resize(lattice_size2);
	for (int i = 0; i < lattice_size2; i++)
	{
		lattice[i].reserve(1198 / (lattice_size* lattice_size) * 2);
	}
	latticeMask.resize(lattice_size2);
	std::fill(latticeMask.begin(), latticeMask.end(), 0);
}

void Game::OnUpdate(float dt)
{
	// PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	for (uint i = 0; i < objects.size(); i++)
	{
		while (i < objects.size() && objects[i]->disabled)
		{
			delete objects[i];
			objects[i] = objects[objects.size() - 1];
			objects.pop_back();
		}
		if (i < objects.size()) {
			objects[i]->Update(dt);
		}
	}
	for (int i = 0; i < lattice_size2; i++) {
		for (int k = 0; k < lattice[i].size(); k++) {
			if (lattice[i][k]->disabled) {
				int x = 5;
			}
		}
	}
}


void Game::OnRender() const
{
	for (auto obj : objects) {
		if (obj->visible) {
			DoRender(obj);
		}
	}
}

void Game::AddObject(GameObject* object)
{
	objects.push_back(object);
	if (object->GetRTTI() == Bug::s_RTTI) {
		AddBug(reinterpret_cast<Bug*>(object));
	}
}

void Game::OnBugsSpawned()
{
	if (objects[1]->id == 1) // the oldest bug does not move, this messes up dir vector of the next generation bug
	{
		objects[1]->position.x += 0.1f;
	}
}

Game::~Game() 
{
	for (uint i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
}