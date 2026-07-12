#include "raylib.h"
#include "raymath.h"
#include "grid.hpp"
#include "timers.hpp"
#include "textures.hpp"
#include "enemy.hpp"
#include <ctime>

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h> 
#endif

#include <stdio.h> 
#include <iostream>

#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
	#define LOG(...) printf(__VA_ARGS__)
#else
	#define LOG(...)
#endif



const int screenWidth = 720;
const int screenHeight = 720;



const float renderingLimitLeft = -50.0f;
const float renderingLimitRight = 770.0f;

const float renderingLimitUp = -50.0f;
const float renderingLimitDown = 770.0f;



const float cameraSpeed = 400.0f;

RenderTexture2D target = { 0 };
Camera2D camera = { 0 };



bool showFps = false;
bool playerAlive = true;

Game::Tile* selectedTile = nullptr;
Game::Tile* lastSelectedTile = nullptr;

Game::Enemy enemy(nullptr);


enum class ScreenState { Logo, Game };
ScreenState currentScreen = ScreenState::Logo;


void UpdateDrawFrame(void);
void GameUpdateDraw();


int main(int argc, char** argv) {

	if (argc > 1) {
		showFps = true;
	}

	SetRandomSeed(std::time(NULL));

	// Generate Map
	
	int cityChance = 20;
	int N = 25;
	for (int q = -N; q <= N; q++) {
    	int r1 = std::max(-N, -q - N);
	    int r2 = std::min( N, -q + N);
	    for (int r = r1; r <= r2; r++) {
			Game::Tile& tile = Game::grid.AddTile({ q, r, -q-r }, 0);
			if (tile.GetPosition() == Game::Hex( 0, 0, 0 )) {
				tile.SetType(Game::TileType::Player);
				tile.SetLandType(Game::TileLand::King);
				tile.SetTroops(2);
			} else {
				
				if (GetRandomValue(1, cityChance) == 1) {
					tile.SetLandType(Game::TileLand::City);
					tile.SetTroops(GetRandomValue(30, 50));
					tile.SetType(Game::TileType::Empty);


					if (GetRandomValue(1, 3) >= 2) {
						cityChance = std::floor(static_cast<float>(cityChance) / 1.1f);
					} else {
						cityChance = std::ceil(static_cast<float>(cityChance) * 1.5f);
					}


				} else {

					tile.SetType(Game::TileType::Empty);
					tile.SetLandType(Game::TileLand::Blank);
					tile.SetStrength(0);
				}
			}
   		}
}
	

	// Spawn enemy king
	int enemyQ;
	int enemyR;
	int enemyS;
	int enemyPosLimit = 10;
	while (true) {
		enemyQ = GetRandomValue(-enemyPosLimit, enemyPosLimit);
		enemyR = GetRandomValue(-enemyPosLimit, enemyPosLimit);
		enemyS = GetRandomValue(-enemyPosLimit, enemyPosLimit);
		if (enemyQ + enemyR + enemyS != 0) {
			continue;
		} else {
			break;
		}
	}

	Game::Tile& enemyTile = Game::grid.GetTile({ enemyQ, enemyR, enemyS });
	enemyTile.SetType(Game::TileType::Enemy);
	enemyTile.SetLandType(Game::TileLand::King);

	enemy = Game::Enemy(&enemyTile);


	// Set up selection
	selectedTile = &Game::grid.GetTile({ 0, 0, 0 });


	#if !defined(_DEBUG)
		SetTraceLogLevel(LOG_NONE); 
	#endif

	InitWindow(screenWidth, screenHeight, "raylib gamejam template");

	// Initialize Stuff
	target = LoadRenderTexture(720, 720);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

	Game::Textures::King = LoadTexture("resources/king.png");
	Game::Textures::City = LoadTexture("resources/city.png");

	camera.target = { 0.0f, 0.0f };
	camera.zoom = 1.0f;



	// Do Things
	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
	#else
		if (!showFps) {
			SetTargetFPS(60);
		}
		while (!WindowShouldClose()) {
			UpdateDrawFrame();
		}
	#endif


	// Bye Bye!
	UnloadRenderTexture(target);
	CloseWindow();
	return 0;
}


