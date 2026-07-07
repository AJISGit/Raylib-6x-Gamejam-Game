#include "raylib.h"
#include "raymath.h"
#include "grid.hpp"

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

const float cameraSpeed = 400.0f;

RenderTexture2D target = { 0 };
Camera2D camera = { 0 };


void UpdateDrawFrame(void);


int main(void) {


	// Generate Map
	int N = 10;
	for (int q = -N; q <= N; q++) {
    	int r1 = std::max(-N, -q - N);
	    int r2 = std::min( N, -q + N);
	    for (int r = r1; r <= r2; r++) {
			Game::grid.insert(Game::Hex(q, r, -q-r));
   		}
	}


	#if !defined(_DEBUG)
    	SetTraceLogLevel(LOG_NONE); 
	#endif

    InitWindow(screenWidth, screenHeight, "raylib gamejam template");

	target = LoadRenderTexture(720, 720);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    
	camera.target = { 0.0f, 0.0f };
	camera.zoom = 1.0f;

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
	#else
    	SetTargetFPS(60);
	    while (!WindowShouldClose()) {
	        UpdateDrawFrame();
	    }
	#endif


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



	BeginTextureMode(target);

		ClearBackground(RAYWHITE);
 		BeginMode2D(camera);
			unsigned char i = 0;
			for (const Game::Hex& hexagon : Game::grid) {
				Color color = {0, 0, 0, 0};

				if (hexagon == Game::Hex(0, 0, 0)) {
					color = BLUE;
				} else {
					color = { i, i, i, 255 };
				}

				Game::DrawHexagon(hexagon, color);
				i++;
			}
		EndMode2D();       

	EndTextureMode();

    BeginDrawing();
		
		ClearBackground(WHITE);
		DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);


    EndDrawing();

}
