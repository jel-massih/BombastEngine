#pragma once

class Canvas
{
public:
	Canvas();
	~Canvas();

public:
	int DrawString(float posX, float posY, const TCHAR* text, const Vec3& color);
};