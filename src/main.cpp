#include "raylib.h"
#include <stdlib.h>

#define MAX_DATA_POINTS 2000
#define DISPLAY_POINT 40

void DrawGraph(float *data, int dataCount, Vector3 color, Rectangle graphArea) {
    DrawRectangleRounded(graphArea, 0.1, 10, Fade(SKYBLUE, 0.2f)); 
    DrawRectangleLinesEx(graphArea, 2, Fade(DARKGRAY, 0.6f)); 

    if (dataCount < 2) return;

    int start = 0;
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

        DrawCircleGradient(startPoint.x, startPoint.y, 5, Color{color.x, color.y, color.z, 200}, RAYWHITE);
        DrawLineBezier(startPoint, endPoint, 3.0f, Color{color.x, color.y, color.z, 200});
    }
}

struct Graph {
    Vector3 color;
    float *data;
    int dataCount;
};

int main() {
    int screenWidth = 800;
    int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Sexy Vitals UI");

    float data[MAX_DATA_POINTS] = {50, 60, 55, 70}; 
    int dataCount = 4;

    Graph graphs[] = {
        {{33, 104, 166}, data, dataCount}, 
        {{32, 63, 51}, data, dataCount}    
    };

    int selected = 0;

    SetTargetFPS(5);

    while (!WindowShouldClose()) {

		screenHeight = GetScreenHeight();
		screenWidth = GetScreenWidth();
        BeginDrawing();
        ClearBackground(Fade(RAYWHITE, 0.95f));

		const float margin = 20;
        DrawText("Vitals Dashboard", screenWidth / 2 - MeasureText("Vitals Dashboard", 30) / 2, screenHeight - 30 - margin, 30, DARKGRAY);

        Rectangle graphArea = {screenWidth/2 - margin, margin, screenWidth/2 - margin, screenHeight/2 - margin};
		DrawGraph(graphs[selected].data, graphs[selected].dataCount, graphs[selected].color, graphArea);

        if (graphs[selected].dataCount < MAX_DATA_POINTS) {
            graphs[selected].data[graphs[selected].dataCount] = rand() % 40 + 40;
            graphs[selected].dataCount++;
        }

        DrawText("Press ESC to Exit", 20, screenHeight - 40, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
