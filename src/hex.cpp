#include <stdexcept>
#include <cmath>
#include "hex.hpp"
#include <iostream>


std::vector<Game::Hex> HexDirectionVectors = {
	{ 1, 0, -1 }, { 1, -1, 0 }, { 0, -1, 1 },
	{ -1, 0, 1 }, { -1, 1, 0 }, { 0, 1, -1 }, 
};


int Game::RoundFloat(float number) {

	int wholePart = static_cast<int>(number);
	float decimalPart = std::fabs(number - wholePart);
	int result = std::abs(wholePart);
	
	bool roundUp = false;
	bool negative = false;

	std::cout << "Whole: " << wholePart << " Decimal: " << decimalPart << '\n';

	if (decimalPart >= 0.5f) {
		roundUp = true;
	} else {
		roundUp = false;
	}

	if (wholePart >= 0) {
		negative = false;
	} else {
		negative = true;
	}

	if (roundUp) {
		result = result + 1;
	}

	if (negative) {
		result = -result;
	}

	return result;


}


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
	

    long long rq = llround(q), rr = llround(r), rs = llround(s);
    double dq = fabs(rq - q), dr = fabs(rr - r), ds = fabs(rs - s);
    if (dq > dr && dq > ds) rq = -rr - rs;
    else if (dr > ds)       rr = -rq - rs;
    else                    rs = -rq - rr;
    return {(int)rq, (int)rr, (int)rs};

}


Vector2 Game::AxialRound(Vector2 axial) {

	Game::Hex cube = Game::CubeRound(axial.x, axial.y, -axial.x - axial.y);
	return CubeToAxial(cube);

}


std::vector<Game::Hex> Game::GetHexNeighbors(Game::Hex hex) {

	std::vector<Game::Hex> results;

	int i = 0;
	for (Game::Hex dir : HexDirectionVectors) {
		results.push_back(hex + HexDirectionVectors[i++]);
	}

	return results;

}


float Game::HexDistance(Game::Hex a, Game::Hex b) {
	Game::Hex c = a - b;
	return (std::abs(c.GetQ()) + std::abs(c.GetR()) + std::abs(c.GetS())) / 2.0f;
}


Game::Hex Game::HexLerp(Game::Hex a, Game::Hex b, float t) {

	float q = Lerp(a.GetQ(), b.GetQ(), t);
	float r = Lerp(a.GetR(), b.GetR(), t);
	float s = Lerp(a.GetS(), b.GetS(), t);

	std::cout << q << ' ' << r << ' ' << s << '\n';

	return CubeRound(q, r, s);
}


std::vector<Game::Hex> Game::HexLinedraw(Game::Hex a, Game::Hex b) {

	float distance = HexDistance(a, b);
	std::vector<Game::Hex> result;
	double step = 1.0 / std::max(static_cast<int>(distance), 1);
	for (int i = 0; i <= distance; i++) {
		result.push_back(HexLerp(a, b, step * i));
	}

	return result;

}


void Game::DrawHexagon(Game::Hex hex, Color color, float scale = 1.0f) {

	DrawPoly(hex.ToPixel(), 6, Game::HEX_SIZE * scale, 0, color);

}

