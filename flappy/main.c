#include "raylib.h"
#include <stdbool.h>

//WINDOW SETTINGS
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//GAME SETTINGS
#define PIPE_COUNT 3
#define PIPE_WIDTH 80
#define PIPE_GAP 180
#define PIPE_SPACING 300
#define GROUND_HEIGHT 80

//BIRD
typedef struct {
    float x;
    float y;
    float velocity;
    float rotation;
    int width;
    int height;
} Bird;

//PIPE
typedef struct {
    float x;
    float gapY;
    bool passed;
} Pipe;

//RESET GAME FUNCTION
void ResetGame(Bird *bird, Pipe pipes[], int *score, float *pipeSpeed, float *timeAlive)
{
    // Reset bird values
    bird->x = 180;
    bird->y = 250;
    bird->velocity = 0;
    bird->rotation = 0;
    bird->width = 40;
    bird->height = 40;

    // Reset pipes
    for (int i = 0; i < PIPE_COUNT; i++) {
        pipes[i].x = 700 + i * PIPE_SPACING;
        pipes[i].gapY = (float)GetRandomValue(160, SCREEN_HEIGHT - GROUND_HEIGHT - 160);
        pipes[i].passed = false;
    }

    // Reset score and speed
    *score = 0;
    *pipeSpeed = 3.5f;
    *timeAlive = 0.0f;
}

//MAIN
int main(void)
{
    // Create the game window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Noah Passmore Game project");
    SetTargetFPS(60);

    Bird bird;
    Pipe pipes[PIPE_COUNT];

    int score = 0;
    float pipeSpeed = 3.5f;
    float timeAlive = 0.0f;
    bool gameOver = false;

    // Start the game with default values
    ResetGame(&bird, pipes, &score, &pipeSpeed, &timeAlive);

    //GAME LOOP
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        //UPDATE
        if (!gameOver)
        {
            // Increase the survival timer
            timeAlive += dt;

            // Make the game slowly get faster over time
            pipeSpeed = 3.5f + timeAlive * 0.2f;

            // Limit the max speed so it stays playable
            if (pipeSpeed > 10.0f) {
                pipeSpeed = 10.0f;
            }

            // Jump when SPACE is pressed
            if (IsKeyPressed(KEY_SPACE)) {
                bird.velocity = -8.5f;
            }

            // Gravity pulls the bird down
            bird.velocity += 0.42f;
            bird.y += bird.velocity;

            // Rotate the bird based on movement
            bird.rotation = bird.velocity * 3.0f;

            // Clamp rotation so it does not spin too much
            if (bird.rotation < -30) bird.rotation = -30;
            if (bird.rotation > 80) bird.rotation = 80;

            // Bird hitbox for collision
            Rectangle birdRect = {
                bird.x - bird.width / 2.0f,
                bird.y - bird.height / 2.0f,
                (float)bird.width,
                (float)bird.height
            };

            // Move each pipe
            for (int i = 0; i < PIPE_COUNT; i++) {
                pipes[i].x -= pipeSpeed;

                // If pipe moves off screen, move it to the right again
                if (pipes[i].x + PIPE_WIDTH < 0) {
                    float farthestX = pipes[0].x;

                    // Find the pipe furthest to the right
                    for (int j = 1; j < PIPE_COUNT; j++) {
                        if (pipes[j].x > farthestX) {
                            farthestX = pipes[j].x;
                        }
                    }

                    pipes[i].x = farthestX + PIPE_SPACING;
                    pipes[i].gapY = (float)GetRandomValue(160, SCREEN_HEIGHT - GROUND_HEIGHT - 160);
                    pipes[i].passed = false;
                }

                // Increase score when the bird passes a pipe
                if (!pipes[i].passed && bird.x > pipes[i].x + PIPE_WIDTH) {
                    pipes[i].passed = true;
                    score++;
                }

                // Create top pipe hitbox
                Rectangle topPipeRect = {
                    pipes[i].x,
                    0,
                    PIPE_WIDTH,
                    pipes[i].gapY - PIPE_GAP / 2.0f
                };

                // Create bottom pipe hitbox
                Rectangle bottomPipeRect = {
                    pipes[i].x,
                    pipes[i].gapY + PIPE_GAP / 2.0f,
                    PIPE_WIDTH,
                    SCREEN_HEIGHT - GROUND_HEIGHT - (pipes[i].gapY + PIPE_GAP / 2.0f)
                };

                // Check if bird hits either pipe
                if (CheckCollisionRecs(birdRect, topPipeRect) ||
                    CheckCollisionRecs(birdRect, bottomPipeRect)) {
                    gameOver = true;
                }
            }

            // Check collision with top of screen
            if (bird.y - bird.height / 2.0f <= 0) {
                gameOver = true;
            }

            // Check collision with ground
            if (bird.y + bird.height / 2.0f >= SCREEN_HEIGHT - GROUND_HEIGHT) {
                gameOver = true;
            }
        }
        else
        {
            // Restart the game when R is pressed
            if (IsKeyPressed(KEY_R)) {
                ResetGame(&bird, pipes, &score, &pipeSpeed, &timeAlive);
                gameOver = false;
            }
        }

        //DRaW
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Draw pipes
        for (int i = 0; i < PIPE_COUNT; i++) {
            int topHeight = (int)(pipes[i].gapY - PIPE_GAP / 2.0f);
            int bottomY = (int)(pipes[i].gapY + PIPE_GAP / 2.0f);
            int bottomHeight = SCREEN_HEIGHT - GROUND_HEIGHT - bottomY;

            // Top pipe
            DrawRectangle((int)pipes[i].x, 0, PIPE_WIDTH, topHeight, GREEN);

            // Bottom pipe
            DrawRectangle((int)pipes[i].x, bottomY, PIPE_WIDTH, bottomHeight, GREEN);
        }

        // Draw ground
        DrawRectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT, BROWN);

        // Draw bird as a circle
        DrawCircle((int)bird.x, (int)bird.y, 20, YELLOW);

        // Draw an eye to make it look more like a bird
        DrawCircle((int)bird.x + 8, (int)bird.y - 5, 4, BLACK);

        // Draw score and speed
        DrawText(TextFormat("Score: %d", score), 20, 20, 30, WHITE);
        DrawText(TextFormat("Speed: %.1f", pipeSpeed), 20, 55, 20, WHITE);

        // Draw game over screen
        if (gameOver) {
            DrawRectangle(220, 210, 360, 120, Fade(BLACK, 0.5f));
            DrawText("GAME OVER", 275, 225, 40, RED);
            DrawText("Press R to restart", 265, 275, 25, WHITE);
        }

        EndDrawing();
    }

    // Close the window when finished
    CloseWindow();
    return 0;
}