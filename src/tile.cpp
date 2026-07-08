#include "tile.hpp"
#include "textures.hpp"
#include <cmath>
#include <string>


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
	lastTroopAdd = GetTime();
}


void Game::Tile::RemoveTroop() {
	--troops;
}


void Game::Tile::AddTroops(std::uint16_t troops) {
	this->troops += troops;
}


void Game::Tile::RemoveTroops(std::uint16_t troops) {
	this->troops -= troops;
}


Game::TileType Game::Tile::GetType() const {
	return type;
}


void Game::Tile::SetType(Game::TileType type) {
	this->type = type;
}



Game::TileLand Game::Tile::GetLandType() const {
	return landType;
}


void Game::Tile::SetLandType(Game::TileLand land) {
	landType = land;
}


std::optional<Texture2D> Game::Tile::GetTexure() const {

	switch (landType) {

		case Game::TileLand::Blank:
			return std::nullopt;
		case Game::TileLand::King:
			return Game::Textures::King;

	}

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


void Game::Tile::Update() {

	switch (landType) {

		case Game::TileLand::Blank:
			if (type == Game::TileType::Empty) { return; }

			if (GetTime() - lastTroopAdd > 10.0) {
				AddTroop();
			}
			return;

		case Game::TileLand::King:
			if (GetTime() - lastTroopAdd > 1.0) {
				AddTroop();
			}
			return;

	}

}


void Game::DrawTile(const Game::Tile& tile) {

	Vector2 pixelPos = tile.GetPosition().ToPixel();

	// Draw the hexagon
	Game::DrawHexagon(tile.GetPosition(), BLACK, 1.0f);
	Game::DrawHexagon(tile.GetPosition(), tile.GetColor(), 0.98f);

	// Draw the image (if any)
	std::optional<Texture2D> decal = tile.GetTexure();
	if (decal.has_value()) {
		DrawTexture(decal.value(), pixelPos.x - 32, pixelPos.y - 32, WHITE);
	}

	// Draw the troop count
	if (tile.GetTroops() == 0) { return; }

	std::string troopText = std::to_string(tile.GetTroops());
	int textSize = 15;

	DrawText(troopText.c_str(), pixelPos.x - MeasureText(troopText.c_str(), textSize) + 3, pixelPos.y - 5.0f, textSize, WHITE);

}

