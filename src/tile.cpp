#include "tile.hpp"
#include <cmath>


Game::Tile::Tile(Game::Hex position, std::uint16_t troops = 1) {

	SetPosition(position);
	SetTroops(troops);

}


Game::Hex Game::Tile::GetPosition() const {
	return position;
}


void Game::Tile::SetPosition(Game::Hex pos) {
	position = pos;
}


std::uint16_t Game::Tile::GetTroops() const {
	return troops;
}


void Game::Tile::SetTroops(std::uint16_t troops) {
	this->troops = troops;
}


void Game::Tile::AddTroop() {
	++troops;
}


Game::TileType Game::Tile::GetType() const {
	return type;
}


void Game::Tile::SetType(Game::TileType type) {
	this->type = type;
}


Color Game::Tile::GetColor() const {

	unsigned char i = 1;

	switch (type) {
		case Game::TileType::Empty:
			return { 200, 200, 200, 255 };
		case Game::TileType::Player:
			return BLUE;
		case Game::TileType::Enemy:
			return RED;

	}

}


void Game::DrawTile(const Game::Tile& tile) {

	Game::DrawHexagon(tile.GetPosition(), BLACK, 1.0f);
	Game::DrawHexagon(tile.GetPosition(), tile.GetColor(), 0.98f);

}

