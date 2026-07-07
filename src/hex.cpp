#include <stdexcept>
#include <cmath>
#include "hex.hpp"


Game::Hex::Hex(int q, int r, int s) {
	SetCoords(q, r, s);
}


int Game::Hex::GetQ() const {
	return q;
}


int Game::Hex::GetR() const {
	return r;
}


int Game::Hex::GetS() const {
	return s;
}


void Game::Hex::SetCoords(int q, int r, int s) {

	if (q + r + s != 0) {
		throw std::runtime_error("q + r + s is not 0");
	}

	this->q = q;
	this->r = r;
	this->s = s;

}




bool Game::Hex::operator==(Game::Hex a) {
	return (a.q == q && a.r == r && a.s == s);
}


bool Game::Hex::operator!=(Game::Hex a) {
	return !(a == *this); 
}


Game::Hex Game::Hex::operator+(Game::Hex a) {
	return Game::Hex(q + a.q, r + a.r, s + a.s);
}


Game::Hex Game::Hex::operator-(Game::Hex a) {
	return Game::Hex(q - a.q, r - a.r, s - a.s);
}


Game::Hex Game::Hex::operator*(Game::Hex a) {
	return Game::Hex(q * a.q, r * a.r, s * a.s);
}


Vector2 Game::Hex::ToPixel() const {

	float x = (3.0f / 2.0f) * q;
	float y = std::sqrt(3) / 2 * q + std::sqrt(3) * r;

	x *= Game::HEX_SIZE;
	y *= Game::HEX_SIZE;

	return { x, y };

}


Game::Hex Game::PixelToHex(Vector2 pixel) {

	float q = std::sqrt(3.0f) / 3.0f * pixel.x + -1.0f/3.0f * pixel.y;
    float r = 2.0f/3.0f * pixel.y;
    return Game::AxialToCube({q, r});

}


Game::Hex Game::AxialToCube(Vector2 axial) {

	return Game::Hex(axial.x, axial.y, -axial.x - axial.y);

}



void Game::DrawHexagon(Game::Hex hex, Color color) {

	DrawPoly(hex.ToPixel(), 6, Game::HEX_SIZE, 0, color);

}

