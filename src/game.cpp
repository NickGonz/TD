#include "game.h"
#include "raylib.h"

// Initialize the game state with starting values
void InitGame(GameState* game) {
    game->currentTime = 0.0f;
    game->currentPhase = PHASE_DAY;
    game->dayDuration = 120.0f;
    game->nightDuration = 300.0f;
}

// Update game logic - called every frame
void UpdateGame(GameState* game, float deltaTime) {
    game->currentTime += deltaTime;

    // Check if we need to switch phases
    if (game->currentPhase == PHASE_DAY) {
        if (game->currentTime >= game->dayDuration) {
            game->currentPhase = PHASE_NIGHT;
            game->currentTime = 0.0f;
        }
    } else {
        if (game->currentTime >= game->nightDuration) {
            game->currentPhase = PHASE_DAY;
            game->currentTime = 0.0f;
        }
    }
}

// Draw the game UI/HUD
void DrawGameUI(const GameState* game) {
    const char* phaseText = (game->currentPhase == PHASE_DAY) ? "DAY" : "NIGHT";
    Color phaseColor = (game->currentPhase == PHASE_DAY) ? ORANGE : DARKBLUE;

    float phaseDuration = (game->currentPhase == PHASE_DAY)
                          ? game->dayDuration
                          : game->nightDuration;
    float timeRemaining = phaseDuration - game->currentTime;

    DrawText(phaseText, 10, 10, 40, phaseColor);

    DrawText(TextFormat("Time: %.1f", timeRemaining), 10, 60, 30, BLACK);

    const char* helpText = (game->currentPhase == PHASE_DAY)
                           ? "Gather resources and build defenses!"
                           : "Survive the monster waves!";
    DrawText(helpText, 10, 550, 20, DARKGRAY);
}
