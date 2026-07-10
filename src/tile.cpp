#include "tile.hpp"
#include "textures.hpp"
#include "timers.hpp"
#include <cmath>
#include <string>
#include <iostream>


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
	if (type == Game::TileType::Empty) { return; }
	if (landType != Game::TileLand::Blank) {
		this->lastTroopAdd = GetTime();
	}
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


std::uint16_t Game::Tile::GetStrength() const {
	return strength;
}


void Game::Tile::SetStrength(std::uint16_t strength) {
	this->strength = strength;
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
			SetStrength(GetTroops() - 1);

			if (GetTime() - Game::lastTroopAdd >= troopAddDelay) {
				AddTroop();
			}
			return;

		case Game::TileLand::King:
			if (GetTime() - lastTroopAdd > 1.0) {
				AddTroop();
			}
			SetStrength(GetTroops());
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
	if ((tile.GetTroops() <= 1) && (tile.GetType() == Game::TileType::Empty)) { return; }

	std::string troopText = std::to_string(tile.GetTroops());
	int textSize = 15;

	DrawText(troopText.c_str(), pixelPos.x - MeasureText(troopText.c_str(), textSize) + 3, pixelPos.y - 5.0f, textSize, WHITE);

}


void Game::MoveTroops_(Game::Tile& from, Game::Tile &to, std::uint16_t troops = 0) {

	if (from.GetTroops() < 2) { return; }

	
	if ((from.GetType() != to.GetType()) && ((from.GetTroops() + 1) <= to.GetStrength())) { return; }


	std::uint16_t troopAmount = troops;
	if (troops == 0) {
		troopAmount = from.GetTroops() - 1;
	}


	from.RemoveTroops(troopAmount);


	if (from.GetType() != to.GetType()) {
		to.AddTroops(troopAmount - to.GetStrength());
	} else {
		to.AddTroops(troopAmount);
	}

	to.SetType(Game::TileType::Player);

}


void Game::MoveTroops(Game::Tile& from, Game::Tile &to, std::int16_t troops = 0) {

	Game::Hex fromPos = from.GetPosition();
	Game::Hex toPos = to.GetPosition();
	int q, r, s;
	q = fromPos.GetQ() - toPos.GetQ();
	r = fromPos.GetR() - toPos.GetR();
	s = fromPos.GetS() - toPos.GetS();

	if ((std::abs(q) > 1) || (std::abs(r) > 1) || (std::abs(s) > 1)) {
		return;
	}
	Game::MoveTroops_(from, to, troops);

}
