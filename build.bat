@echo off
REM Build script for raylib project with MinGW

echo Building test.cpp...

g++ test.cpp -o test.exe ^
    -I raylib/include ^
    -L raylib/lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run with: test.exe
) else (
    echo Build failed!
)
