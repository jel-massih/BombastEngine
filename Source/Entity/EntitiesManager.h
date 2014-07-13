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

	void Shutdown();

	const std::vector<BitmapClass*> &GetBitmaps() const;
	const std::vector<ModelClass*> &GetModels() const;

	void RegisterBitmap(BitmapClass* bitmap);
	void DeRegisterBitmap(BitmapClass* bitmap);

	void RegisterModel(ModelClass* bitmap);
	void DeRegisterModel(ModelClass* bitmap);

private:
	std::vector<BitmapClass*> m_bitmaps;
	std::vector<ModelClass*> m_models;
};

#endif