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


Game::Hex hex1(1, -1, 0);
Game::Hex hex2(0, 1, -1);
Game::Hex hex3 = hex1 + hex2;

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
        ClearBackground(RAYWHITE);

		DrawText(TextFormat("%d, %d, %d", hex3.GetQ(), hex3.GetR(), hex3.GetS()), 300, 350, 15, BLACK);
        
    EndTextureMode();
    

    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

    EndDrawing();

}
