#pragma once

#include <stdint.h>
#include <assert.h>

#define ArrayCount(A) (sizeof(A) / sizeof(A[0]))

struct game_button_state
{
    int State;
};

struct game_controller_input
{
    int IsConnected;
    int IsAnalog;
    float StickAverageX;
    float StickAverageY;

    union {
        game_button_state Buttons[12];
        struct
        {
            game_button_state MoveUp;
            game_button_state MoveDown;
            game_button_state MoveLeft;
            game_button_state MoveRight;

            game_button_state ActionUp;
            game_button_state ActionDown;
            game_button_state ActionLeft;
            game_button_state ActionRight;

            game_button_state LeftShoulder;
            game_button_state RightShoulder;

            game_button_state Start;
            game_button_state Back;
        };
    };
};

struct game_input
{
    game_button_state MouseButtons[5];
    int MouseX;
    int MouseY;

    game_controller_input Controllers[5];
};

#define KEYBOARD_CONTROLLER_INDEX 0
    
inline game_controller_input* GetController(game_input* Input, int unsigned ControllerIndex)
{
    assert(ControllerIndex < ArrayCount(Input->Controllers));
    return &Input->Controllers[ControllerIndex];
}
