#pragma once

class BatchedRenderables;

class Canvas
{
public:
	Canvas();
	~Canvas();

public:
	int DrawString(float posX, float posY, const TCHAR* text, const Vec3& color);

	BatchedRenderables* GetTextRenderBatch();

private:
	BatchedRenderables* m_pTextRenderBatch;
};