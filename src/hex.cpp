#include <stdexcept>
#include <cmath>
#include "hex.hpp"
#include <iostream>


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




bool Game::Hex::operator==(Game::Hex a) const {
	return (a.q == q && a.r == r && a.s == s);
}


bool Game::Hex::operator!=(Game::Hex a) const {
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
	x += Game::HEX_ORIGIN_X;
	y += Game::HEX_ORIGIN_Y;

	return { x, y };

}


Game::Hex Game::PixelToHex(Vector2 pixel) {

	float x = pixel.x - HEX_ORIGIN_X;
	float y = pixel.y - HEX_ORIGIN_Y;

    // invert the scaling
    x = pixel.x / HEX_SIZE;
    y = pixel.y / HEX_SIZE;
    // cartesian to hex
    float q = ((2.0f/3.0f) * x);
    float r = (((-1.0f/3.0f) * x) + (std::sqrt(3.0f)/3) * y);


	std::cout << "Pixel: ( " << pixel.x << ", " << pixel.y << " )\n";
	std::cout << "Hex: ( " << q << ", " << r << ", " << -q - r << " )\n";

	Game::Hex result = Game::AxialToCube(Game::AxialRound({ q, r }));

    return result;

}


Game::Hex Game::AxialToCube(Vector2 axial) {

	return Game::Hex(axial.x, axial.y, -axial.x - axial.y);

}


Vector2 Game::CubeToAxial(Game::Hex cube) {
	return { static_cast<float>(cube.GetQ()), static_cast<float>(cube.GetR()) };
}


Game::Hex Game::CubeRound(float q, float r, float s) {
	
	float roundQ = std::round(q);
	float roundR = std::round(r);
	float roundS = std::round(s);
	float qDiff = std::fabs(roundQ - q);
	float rDiff = std::fabs(roundR - r);
	float sDiff = std::fabs(roundS - s);

	if ((qDiff > rDiff) && (qDiff > sDiff)) {
        roundQ = -r-s;
    } else if (rDiff > sDiff) {
        roundR = -q-s;
	} else {
        roundS = -q-r;
	}

	return Game::Hex(roundQ, roundR, roundS);

}


Vector2 Game::AxialRound(Vector2 axial) {

	Game::Hex cube = Game::CubeRound(axial.x, axial.y, -axial.x - axial.y);
	return CubeToAxial(cube);

}


void Game::DrawHexagon(Game::Hex hex, Color color, float scale = 1.0f) {

	DrawPoly(hex.ToPixel(), 6, Game::HEX_SIZE * scale, 0, color);

}

