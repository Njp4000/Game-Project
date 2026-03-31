

## Description
This project is a simple Flappy Bird-style game developed in C using the raylib graphics library. The player controls a bird that must pass through gaps between pipes without colliding.

The game increases in difficulty over time as the pipe speed gradually increases.

## Controls

| Key | Action |
|-----|--------|
| Space | Jump |
| R | Restart game |

---

## How to Compile

Make sure you have:
- raylib installed
- MinGW (gcc) installed

Then run:

```bash
gcc main.c -o flappy.exe -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
