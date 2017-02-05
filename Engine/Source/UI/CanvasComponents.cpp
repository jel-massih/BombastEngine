#include "CanvasComponents.h"

void CanvasTextLabel::Render(Canvas* pCanvas)
{
	if (pCanvas == nullptr || !HasValidText())
	{
		return;
	}

	Vec2 drawPos(m_position);
	Vec3 drawColor(m_color);

	m_pBatchedRenderables = nullptr;

	RenderString(pCanvas, drawPos, drawColor);
}

bool CanvasTextLabel::HasValidText() const
{
	return !m_text.empty();
}

void CanvasTextLabel::RenderString(Canvas* pCanvas, const Vec2& drawPos, const Vec3& drawColor)
{
	const float maxCharHeight = 1.0f;

	Vec2 topLeft(0, 0);
	const float posX = topLeft.x;
	float posY = topLeft.y;

	float lineX = posX;

	m_drawSize = Vec2::g_ZeroVec2;

	const int textLength = m_text.length();
	for (int i = 0; i < textLength; i++)
	{
		const TCHAR curChar = m_text[i];

		if (m_drawSize.y == 0)
		{
			m_drawSize.y = maxCharHeight;
		}
	}
}