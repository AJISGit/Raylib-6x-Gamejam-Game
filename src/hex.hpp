#pragma once

namespace Game {


	class Hex;

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

	};

}

