// crousades.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "crousades-main.h"

#include "core/ecs/ecs.h"
#include "core/particle_system/particles.h"

void GameInit(HWND window_handle)
{
	InitParticleSystem(window_handle, 200);
}

void GameMain(game_input* Input)
{
	ParticleSystemUpdate(Input->DtPerFrame);

    //HPEN pen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
      //HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
      //SelectObject(hdc, pen);
      //SelectObject(hdc, brush);
      //if (NewInput->MouseButtons[MOUSE_LEFT_BUTTON].State)
      //{
      //    MoveToEx(hdc, Input->MouseX, Input->MouseY, NULL);
      //}
      //if (NewInput->MouseButtons[MOUSE_RIGHT_BUTTON].State)
      //{
      //    LineTo(hdc, Input->MouseX, Input->MouseY);
      //}
      //RECT rect = {};
      //GetClientRect(window_handle, &rect);
      //int screenWidth = rect.right - rect.left;
      //if (NewInput->MouseButtons[MOUSE_MID_BUTTON].State)
      //{
      //    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
      //}
      //playerX += playerSpeed * NewInput->DtPerFrame;
      //if (playerX + playerWidth >= screenWidth)
      //{
      //    playerX = screenWidth - playerWidth;
      //    playerSpeed *= -1;
      //}
      //if (playerX < 0)
      //{
      //    playerX = 0;
      //    playerSpeed *= -1;
      //}
      //Rectangle(hdc, playerX, playerY, playerX + playerWidth, playerY + playerHeight);
      //DeleteObject(pen);
      //DeleteObject(brush);

      //wchar_t buf[128];
      //int len = wsprintf(buf, L"MS per sec:[%.6f]", 1000.0f / (float)frameTime);
      //TextOut(hdc, 30, 100, buf, len);
}

void GameShutdown()
{
	Shutdown();
}
