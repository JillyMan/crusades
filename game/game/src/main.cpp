#define WIN32_LEAN_AND_MEAN

#include "../resources/image_resources.h"
#include "../resources/sound_resources.h"
#include "../resources/string_resources.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>  // very important and include WINMM.LIB too!

#include "game.h"

#define GAME_CLASS_WINDOW L"WND_GAME"
#define GAME_NAME_WINDOW  L"Crousades"

const int ClientWindowWidth = 1280;
const int ClientWindowHeight = 720;

bool IsRunning;
HINSTANCE HInstanceApp = NULL;

int Counter;

struct mouse_input
{
};

struct game_input
{
    mouse_input Mouse;



};

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

void Win32PullMessages()
{
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
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
            break;
        }
        case WM_ACTIVATEAPP:
        {
            break;
        }

        case WM_CHAR: 
        {
            int ascii_code = w_param;
            int key_state = l_param;

            HDC hdc = GetDC(window_handle);

            SetTextColor(hdc, RGB(0, 255, 0));
            SetBkColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, OPAQUE);

            wchar_t buffer[512];

            wsprintf(buffer, L"WM_CHAR: Character = %c ", ascii_code);
            TextOut(hdc, 10, 50, buffer, wcslen(buffer));
            wsprintf(buffer, L"Key State = 0X%X ", key_state);

            TextOut(hdc, 10, 80, buffer, wcslen(buffer));
            
            
            ReleaseDC(window_handle, hdc);

            break;
        }
        case WM_PAINT:
        {
            Counter++;

            InvalidateRect(window_handle, NULL, TRUE);
            PAINTSTRUCT ps = {};
            HDC hdc = BeginPaint(window_handle, &ps);

            FillRect(ps.hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 0, 0)));

            wchar_t message[125];
            wsprintf(message, TEXT("Your score: %d"), Counter);
            
            int len = wcslen(message);

            SetBkMode(hdc, TRANSPARENT);

            TextOut(hdc, 10, 10, message, len);

            EndPaint(window_handle, &ps);
            break;
        }
        case WM_DESTROY:
        {
            IsRunning = false;
            PostQuitMessage(0);
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

#define KEYDOWN(vk_code) \
 ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) \
 ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

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
            while(IsRunning) 
            {
                Win32PullMessages();
                GameMain();

                HDC hdc = GetDC(window_handle);
                // Установка цвета и режима вывода строки
                SetTextColor(hdc, RGB(0, 255, 0));
                SetBkColor(hdc, RGB(0, 0, 0));
                SetBkMode(hdc, OPAQUE);
                // Вывод информации о состоянии
                // клавиш перемещения курсора
                wchar_t buffer[512];
                wsprintf(buffer, L"Up Arrow: = %d", KEYDOWN(VK_UP));
                TextOut(hdc, 0, 100, buffer, wcslen(buffer));
                wsprintf(buffer, L"Down Arrow: = %d", KEYDOWN(VK_DOWN));
                TextOut(hdc, 0, 120, buffer, wcslen(buffer));
                wsprintf(buffer, L"Right Arrow: = %d", KEYDOWN(VK_RIGHT));
                TextOut(hdc, 0, 140, buffer, wcslen(buffer));
                wsprintf(buffer, L"Left Arrow: = %d", KEYDOWN(VK_LEFT));
                TextOut(hdc, 0, 160, buffer, wcslen(buffer));
                // Освобождение контекста устройства
                ReleaseDC(window_handle, hdc);

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