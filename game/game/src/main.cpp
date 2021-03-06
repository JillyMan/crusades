#define WIN32_LEAN_AND_MEAN
/*
    TODO: 
        - if window in windowed mode, double bufering will off.
        - Add logger

    BUGS:
        - if try to hide game window, will see error in LockVideoMemory function.
*/

#include "platform.h"

#include "../resources/image_resources.h"
#include "../resources/sound_resources.h"
#include "../resources/string_resources.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <assert.h>

////---- delete when game code will moved to dll
#include "crousades_main.h"
////----

#define GAME_CLASS_WINDOW L"WND_GAME"
#define GAME_NAME_WINDOW  L"Crousades"

const int ClientWindowWidth = 1920;
const int ClientWindowHeight = 1080;
    
bool IsRunning;
bool IsPause;
HINSTANCE HInstanceApp = NULL;

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

inline void Win32ProcessKeyboardMessage(game_button_state* NewState, int state)
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
            // todo: mb try to make game save here?
            break;
        }
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t vkCode = (uint32_t)message.wParam;
            uint64_t wasDown = (message.lParam & (1 << 30)) != 0;
            uint64_t isDown = (message.lParam & ((uint64_t)1 << 31)) == 0;

            if (wasDown != isDown)
            {
                if (vkCode == 'W')
                {

                }

                if (vkCode == 'P')
                {
                    IsPause = !IsPause;
                }
            }

            if (vkCode == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }

            break;
        }
        default:
            //todo: do we really have to translate message here?
            TranslateMessage(&message);
            DispatchMessageA(&message);
            break;
        }
    }
}

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
            if (w_param == WA_ACTIVE)
            {
                IsPause = false;
            }
            if (w_param == WA_INACTIVE)
            {
                IsPause = true;
            }

            break;
        }
        case WM_ACTIVATEAPP:
        {
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps = { 0 };
            HDC hdc = BeginPaint(window_handle, &ps);
            EndPaint(window_handle, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostMessage(window_handle, WM_QUIT, 0, 0);
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

    if (RegisterClassEx(&window_class))
    {
        int window_width = ClientWindowWidth;
        int window_height = ClientWindowHeight;

        HWND window_handle = CreateWindowEx(
            NULL,
            GAME_CLASS_WINDOW,
            GAME_NAME_WINDOW,
            WS_VISIBLE | WS_OVERLAPPEDWINDOW/*WS_POPUP*/,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window_width,
            window_height,
            NULL, NULL, hInstance, NULL
        );

        if (window_handle)
        {
            int MonitorRefreshHz = 60;
            HDC RefreshDC = GetDC(window_handle);
            int Win32RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);

            ReleaseDC(window_handle, RefreshDC);
            if (Win32RefreshRate > 1)
            {
                MonitorRefreshHz = Win32RefreshRate;
            }

            float GameUpdateHz = 60;
            float TargetSecondsPerFrame = (1.0f / (GameUpdateHz));
            int TargetMSPerFrame = 1000 / GameUpdateHz;

            NextSoundState();

            game_input Input[2] = {};
            game_input* NewInput = &Input[0];
            game_input* OldInput = &Input[1];

            float TargetFramePerSecond = 60.f;
            float TargetFramePerMS = 1000.f / TargetFramePerSecond;

            ULONGLONG startTime = GetTickCount64();

            GameInit(window_handle);

            IsRunning = true;

            while(IsRunning)
            {
                NewInput->DtPerFrame = TargetSecondsPerFrame;

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

                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[MOUSE_LEFT_BUTTON],
                    GetKeyState(VK_LBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[MOUSE_RIGHT_BUTTON],
                    GetKeyState(VK_RBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[MOUSE_MID_BUTTON],
                    GetKeyState(VK_MBUTTON) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[MOUSE_XBUTTON_1],
                    GetKeyState(VK_XBUTTON1) & (1 << 15));
                Win32ProcessKeyboardMessage(&NewInput->MouseButtons[MOUSE_XBUTTON_2],
                    GetKeyState(VK_XBUTTON2) & (1 << 15));

                Win32PullMessages(window_handle, NewKeyboardController);

                if (!IsPause)
                {
                    GameMain(NewInput);
                }

                game_input* Temp = NewInput;
                NewInput = OldInput;
                OldInput = Temp;

                uint64_t perCycle = GetTickCount64() - startTime;
                while ((GetTickCount64() - startTime) < TargetMSPerFrame);

                uint64_t nowTime = GetTickCount64();
                uint64_t perFrame = nowTime - startTime;
                startTime = nowTime;

#ifdef _DEBUG
                HDC hdc = GetDC(window_handle);
                wchar_t buf[255];
                int len = wsprintf(buf, L"Frame: [%I64d] ms, cycle: [%I64d] ms\n", perFrame, perCycle);
                OutputDebugStringW(buf);
                SetWindowTextW(window_handle, buf);
//                TextOut(hdc, 10, 50, buf, len);
                ReleaseDC(window_handle, hdc);
#endif
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