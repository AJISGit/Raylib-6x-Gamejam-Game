#pragma once
#include "hex.hpp"
#include "tile.hpp"
#include <unordered_map>
#include <functional>


namespace Game {


	class TileGrid final {

		private:
		std::unordered_map<Hex, Tile*> tiles;

		public:
		~TileGrid();

		Tile& GetTile(Hex pos) const;
		Tile& AddTile(Hex pos, std::uint16_t troops);

		std::unordered_map<Hex, Tile*> GetTiles() const;

	};

	extern TileGrid grid;

}
