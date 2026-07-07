#pragma once
#include "raylib.h"

namespace Game {


	class Hex;

	constexpr float HEX_SIZE = 50.0f;

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

		bool operator==(Hex a);
		bool operator!=(Hex a);
		Hex operator+(Hex a);
		Hex operator-(Hex a);
		Hex operator*(Hex a);

		Vector2 ToPixel() const;

	};

	Hex PixelToHex(Vector2 pixel);
	Hex AxialToCube(Vector2 axial);

	void DrawHexagon(Hex hex, Color color);

}

