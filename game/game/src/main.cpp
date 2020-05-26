#define WIN32_LEAN_AND_MEAN

#include "platform.h"

#include "../resources/image_resources.h"
#include "../resources/sound_resources.h"
#include "../resources/string_resources.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <assert.h>

#include <stdlib.h> 

////---- delete when game code will moved to dll
#include "game.h"
////----

#define GAME_CLASS_WINDOW L"WND_GAME"
#define GAME_NAME_WINDOW  L"Crousades"

const int ClientWindowWidth = 1280;
const int ClientWindowHeight = 720;

bool IsRunning;
HINSTANCE HInstanceApp = NULL;

int Counter;

enum class SoundType {
    START,
    MAIN,
    END,
};

void NextSoundState()
{
    static SoundType CurrentSountState = SoundType::START;

#if 0
    switch (CurrentSountState) 
    {
    case SoundType::START:
        {
            PlaySound(MAKEINTRESOURCEW(SOUND_START_GAME), HInstanceApp, SND_SYNC | SND_RESOURCE);
            CurrentSountState = SoundType::MAIN;
            break;
        }
        case SoundType::MAIN:
        {
            PlaySound(MAKEINTRESOURCEW(SOUND_MAIN_MENU), HInstanceApp, SND_ASYNC | SND_LOOP | SND_RESOURCE);
            CurrentSountState = SoundType::END;
            break;
        }
        case SoundType::END:
        {
            PlaySound(MAKEINTRESOURCEW(SOUND_PLAYER_TELEPORT), HInstanceApp, SND_RESOURCE);
            CurrentSountState = SoundType::START;
            break;
        }
    }
#endif
}

void Fail(LPCWSTR message)
{
    MessageBox(NULL, message, L"Fail", MB_OK);
}

static void Win32ProcessKeyboardMessage(game_button_state* NewState, int state)
{
    if (NewState->State != state)
    {
        NewState->State = state;
    }
}

void Win32PullMessages(HWND window_handle, game_controller_input* Input)
{
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        switch (message.message)
        {
        case WM_QUIT:
        {
            IsRunning = false;
            break;
        }

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t vkCode = (uint32_t)message.wParam;
            int wasDown = (message.lParam & (1 << 30)) != 0;
            int isDown = (message.lParam & (1 << 31)) == 0;

            if (wasDown != isDown)
            {
                if (vkCode == 'W')
                {

                }
            }
            break;
        }
        default:
            TranslateMessage(&message);
            DispatchMessage(&message);
            break;
        }
    }
}

game_input* NewwInput = new game_input();

LRESULT CALLBACK Win32ProcessCallback(HWND window_handle, UINT message_type, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    switch (message_type) {

        case WM_CREATE:
        {
            NextSoundState();
            break;
        }
        case WM_ACTIVATE: 
        {
            break;
        }
        case WM_ACTIVATEAPP:
        {
            break;
        }
        case WM_PAINT:
        {
            Counter++;

            PAINTSTRUCT ps = { 0 };
            HDC hdc = BeginPaint(window_handle, &ps);
            EndPaint(window_handle, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostMessage(window_handle, WM_QUIT, 0, 0);
//            PostQuitMessage(0);
            break;
        }
        default:
            result = DefWindowProc(window_handle, message_type, w_param, l_param);
            break;
    } 

    return result;
}

WNDCLASSEX GetMyOwnWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX window_class = { 0 };
    window_class.hInstance = hInstance;
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    window_class.lpfnWndProc = &Win32ProcessCallback;

    window_class.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(GAME_ICON_LOGO));
    window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(GAME_ICON_LOGO));
    window_class.hCursor = LoadCursor(NULL, IDI_APPLICATION);

    window_class.hbrBackground = GetStockBrush(BLACK_BRUSH);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = GAME_CLASS_WINDOW;

    return window_class;
}

INT WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{
    HInstanceApp = hInstance;

    INT result = 0;
    WNDCLASSEX window_class = GetMyOwnWindowClass(hInstance);

    srand(2550);

    if (RegisterClassEx(&window_class))
    {
        int window_width = ClientWindowWidth;
        int window_height = ClientWindowHeight;

        HWND window_handle = CreateWindowEx(
            NULL,
            GAME_CLASS_WINDOW,
            GAME_NAME_WINDOW,
            WS_VISIBLE | WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window_width,
            window_height,
            NULL,NULL,hInstance, NULL
        );

        if (window_handle)
        {
            GameInit();
            IsRunning = true;
            
            NextSoundState();

            game_input Input[2] = {};
            game_input* NewInput = &Input[0];
            game_input* OldInput = &Input[1];

            while(IsRunning) 
            {
                game_controller_input* OldKeyboardController = GetController(OldInput, KEYBOARD_CONTROLLER_INDEX);
                game_controller_input* NewKeyboardController = GetController(NewInput, KEYBOARD_CONTROLLER_INDEX);

                *NewKeyboardController = {};
                NewKeyboardController->IsConnected = true;

                for (int i = 0; i < ArrayCount(NewKeyboardController->Buttons); ++i)
                {
                    NewKeyboardController->Buttons[i].State = OldKeyboardController->Buttons[i].State;
                }

                POINT MouseP;
                GetCursorPos(&MouseP);
                ScreenToClient(window_handle, &MouseP);
                NewInput->MouseX = MouseP.x;
                NewInput->MouseY = MouseP.y;
                NewwInput = NewInput;

                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[0],
                    GetKeyState(VK_LBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[1],
                    GetKeyState(VK_MBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[2],
                    GetKeyState(VK_RBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[3],
                    GetKeyState(VK_XBUTTON1) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[4],
                    GetKeyState(VK_XBUTTON2) & (1 << 15));

                Win32PullMessages(window_handle, NewKeyboardController);

                //getdc return dc on client area
                HDC hdc = GetDC(window_handle);
                
                //HDC hdc = GetWindowDC(window_handle);
                //HPEN pen = GetStockPen(BLACK_PEN);
                HPEN pen = CreatePen(PS_DASHDOT, 10, RGB(0, 0, 255));
                HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));

                SelectObject(hdc, pen);
                SelectObject(hdc, brush);

                RECT rect = {};
                GetClientRect(window_handle, &rect);
                FillRect(hdc, &rect, CreateSolidBrush(BLACK_BRUSH));

                for (int i = 0; i < 100; ++i) 
                {
                    int x = rand() % rect.right - rect.left;
                    int y = rand() % rect.bottom - rect.top;

                    COLORREF color = RGB(rand() % 255, rand() % 255, rand() % 255);

                    SetPixel(hdc, x, y, color);
                }

                DeleteObject(brush);
                DeleteObject(pen);

                ReleaseDC(window_handle, hdc);
                GameMain(NewInput);

                game_input* Temp = NewInput;
                NewInput = OldInput;
                OldInput = Temp;
            }

            GameShutdown();
            NextSoundState();
        }
        else 
        {
            Fail(L"Fail to CreateWindow()");
            result = GetLastError();
        }
    }
    else
    {
        Fail(L"Failed to RegistreClass");
        result = GetLastError();
    }

    return (result);
}