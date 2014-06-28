#ifndef ENTITY_H
#define ENTITY_H

#include "../BombastEngineStd.h"
#include "../Graphics2D/BitmapClass.h"

class Entity {
public:
	Entity();
	virtual ~Entity();
	Entity(const Entity& other);

	virtual bool Initialize();
	virtual void Shutdown();

	virtual bool Frame() = 0;
	virtual bool Render() = 0;

	void GetPosition(int& outX, int& outY);
	void SetPosition(int x, int y);

	void GetSize(int& outWidth, int& outHeight);
	void SetSize(int width, int height);

	void GetName(TCHAR& out);

protected:
	int m_posX;
	int m_posY;

	int m_width;
	int m_height;

	TCHAR* m_name;
};

#endif