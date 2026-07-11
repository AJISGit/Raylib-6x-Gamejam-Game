#include "enemy.hpp"
#include <iostream>



Game::Enemy::Enemy(Game::Tile* kingTile) {

	if (kingTile == nullptr) { return; }

	king = kingTile;

	selectedTile = king;

}


void Game::Enemy::Update() {

//	if (GetTime() < 5) { return; }

	if (GetTime() - moveTimer < moveDelay) {
		return;
	}
	moveTimer = GetTime();

	if (king->GetType() != Game::TileType::Enemy) {
		return;
	}


	lastSelectedTile = selectedTile;


	if (stage == 4) {
		stageTimeLimit = 30.0;
	} else {
		stageTimeLimit = 15.0;
	}


	if (!generatedPath) {
		if (stage == 0) {
			lastSelectedTile = king;
			pathToPlr = Game::GetHexNeighbors(king->GetPosition());
		} else if (stage == 1) {
			int q;
			int r;
			int s;
			int posLimit = 10;
			while (true) {
				q = GetRandomValue(-posLimit, posLimit);
				r = GetRandomValue(-posLimit, posLimit);
				s = GetRandomValue(-posLimit, posLimit);
				if (q + r + s != 0) {
					continue;
				} else {
					break;
				}
			}
			Game::Hex hex = { q, r, s };
			lastSelectedTile = king;
			pathToPlr = Game::HexLinedraw(king->GetPosition(), hex);
		} else if (stage == 2) {
			lastSelectedTile = &Game::grid.GetTile(king->GetPosition());
			pathToPlr = Game::GetHexNeighbors(king->GetPosition());
			pathToPlr.push_back(king->GetPosition());
		} else if (stage == 3) {
			int index = GetRandomValue(0, ownedHexes.size() - 1);
			Tile& tile = Game::grid.GetTile(ownedHexes[index]);
			lastSelectedTile = &tile;
			pathToPlr = Game::HexLinedraw(tile.GetPosition(), { 0, 0, 0 });
		} else if (stage == 4) {
			lastSelectedTile = king;
			pathToPlr = Game::HexLinedraw(king->GetPosition(), { 0, 0, 0 });
		} else if (stage == 5) {

			int index = GetRandomValue(0, ownedHexes.size() - 1);
			Tile& tile = Game::grid.GetTile(ownedHexes[index]);
			lastSelectedTile = &tile;
			pathToPlr = Game::GetHexNeighbors(tile.GetPosition());

		} else if (stage > 5 && stage < 7) {
			int index = GetRandomValue(0, ownedHexes.size() - 1);
			Tile& tile = Game::grid.GetTile(ownedHexes[index]);
			lastSelectedTile = &tile;
			pathToPlr = Game::HexLinedraw(tile.GetPosition(), king->GetPosition());
		}

		generatedPath = true;
	}




	
	if (lastSelectedTile->GetTroops() < 2) {
		i = 0;
		if (stage == 4) {
			lastSelectedTile = king;
			selectedTile = &Game::grid.GetTile(pathToPlr[0]);
		} else {
			generatedPath = false;
		}
		return;
	}
	
	if ((i > (pathToPlr.size() - 1)) || (GetTime() - stageTimer >= stageTimeLimit)) {
		int randValue = GetRandomValue(1, 10);
		if (randValue <= 4) {
			stage--;
		} else if (randValue > 7) {
			stage = 4;
		} else {
			stage++;
		}

		if (stage < 0) {
			stage = 7;
		} else if (stage > 7) {
			stage = 0;
		}

		std::cout << "Stage: " << stage << '\n';

		generatedPath = false;
		stageTimer = GetTime();
		i = 0;
		return;
	}
	selectedTile = &Game::grid.GetTile(pathToPlr[i]);

	Game::MoveTroops(*lastSelectedTile, *selectedTile, 0, Game::TileType::Enemy);
	ownedHexes.push_back(selectedTile->GetPosition());
	i++;

}
