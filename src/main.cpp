#include "raylib.h"
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define MAX_DATA_POINTS 2000
#define DISPLAY_POINT 40
#define PORT 1234

static Texture2D lastTexture = {0};

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

        DrawCircleGradient(endPoint.x, endPoint.y, 5, Color{color.x, color.y, color.z, 200}, RAYWHITE);
        DrawLineBezier(startPoint, endPoint, 3.0f, Color{color.x, color.y, color.z, 200});
    }
}

struct Graph {
    Vector3 color;
    float *data;
    int dataCount;
};

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    
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

    SetTargetFPS(144);

    Texture2D previousTexture = LoadTextureFromImage(GenImageColor(1, 1, WHITE));     
    bool newFrameAvailable = false;

    while (!WindowShouldClose()) {

		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
        BeginDrawing();
        ClearBackground(Fade(RAYWHITE, 0.95f));

		const float margin = 20;
        DrawText("Vitals Dashboard", screenWidth / 2 - MeasureText("Vitals Dashboard", 30) / 2, screenHeight - 30 - margin, 30, DARKGRAY);

        Rectangle graphArea = {(float)screenWidth/2 + margin, margin, (float)screenWidth/2 - 2 * margin, (float)screenHeight/2 - 2 * margin};
		DrawGraph(graphs[selected].data, graphs[selected].dataCount, graphs[selected].color, graphArea);

        if (graphs[selected].dataCount < MAX_DATA_POINTS) {
            graphs[selected].data[graphs[selected].dataCount] = rand() % 40 + 40;
            graphs[selected].dataCount++;
        }

		Vector2 w = {screenWidth * 0.75f, screenHeight * 0.75f};
		DrawRing(w, 30, 80, 90,  0.60* 360 + 90, 20,  Fade(MAROON, 0.5f));
		DrawRingLines(w, 30, 80, 90,450, 20,  Fade(BLACK, 0.5f));
		DrawText("60%", w.x - MeasureText("60%", 28)/2, w.y - 14, 28, BLACK);

        DrawText("Press ESC to Exit", 20, screenHeight - 40, 4, GRAY);
        

        uint32_t frame_size = 0;
        int bytes_received = read(sock, &frame_size, sizeof(frame_size));

        char *frame_data = new char[frame_size];
        bytes_received = read(sock, frame_data, frame_size);

        Image img = LoadImageFromMemory(".jpg", (unsigned char *)frame_data, frame_size);
        Texture2D texture = LoadTextureFromImage(img);

        Rectangle rectangle = {20, 20, 380, 300}; 

        if (img.width > 0 && img.height > 0) {

            if (lastTexture.id != 0) {
                UnloadTexture(lastTexture);
            }

            lastTexture = texture;
        } else {
            texture = lastTexture;
        }

        DrawRectangleRec(rectangle, DARKGRAY);

        if (texture.id != 0) {
            DrawTexturePro(texture, 
                        {0, 0, (float)texture.width, (float)texture.height}, 
                        rectangle, 
                        {0, 0}, 
                        0.0f,
                        WHITE);
        }

        EndDrawing();

        delete[] frame_data;
    }
    
    close(sock);
    CloseWindow();

    return 0;
}
