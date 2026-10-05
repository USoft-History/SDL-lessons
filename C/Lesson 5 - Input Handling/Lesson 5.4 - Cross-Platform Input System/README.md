# Lesson 5.4 — Cross-Platform Input System

In previous lessons, we learned how to handle keyboard input on PC and controller input on PlayStation 2. However, the game code still depended on the specific platform.

In this lesson, we will create a unified **Input System** that allows the game to receive input from different devices through a common interface.

After completing this lesson, the game code will no longer need to know whether the input comes from a keyboard, gamepad, or another platform.

---

## What You Will Learn

* Creating a custom input layer
* Abstracting platform-specific code
* Working with an input state structure
* Supporting multiple simultaneous button presses
* Organizing a project for multiple platforms

---

# Step 1. Create an Input State Structure

Create a file called `input.h`.

Instead of processing individual events directly, we will store the current state of all buttons in a structure.

```c
typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;

    bool fire;
    bool quit;
} InputState;
```

This structure describes player actions rather than specific keyboard keys or gamepad buttons.

---

# Step 2. Add the Input System Interface

In the same file, declare the following functions:

```c
int Input_Init();
void Input_Poll(InputState *input);
```

Function responsibilities:

* `Input_Init()` — initializes the input system
* `Input_Poll()` — updates the current input state

---

# Step 3. Implement Input for PC

Create the file:

```text
src/input_pc.c
```

This implementation will use SDL keyboard input.

```c
int Input_Init()
{
    return 0;
}
```

Get the current keyboard state:

```c
const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
```

Fill the input structure:

```c
input->up = keyboard[SDL_SCANCODE_W] ||
            keyboard[SDL_SCANCODE_UP];

input->down = keyboard[SDL_SCANCODE_S] ||
              keyboard[SDL_SCANCODE_DOWN];

input->left = keyboard[SDL_SCANCODE_A] ||
              keyboard[SDL_SCANCODE_LEFT];

input->right = keyboard[SDL_SCANCODE_D] ||
               keyboard[SDL_SCANCODE_RIGHT];
```

Use the Escape key to exit:

```c
input->quit = keyboard[SDL_SCANCODE_ESCAPE];
```

---

# Step 4. Implement Input for PlayStation 2

Create the file:

```text
src/input_ps2.c
```

Initialize the controller drivers:

```c
SifInitRpc(0);

SifLoadModule("rom0:SIO2MAN", 0, NULL);
SifLoadModule("rom0:PADMAN", 0, NULL);

padInit(0);
```

Open the controller port:

```c
padPortOpen(0, 0, padBuf);
```

Read the controller state:

```c
padRead(0, 0, &buttons);
```

Fill the input structure:

```c
input->up = btns & PAD_UP;
input->down = btns & PAD_DOWN;

input->left = btns & PAD_LEFT;
input->right = btns & PAD_RIGHT;

input->fire = btns & PAD_CROSS;
```

---

# Step 5. Use the Input System in the Game

Now the game code no longer knows anything about the underlying platform.

Create an input state:

```c
InputState input = {0};
```

Update it:

```c
Input_Poll(&input);
```

Move the object:

```c
if (input.up)
    rect.y -= MOVE_SPEED;

if (input.down)
    rect.y += MOVE_SPEED;

if (input.left)
    rect.x -= MOVE_SPEED;

if (input.right)
    rect.x += MOVE_SPEED;
```

Exit the application:

```c
if (input.quit)
    running = false;
```

---

# Step 6. Support Multiple Simultaneous Inputs

Previously, the input function returned only a single action:

```c
INPUT_UP
INPUT_DOWN
INPUT_LEFT
INPUT_RIGHT
```

Because of this limitation, it was impossible to process multiple buttons at the same time.

For example:

```text
UP + LEFT
```

did not work correctly.

Now each button has its own state:

```c
input.up
input.left
```

This allows multiple buttons to be pressed simultaneously.

Examples:

```text
W + D
```

or

```text
UP + LEFT
```

The rectangle will move diagonally.

---

# Step 7. Center the Object on Screen

Instead of using fixed coordinates:

```c
SDL_Rect rect = {350, 250, 200, 100};
```

we can calculate the position automatically:

```c
SDL_Rect rect =
{
    (SCREEN_WIDTH - 200) / 2,
    (SCREEN_HEIGHT - 100) / 2,
    200,
    100
};
```

Now the object will always appear in the center of the screen regardless of the resolution.

---

# Project Structure

```text
src/
├── main.c
├── input.h
├── input_pc.c
└── input_ps2.c
```

---

# Result

In this lesson, we created a custom cross-platform input system.

The game code now works the same way on:

* Linux
* Windows
* PlayStation 2

All platform-specific logic is hidden inside the input system implementation, while the game itself uses a single, unified interface.

This is an important step toward building a proper game engine architecture.
