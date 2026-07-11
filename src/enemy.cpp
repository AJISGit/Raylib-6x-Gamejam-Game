#include "enemy.hpp"
#include <iostream>



Game::Enemy::Enemy(Game::Tile* kingTile) {

	if (kingTile == nullptr) { return; }

	king = kingTile;

	selectedTile = king;

}


void Game::Enemy::Update() {

	if (GetTime() < 5) { return; }

	if (GetTime() - moveTimer < moveDelay) {
		return;
	}
	moveTimer = GetTime();
	moveDelay = GetRandomValue(1, 5) / 10.0f;

	if (king->GetType() != Game::TileType::Enemy) {
		return;
	}


	lastSelectedTile = selectedTile;


	if (!generatedPath) {
		if (stage == 0) {
			pathToPlr = Game::GetHexNeighbors(king->GetPosition());
		} else if (stage == 1) {
			Game::Hex hex = Game::GetHexNeighbors(king->GetPosition())[GetRandomValue(1, 3)];
			lastSelectedTile = &Game::grid.GetTile(hex);
			pathToPlr = Game::GetHexNeighbors(hex);
		} else if (stage == 2) {
			lastSelectedTile = &Game::grid.GetTile(king->GetPosition());
			pathToPlr = Game::GetHexNeighbors(king->GetPosition());
			pathToPlr.push_back(king->GetPosition());
		} else if (stage == 3) {
			lastSelectedTile = king;
			pathToPlr = Game::HexLinedraw(king->GetPosition(), { 0, 0, 0 });
		}
		generatedPath = true;
	}

	
	if (lastSelectedTile->GetTroops() < 2) {
		lastSelectedTile = king;
		selectedTile = &Game::grid.GetTile(pathToPlr[0]);
		i = 0;
		return;
	}
	
	if (i > (pathToPlr.size() - 1)) {
		++stage;
		generatedPath = false;
		i = 0;
		return;
	}
	selectedTile = &Game::grid.GetTile(pathToPlr[i]);

	Game::MoveTroops(*lastSelectedTile, *selectedTile, 0);
	i++;

}
