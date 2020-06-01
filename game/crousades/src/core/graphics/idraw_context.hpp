#pragma once

class IDrawContext
{
public:
	virtual void StartDraw() = 0;

	virtual void Plot32(int x, int y, int color) = 0;

	virtual void GetVideoMemoryDimension(int& w, int& h) = 0;

	virtual void EndDraw() = 0;
};
