#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"

#define WORLD_WIDTH 135
#define WORLD_HEIGHT 135
#define CELL_SIZE 5

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

int world[WORLD_HEIGHT][WORLD_WIDTH];
int SandCount = 0;

void DrawWorld();
void RunPhysics();
bool IsValidCell(int x, int y);
bool IsSolidCell(int x, int y);
bool IsSandCell(int x, int y);
bool IsWaterCell(int x, int y);

void AddToWorld();

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");
    /* ToggleFullscreen(); */

    SetTargetFPS(60);

    float timer = 0.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawWorld();

        if (timer >= 0.02f) {
            RunPhysics();
            timer = 0.0f;
        }
        timer += GetFrameTime();

        AddToWorld();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawWorld()
{
    DrawRectangleLinesEx(Rectangle{0, 0, CELL_SIZE * WORLD_WIDTH, CELL_SIZE * WORLD_HEIGHT}, 1.0f, RAYWHITE);

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            // draw the grid

            /* DrawRectangleLinesEx(Rectangle{x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, 0.6f, RAYWHITE); */

            if (world[x][y] == -1) {
                DrawRectangleRec(Rectangle{x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, GRAY);
            }
            else if (world[x][y] == 1) {
                DrawRectangleRec(Rectangle{x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, YELLOW);
            }
            else if (world[x][y] == 2) {
                DrawRectangleRec(Rectangle{x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, BLUE);
            }
        }
    }
}

void RunPhysics()
{
    for (int y = WORLD_HEIGHT; y >= 0; y--) {
        for (int x = WORLD_WIDTH; x >= 0; x--) {
            if (IsValidCell(x, y) && IsSandCell(x, y)) {
                if (IsValidCell(x, y + 1)) {
                    int randomValue = GetRandomValue(0, 100);
                    int dir;
                    if (randomValue < 50) {
                        dir = -1;
                    }
                    else {
                        dir = 1;
                    }

                    if (!IsSolidCell(x, y + 1)) {
                        // move cell down
                        world[x][y] = 0;
                        world[x][y + 1] = 1;
                    }
                    else if (IsValidCell(x + dir, y + 1) && !IsSolidCell(x + dir, y + 1)) {
                        world[x][y] = 0;
                        world[x + dir][y + 1] = 1;
                    }
                    /* else if (IsValidCell(x + 1, y + 1) && !IsSolidCell(x + 1, y + 1)) { */
                    /*     world[x][y] = 0; */
                    /*     world[x + 1][y + 1] = 1; */
                    /* } */
                }
            }
            else if (IsValidCell(x, y) && IsWaterCell(x, y)) {
                if (IsValidCell(x, y + 1) && !IsSolidCell(x, y + 1)) {
                    // move cell down
                    world[x][y] = 0;
                    world[x][y + 1] = 2;
                }
                /* else if (IsValidCell(x - 1, y) && !IsSolidCell(x - 1, y)) { */
                /*     world[x][y] = 0; */
                /*     world[x - 1][y] = 2; */
                /* } */
                else if (IsValidCell(x + 1, y) && !IsSolidCell(x + 1, y)) {
                    world[x][y] = 0;
                    world[x + 1][y] = 2;
                }
            }
        }
    }
}

bool IsValidCell(int x, int y) { return x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT; }
bool IsSolidCell(int x, int y) { return world[x][y] == 1 || world[x][y] == 2 || world[x][y] == -1; }

bool IsSandCell(int x, int y) { return world[x][y] == 1; }
bool IsWaterCell(int x, int y) { return world[x][y] == 2; }

void AddToWorld()
{
    Vector2 mousePos = GetMousePosition();
    mousePos.x = floor(mousePos.x / CELL_SIZE);
    mousePos.y = floor(mousePos.y / CELL_SIZE);

    std::string mousePosStr = std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { // sand
        if (IsValidCell((int)mousePos.x, (int)mousePos.y)) {
            world[(int)mousePos.x][(int)mousePos.y] = 1;
            SandCount += 1;
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) { // solid
        for (int x = (int)mousePos.x - 2; x < (int)mousePos.x + 2; x++) {
            for (int y = (int)mousePos.y - 2; y < (int)mousePos.y + 2; y++) {
                if (IsValidCell(x, y)) {
                    world[x][y] = -1;
                }
            }
        }
    }

    if (IsKeyDown(KEY_X)) { // delete
        for (int x = (int)mousePos.x - 2; x < (int)mousePos.x + 2; x++) {
            for (int y = (int)mousePos.y - 2; y < (int)mousePos.y + 2; y++) {
                if (IsValidCell(x, y)) {
                    world[x][y] = 0;
                }
            }
        }
    }

    if (IsKeyDown(KEY_P)) { // place
        for (int x = (int)mousePos.x - 4; x < (int)mousePos.x + 4; x++) {
            for (int y = (int)mousePos.y - 4; y < (int)mousePos.y + 4; y++) {
                if (IsValidCell(x, y)) {
                    int randomValue = GetRandomValue(0, 100);
                    if (randomValue > 90)
                        world[x][y] = 1;
                }
            }
        }
    }
    DrawText(mousePosStr.c_str(), SCREEN_WIDTH - 300, 20, 20, RAYWHITE);

    std::string sandCountStr = std::to_string(SandCount);
    DrawText(sandCountStr.c_str(), SCREEN_WIDTH - 300, 40, 20, RAYWHITE);
}
