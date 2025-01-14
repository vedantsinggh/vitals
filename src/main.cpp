#include "raylib.h"
#include <cstdlib>
#include <stdlib.h>

#define MAX_DATA_POINTS 2000
#define DISPLAY_POINT 40

void DrawGraph(float *data, int dataCount, Rectangle graphArea) {
	DrawRectangleLinesEx(graphArea, 2, RED);
	int start = 0;
    if (dataCount < 2) return; 
	if (dataCount > DISPLAY_POINT) start = dataCount - DISPLAY_POINT;

	float minValue = 0;
	float maxValue = 120;
    float range = maxValue - minValue;
    if (range == 0) range = 1;

    float stepX = graphArea.width / (dataCount - start - 1);

    for (int i = start; i < dataCount - 1; i++) {
        float x1 = graphArea.x + (i - start) * stepX;
        float y1 = graphArea.y + graphArea.height - ((data[i] - minValue) / range) * graphArea.height;



        float x2 = graphArea.x + (i - start + 1) * stepX;
        float y2 = graphArea.y + graphArea.height - ((data[i + 1] - minValue) / range) * graphArea.height;

		Vector2 startPoint = {x1, y1};
		Vector2 endPoint = {x2, y2};

		DrawLineBezier(startPoint, endPoint, 4.0f,Color{ 33, 104, 166, (x1 - graphArea.x)/(float)(graphArea.width) * 255 + 25});
        //DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 2, BLUE);
    }
}

int main() {
    float screenWidth = 800;
    float screenHeight = 600;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Vitals");

    float data[MAX_DATA_POINTS];
    int dataCount = 4;

	SetTargetFPS(5);

    while (!WindowShouldClose()) {
		screenHeight = GetScreenHeight();
		screenWidth = GetScreenWidth();
        BeginDrawing();
        ClearBackground((Color){ 200, 200, 200, 255 });

        DrawText("Graph", 0.75 * screenWidth - (float)MeasureText("Graph", 20) / 2, 10, 20, RED);
        Rectangle graphArea = {screenWidth/2, 0, screenWidth, screenHeight/2};
        DrawGraph(data, dataCount, graphArea);
		if(dataCount < MAX_DATA_POINTS){
			data[dataCount] = rand() % 40 + 40;
			dataCount++;
		}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
