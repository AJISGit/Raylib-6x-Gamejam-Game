#pragma once
#include "raylib.h"
#include "raymath.h"
#include <unordered_set>
#include <vector>

namespace Game {


	class Hex;

	constexpr float HEX_SIZE = 50.0f;
	constexpr float HEX_ORIGIN_X = 360.0f;
	constexpr float HEX_ORIGIN_Y = 360.0f;
	
	int RoundFloat(float number);

	class Hex {

		private:
		int q = 0;
		int r = 0;
		int s = 0;

		public:
		Hex(int q, int r, int s);

		int GetQ() const;
		int GetR() const;
		int GetS() const;

		void SetCoords(int q, int r, int s);

		bool operator==(Hex a) const;
		bool operator!=(Hex a) const;
		Hex operator+(Hex a);
		Hex operator-(Hex a);
		Hex operator*(Hex a);

		Vector2 ToPixel() const;

	};



	Hex CubeRound(float q, float r, float s);
	Vector2 AxialRound(Vector2 axial);

	Hex PixelToHex(Vector2 pixel);
	Hex AxialToCube(Vector2 axial);
	Vector2 CubeToAxial(Hex cube);

	std::vector<Hex> GetHexNeighbors(Hex hex);
	float HexDistance(Hex a, Hex b);

	Hex HexLerp(Hex a, Hex b, float t);
	std::vector<Hex> HexLinedraw(Hex a, Hex b); 

	void DrawHexagon(Hex hex, Color color, float scale);

}


namespace std {
	template <> struct hash<Game::Hex> {
		size_t operator()(const Game::Hex& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.GetQ());
			size_t hr = int_hash(h.GetR());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
			
		}
	};
}

