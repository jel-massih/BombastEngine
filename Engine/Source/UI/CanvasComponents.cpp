#include "CanvasComponents.h"

void CanvasTextLabel::Draw(Canvas* pCanvas)
{
	if (pCanvas == nullptr || !HasValidText())
	{
		return;
	}

	Vec2 drawPos(m_position);
	Vec3 drawColor(m_color);

	DrawStringInternal(pCanvas, drawPos, drawColor);
}

bool CanvasTextLabel::HasValidText() const
{
	return !m_text.empty();
}

void CanvasTextLabel::DrawStringInternal(Canvas* pCanvas, const Vec2& drawPos, const Vec3& drawColor)
{

}