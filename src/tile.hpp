#pragma once
#include "hex.hpp"
#include <cstdint>
#include <optional>


namespace Game {

	enum class TileType { Player, Enemy, Empty };
	enum class TileLand { King, City, Blank };
	
	class Tile {
		
		private:

		Hex position = { 0, 0, 0 };
		std::uint16_t troops = 0;
		std::uint16_t strength = 1;
		TileType type = TileType::Empty;
		TileLand landType = TileLand::Blank;

		double lastTroopAdd = 0.0f;

		public:

		Tile(Hex position, std::uint16_t troops);
		Hex GetPosition() const;
		void SetPosition(Hex pos);

		std::uint16_t GetTroops() const;
		void SetTroops(std::uint16_t troops);
		void AddTroop();
		void RemoveTroop();
		void AddTroops(std::uint16_t troops);
		void RemoveTroops(std::uint16_t troops);

		std::uint16_t GetStrength() const;
		void SetStrength(std::uint16_t strength);


		TileType GetType() const;
		void SetType(TileType type);

		TileLand GetLandType() const;
		void SetLandType(TileLand land);

		std::optional<Texture2D> GetTexure() const;
		Color GetColor() const;

		void Update();


	};

	void DrawTile(const Tile& tile);
	void MoveTroops_(Tile& from, Tile& to, std::uint16_t troops);
	void MoveTroops(Tile& from, Tile& to, std::int16_t troops);

}
