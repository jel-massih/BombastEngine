#include "CanvasComponents.h"
#include "Canvas.h"
#include "../Rendering/BatchedRenderables.h"

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
	const int textLength = m_text.length();
	const float maxCharHeight = 1.0f;

	Vec2 topLeft(0, 0);
	const float posX = topLeft.x;
	float posY = topLeft.y;

	float lineX = posX;

	m_drawSize = Vec2::g_ZeroVec2;

	if (m_pBatchedRenderables == nullptr)
	{
		m_pBatchedRenderables = pCanvas->GetTextRenderBatch();
		m_pBatchedRenderables->AllocateVertexArray(textLength * 4);
		m_pBatchedRenderables->AllocateIndexArray(textLength * 2);
	}

	for (int i = 0; i < textLength; i++)
	{
		const TCHAR curChar = m_text[i];
		const FontCharacter& fontChar = m_pFont->GetCharacter(curChar);

		if (m_drawSize.y == 0)
		{
			m_drawSize.y = maxCharHeight;
		}

		const bool bIsWhitespace = curChar == ' ';

		lineX += Font::FontKerning;

		if (!bIsWhitespace)
		{
			const float left = drawPos.x + lineX;
			const float top = drawPos.y + posY;
			const float right = drawPos.y + posY + fontChar.size;
			const float bottom = drawPos.y + posY + 1;


			int v00 = m_pBatchedRenderables->AddVertex(
				Vec3(left, top, 0.f),
				Vec2(fontChar.startU, fontChar.startV),
				drawColor);
			int v10 = m_pBatchedRenderables->AddVertex(
				Vec3(right, top, 0.f),
				Vec2(fontChar.endU, fontChar.startV),
				drawColor);
			int v01 = m_pBatchedRenderables->AddVertex(
				Vec3(left, bottom, 0.f),
				Vec2(fontChar.startU, fontChar.endV),
				drawColor);
			int v11 = m_pBatchedRenderables->AddVertex(
				Vec3(right, bottom, 0.f),
				Vec2(fontChar.endU, fontChar.endV),
				drawColor);

			m_pBatchedRenderables->DeclareTriangle(v00, v10, v11);
			m_pBatchedRenderables->DeclareTriangle(v00, v11, v01);
		}

		m_drawSize.x += lineX;
	}
}