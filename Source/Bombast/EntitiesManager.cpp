#include "EntitiesManager.h"

#include "../Bombast/BombastApp.h"

EntitiesManager::EntitiesManager()
{
}

EntitiesManager::EntitiesManager(const EntitiesManager& other)
{

}

EntitiesManager::~EntitiesManager()
{

}

void EntitiesManager::Shutdown()
{
	for (BitmapClass* bitmap : m_bitmaps)
	{
		SAFE_DELETE(bitmap);
	}

	m_bitmaps.clear();
	m_models.clear();
}

void EntitiesManager::RegisterBitmap(BitmapClass* bitmap)
{
	m_bitmaps.push_back(bitmap);
}

void EntitiesManager::DeRegisterBitmap(BitmapClass* bitmap)
{
	std::vector<BitmapClass*>::iterator it;

	for (it = m_bitmaps.begin(); it != m_bitmaps.end();)
	{
		if ((*it) == bitmap) {
			it = m_bitmaps.erase(it);
		}
		else 
		{
			++it;
		}
	}
}

void EntitiesManager::RegisterModel(ModelClass* model)
{
	m_models.push_back(model);
}

void EntitiesManager::DeRegisterModel(ModelClass* model)
{
	//@TODO: Find best way to Deregister a Bitmap
	for (ModelClass* arrayModel : m_models)
		continue;
	//BE_ERROR(L"DERP");
}

const std::vector<BitmapClass*> &EntitiesManager::GetBitmaps() const
{
	return m_bitmaps;
}

const std::vector<ModelClass*> &EntitiesManager::GetModels() const
{
	return m_models;
}