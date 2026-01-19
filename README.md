# Day/Night Tower Defense

A tower defense game with a day/night cycle where you gather resources during the day and defend your heart from monster hordes at night.

## Game Concept

### Core Gameplay Loop
- **Day Phase (2 minutes)**: Gather resources (wood, stone, iron) to build towers and defenses
- **Night Phase (5 minutes)**: Survive monster waves that pathfind toward your heart
- **Progression**: Level up between waves to modify tower behaviors and unlock upgrades

### Key Features
- **The Heart**: Central structure mobs target - protect it at all costs
- **Tiered Materials**: Wood → Stone → Iron → Steel progression
- **Active Combat**: Fight alongside your towers with your own weapon
- **Tower Modifiers**: Customize tower behavior (fire rate vs piercing shots, etc.)
- **Smart AI**: Mobs pathfind to the heart and destroy obstacles

## Tech Stack
- **Language**: C/C++
- **Graphics**: raylib (cross-platform 2D)
- **Platforms**: Windows (MinGW) + Linux
- **Art Style**: Geometric shapes (circles, squares, triangles)

## Setup Instructions

### Windows (MinGW/MSYS2)

1. **Install raylib**:
   ```powershell
   # Create raylib directory
   mkdir raylib

   # Download raylib 5.0 for MinGW
   Invoke-WebRequest -Uri "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip" -OutFile "raylib.zip"

   # Extract
   Expand-Archive -Path "raylib.zip" -DestinationPath "raylib_temp"

   # Move files
   Move-Item -Path "raylib_temp/raylib-5.0_win64_mingw-w64/include" -Destination "raylib/"
   Move-Item -Path "raylib_temp/raylib-5.0_win64_mingw-w64/lib" -Destination "raylib/"

   # Clean up
   Remove-Item "raylib.zip"
   Remove-Item -Recurse "raylib_temp"
   ```

2. **Build and run**:
   ```bash
   make
   make run
   ```

### Linux

1. **Install raylib**:
   ```bash
   # Ubuntu/Debian
   sudo apt install libraylib-dev

   # Arch Linux
   sudo pacman -S raylib

   # Or build from source
   git clone https://github.com/raysan5/raylib.git
   cd raylib/src
   make PLATFORM=PLATFORM_DESKTOP
   sudo make install
   ```

2. **Build and run**:
   ```bash
   make
   make run
   ```

## Build Commands

- `make` - Build the game (default)
- `make run` - Build and run
- `make clean` - Remove build artifacts
- `make debug` - Build with debug symbols
- `make release` - Build optimized release version

## Project Structure

```
td/
├── src/          # Source files (.cpp)
├── include/      # Header files (.h)
├── build/        # Compiled object files (generated)
├── bin/          # Executable output (generated)
├── raylib/       # raylib library (Windows only, not committed)
├── Makefile      # Cross-platform build system
└── README.md     # This file
```

## Development Roadmap

### Phase 1: Core Systems
- [ ] Day/night cycle timer
- [ ] Player movement and camera
- [ ] Grid-based world representation
- [ ] Basic resource gathering

### Phase 2: Tower Defense Basics
- [ ] Tower placement system
- [ ] Enemy spawning and waves
- [ ] Pathfinding (A* algorithm)
- [ ] Tower targeting and shooting

### Phase 3: Combat & Progression
- [ ] Player combat mechanics
- [ ] The Heart structure
- [ ] Level up system
- [ ] Tower modifiers

### Phase 4: Polish
- [ ] UI/HUD
- [ ] Sound effects
- [ ] Particle effects
- [ ] Balance and gameplay tuning

## Learning Goals
- Low-level game development in C/C++
- Engine concepts: game loops, entity management, state machines
- Algorithms: pathfinding, collision detection
- Cross-platform development
- Foundation for building a custom game engine

## License
This is a personal learning project.
