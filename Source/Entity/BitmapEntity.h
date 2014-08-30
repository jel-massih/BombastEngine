#ifndef BITMAP_ENTITY_H
#define BITMAP_ENTITY_H

#include "../BombastEngineStd.h"
#include "Entity.h"
#include "../Graphics2D/BitmapClass.h"

class BitmapEntity : public Entity {
public:
	BitmapEntity();
	~BitmapEntity();
	BitmapEntity(const BitmapEntity& other);

	bool Initialize();
	void Shutdown();

	bool Frame();
	bool Render();

	bool CreateBitmap(std::string file);
	void SetBitmap(BitmapClass* bitmap);
	BitmapClass* GetBitmap();

private:

	BitmapClass* m_pBitmap;
};

#endif