#pragma once
#include "tile.hpp"
#include "grid.hpp"


namespace Game {
	
	class Enemy {

		private:

		double moveTimer = 0.0;
		double moveDelay = 0.3;
		double stageTimer = 0.0;
		double stageTimeLimit = 15.0;

		Tile* selectedTile = nullptr;
		Tile* lastSelectedTile = nullptr;
		Tile* king = nullptr;


		std::vector<Hex> pathToPlr;
		int i = 0;

		int stage = 0;
		bool generatedPath = false;

		public:

		Enemy(Tile* kingTile);
		void Update();


	};

}
