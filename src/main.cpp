#include "raylib.h"
#include "hex.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h> 
#endif

#include <stdio.h> 

#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif



const int screenWidth = 720;
const int screenHeight = 720;

RenderTexture2D target = { 0 };


Game::Hex hex1(0, 0, 0);
Game::Hex hex2(1, 0, -1);
Game::Hex hex3(2, 0, -2);
Game::Hex hex4(2, -1, -1);

void UpdateDrawFrame(void);


int main(void) {

	#if !defined(_DEBUG)
    	SetTraceLogLevel(LOG_NONE); 
	#endif

    InitWindow(screenWidth, screenHeight, "raylib gamejam template");
    
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

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

    BeginTextureMode(target);
        ClearBackground(WHITE);

		Game::DrawHexagon(hex1, BLACK);
		Game::DrawHexagon(hex2, GREEN);
		Game::DrawHexagon(hex3, BLUE);
		Game::DrawHexagon(hex4, RED);
        
    EndTextureMode();
    

    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

    EndDrawing();

}
