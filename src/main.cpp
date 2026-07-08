#include "raylib.h"
#include "raymath.h"
#include "grid.hpp"
#include "textures.hpp"

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


void UpdateDrawFrame(void);


int main(int argc, char** argv) {

	if (argc > 1) {
		showFps = true;
	}

	// Generate Map
	int N = 50;
	for (int q = -N; q <= N; q++) {
    	int r1 = std::max(-N, -q - N);
	    int r2 = std::min( N, -q + N);
	    for (int r = r1; r <= r2; r++) {
			Game::Tile& tile = Game::grid.AddTile({ q, r, -q-r }, 1);
			if (tile.GetPosition() == Game::Hex( 0, 0, 0 )) {
				tile.SetType(Game::TileType::Player);
				tile.SetLandType(Game::TileLand::King);
			} else {
				tile.SetType(Game::TileType::Empty);
			}
   		}
	}


	#if !defined(_DEBUG)
    	SetTraceLogLevel(LOG_NONE); 
	#endif

    InitWindow(screenWidth, screenHeight, "raylib gamejam template");

	// Initialize Stuff
	target = LoadRenderTexture(720, 720);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

	Game::Textures::King = LoadTexture("resources/king.png");

    
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
	UnloadTexture(Game::Textures::King);
	UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}


void UpdateDrawFrame(void) {


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

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / camera.zoom);
		camera.target = Vector2Add(camera.target, delta);

	}


	for (std::pair<const Game::Hex, Game::Tile*> key : Game::grid.GetTiles()) {
		key.second->Update();
	}


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
				}

			}
		EndMode2D();       

	EndTextureMode();

    BeginDrawing();
		
		ClearBackground(WHITE);
		DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);
		if (showFps) {
			DrawFPS(10, 10);
		}


    EndDrawing();

}