void GameUpdateDraw(void) {

	if (Game::grid.GetTile({ 0, 0, 0 }).GetType() != Game::TileType::Player) {
		playerAlive = false;
	}
	// I don't care that people hate gotos. Those people are boring.
	if (!playerAlive) { goto afterTheThing; }



	if (IsKeyDown(KEY_W)) {
		camera.target.y -= cameraSpeed * GetFrameTime();
	}
	if (IsKeyDown(KEY_A)) {
		camera.target.x -= cameraSpeed * GetFrameTime();
	}
	if (IsKeyDown(KEY_S)) {
		camera.target.y += cameraSpeed * GetFrameTime();
	}
	if (IsKeyDown(KEY_D)) {
		camera.target.x += cameraSpeed * GetFrameTime();
	}

	// x 1500 y 750

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / camera.zoom);
		camera.target = Vector2Add(camera.target, delta);

	}


	if (camera.target.x < -1500) {
		camera.target.x = -1500;
	} else if (camera.target.x > 1500) {
		camera.target.x = 1500;
	}

	if (camera.target.y < -750) {
		camera.target.y = -750;
	} else if (camera.target.y > 750) {
		camera.target.y = 750;
	}


	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {



		lastSelectedTile = selectedTile;

		for (std::pair<const Game::Hex, Game::Tile*> key : Game::grid.GetTiles()) {

			Vector2 pixelPos = key.first.ToPixel();

			pixelPos = GetWorldToScreen2D(pixelPos, camera);				
			if (
				(pixelPos.x < renderingLimitLeft) || (pixelPos.x > renderingLimitRight) ||
				(pixelPos.y < renderingLimitUp) || (pixelPos.y > renderingLimitDown)
			) {
				continue;
			}

			if (!CheckCollisionPointCircle(GetMousePosition(), pixelPos, Game::HEX_SIZE)) { continue; }
			selectedTile = key.second;
			break;

		}

		Game::MoveTroops(*lastSelectedTile, *selectedTile, 0, Game::TileType::Player);
	}



	for (std::pair<const Game::Hex, Game::Tile*> key : Game::grid.GetTiles()) {
		if (!playerAlive) { break; }
		key.second->Update();
	}

	// Timers
	if (GetTime() - Game::lastTroopAdd >= Game::troopAddDelay) {
		Game::lastTroopAdd = GetTime();
	}
	if (GetTime() - Game::lastCityAdd >= Game::cityAddDelay) {
		Game::lastCityAdd = GetTime();
	}
	
	enemy.Update();

	

	afterTheThing:


	BeginTextureMode(target);

		ClearBackground(BLACK);
 		BeginMode2D(camera);
			for (std::pair<const Game::Hex, Game::Tile*> key : Game::grid.GetTiles()) {

				Vector2 pixelPos = key.first.ToPixel();
				bool render = true;


				pixelPos = GetWorldToScreen2D(pixelPos, camera);				
				if (
					(pixelPos.x < renderingLimitLeft) || (pixelPos.x > renderingLimitRight) ||
					(pixelPos.y < renderingLimitUp) || (pixelPos.y > renderingLimitDown)
				) {
					render = false;
				}

				if (render) {
					Game::DrawTile(*key.second);
					if (key.second == selectedTile && playerAlive) {
						Game::DrawHexagon(key.second->GetPosition(), { 255, 255, 255, 100 }, 1.0f);
					}
				}

			}
		EndMode2D();       

		if (!playerAlive) {
			DrawRectangle(0, 0, 720, 720, { 0, 0, 0, 200 });
			int textSize = 64;
			DrawText("Game Over", 360 - MeasureText("Game Over", textSize) / 2, 350, textSize, WHITE);
		}



	EndTextureMode();

    BeginDrawing();
		
		ClearBackground(WHITE);
		DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);
		if (showFps) {
			DrawFPS(10, 10);
		}


    EndDrawing();

}


void UpdateDrawFrame() {
	GameUpdateDraw();
}

