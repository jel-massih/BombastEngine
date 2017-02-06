#include "Canvas.h"
#include "../Rendering/BatchedRenderables.h"

Canvas::Canvas()
	:m_pTextRenderBatch(nullptr)
{

}

Canvas::~Canvas()
{

}

BatchedRenderables* Canvas::GetTextRenderBatch()
{
	if (m_pTextRenderBatch == nullptr)
	{
		m_pTextRenderBatch = BE_NEW BatchedRenderables();
	}
	return m_pTextRenderBatch;
}

int Canvas::DrawString(float posX, float posY, const TCHAR* text, const Vec3& color)
{
	//CanvasTextLabel label(Vec2(posX, posY), )
	return 0;
}