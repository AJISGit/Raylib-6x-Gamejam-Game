#pragma once
#include "hex.hpp"
#include <cstdint>


namespace Game {

	enum class TileType { Player, Enemy, Empty };
	
	class Tile {
		
		private:

		Hex position = { 0, 0, 0 };
		std::uint16_t troops = 0;
		TileType type = TileType::Empty;

		public:

		Tile(Hex position, std::uint16_t troops);
		Hex GetPosition() const;
		void SetPosition(Hex pos);

		std::uint16_t GetTroops() const;
		void SetTroops(std::uint16_t troops);
		void AddTroop();

		TileType GetType() const;
		void SetType(TileType type);
		Color GetColor() const;


	};

	void DrawTile(const Tile& tile);

}
