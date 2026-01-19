#ifndef GAME_H
#define GAME_H

// Header guard prevents this file from being included multiple times

// Game phases
enum GamePhase {
    PHASE_DAY,
    PHASE_NIGHT
};

// Main game state - holds all the important game data
struct GameState {
    float currentTime;          // Total time elapsed in current phase
    GamePhase currentPhase;     // Are we in day or night?
    float dayDuration;          // How long day phase lasts (in seconds)
    float nightDuration;        // How long night phase lasts (in seconds)
};

// Function declarations (the "interface" - what you can call from other files)
void InitGame(GameState* game);                    // Set up the game at start
void UpdateGame(GameState* game, float deltaTime); // Update game logic each frame
void DrawGameUI(const GameState* game);            // Draw the UI/HUD

#endif // GAME_H
