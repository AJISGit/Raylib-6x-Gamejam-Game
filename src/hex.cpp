#include <stdexcept>
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



