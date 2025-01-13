#include "raylib.h"

int main(){
	InitWindow(800, 600, "TEST APP");

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();
}
