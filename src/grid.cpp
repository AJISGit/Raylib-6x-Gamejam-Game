#include "grid.hpp"
#include <functional>


Game::TileGrid Game::grid;


Game::TileGrid::~TileGrid() {
	for (std::pair<Hex, Tile*> key : tiles) {
		delete key.second;
	}
}


Game::Tile& Game::TileGrid::GetTile(Game::Hex pos) const {
	return *tiles.at(pos);
}


Game::Tile& Game::TileGrid::AddTile(Game::Hex pos, std::uint16_t troops) {
	tiles[pos] = new Game::Tile(pos, troops);
	return GetTile(pos);
}



std::unordered_map<Game::Hex, Game::Tile*> Game::TileGrid::GetTiles() const {

	std::unordered_map<Game::Hex, Game::Tile*> result;
	for (std::pair<Hex, Tile*> key : tiles) {
		result[key.first] = key.second;
	}

	return result;

}

