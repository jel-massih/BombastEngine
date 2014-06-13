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
	m_bitmaps.clear();
	m_models.clear();
}

void EntitiesManager::RegisterBitmap(BitmapClass* bitmap)
{
	m_bitmaps.push_back(bitmap);
}

void EntitiesManager::DeRegisterBitmap(BitmapClass* bitmap)
{
	//@TODO: Find best way to Deregister a Bitmap
	for (BitmapClass* arrayBitmap : m_bitmaps)
		continue;
		//BE_ERROR(L"DERP");
}

const std::vector<BitmapClass*> &EntitiesManager::GetBitmaps() const
{
	return m_bitmaps;
}