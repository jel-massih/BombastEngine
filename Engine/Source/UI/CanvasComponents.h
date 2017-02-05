#pragma once

class Canvas;

class CanvasComponent
{
public:
	CanvasComponent(const Vec2& pos)
		: m_position(pos)
		, m_color(Vec3(255, 255, 255)) {};

	virtual void Draw(Canvas* pCanvas) = 0;
	
protected:
	Vec2 m_position;
	Vec3 m_color;
};

class CanvasTextLabel: public CanvasComponent
{
public:
	CanvasTextLabel(const Vec2& pos, const TCHAR* text, const Vec2& color)
		: CanvasComponent(pos)
		, m_text(text){}

	virtual void Draw(Canvas* pCanvas) override;

protected:
	bool HasValidText() const;

	void DrawStringInternal(Canvas* pCanvas, const Vec2& drawPos, const Vec3& drawColor);

protected:
	std::wstring m_text;
};