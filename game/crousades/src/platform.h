#pragma once

#include <stdint.h>
#include <assert.h>

#define internal static

#define ArrayCount(A) (sizeof(A) / sizeof(A[0]))

#define MOUSE_BUTTONS       5
#define MOUSE_LEFT_BUTTON   0
#define MOUSE_RIGHT_BUTTON  1
#define MOUSE_MID_BUTTON    2
#define MOUSE_XBUTTON_1     3
#define MOUSE_XBUTTON_2     4

#define KEYBOARD_CONTROLLER_INDEX 0

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
    float DtPerFrame;

    game_button_state MouseButtons[5];
    int MouseX;
    int MouseY;

    game_controller_input Controllers[5];
};
    
enum class GraphicsApiEnum
{
    DirectX,
    OpenGL
};

struct graphics_setting
{
    int IsWindowed;
    int ResolutionWidth;
    int ResolutionHeight;
    int IsSecondBuffer;

    GraphicsApiEnum GraphicsApiType;
};

struct game_settings
{
    float TimePerSecond;
    graphics_setting GraphicsSetting;
};

inline game_controller_input* GetController(game_input* Input, int unsigned ControllerIndex)
{
    assert(ControllerIndex < ArrayCount(Input->Controllers));
    return &Input->Controllers[ControllerIndex];
}
