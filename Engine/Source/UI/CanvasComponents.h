#pragma once

class Canvas;
class BatchedRenderables;

class CanvasComponent
{
public:
	CanvasComponent(const Vec2& pos)
		: m_position(pos)
		, m_color(Vec3(255, 255, 255)) {};

	virtual void Render(Canvas* pCanvas) = 0;
	
protected:
	Vec2 m_position;
	Vec3 m_color;
};

class CanvasTextLabel: public CanvasComponent
{
public:
	CanvasTextLabel(const Vec2& pos, const TCHAR* text, const Vec2& color, const Font* font)
		: CanvasComponent(pos)
		, m_text(text)
		, m_pFont(font)
		, m_drawSize(Vec2::g_ZeroVec2){}

	virtual void Render(Canvas* pCanvas) override;

protected:
	bool HasValidText() const;

	void RenderString(Canvas* pCanvas, const Vec2& drawPos, const Vec3& drawColor);

protected:
	std::wstring m_text;

	BatchedRenderables* m_pBatchedRenderables;
	const Font* m_pFont;

	//size of drawn text
	Vec2 m_drawSize;
};