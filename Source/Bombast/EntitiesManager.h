#ifndef ENTITIES_MANAGER_H
#define ENTITIES_MANAGER_H

#include "../Graphics3D/ModelClass.h"
#include "../Graphics2D/BitmapClass.h"

#include <vector>

class EntitiesManager
{
public:
	EntitiesManager();
	EntitiesManager(const EntitiesManager& other);
	~EntitiesManager();

	bool Initialize();
	void Shutdown();

	BitmapClass* GetBitmaps();
	ModelClass* GetModels();

	void RegisterBitmap(BitmapClass* bitmap);
	void DeRegisterBitmap(BitmapClass* bitmap);

private:
	std::vector<BitmapClass*> m_bitmaps;
	std::vector<ModelClass*> m_models;
};

#endif