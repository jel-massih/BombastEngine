#include "BitmapEntity.h"

BitmapEntity::BitmapEntity()
{
	m_pBitmap = 0;
}

BitmapEntity::~BitmapEntity()
{
}

BitmapEntity::BitmapEntity(const BitmapEntity& other)
{
}

bool BitmapEntity::Initialize()
{
	return true;
}

void BitmapEntity::Shutdown()
{
}

bool BitmapEntity::Frame()
{
	return true;
}

bool BitmapEntity::Render()
{
	return true;
}

bool BitmapEntity::CreateBitmap(WCHAR* file)
{
	bool result;
	m_pBitmap = BE_NEW(BitmapClass);
	if (!m_pBitmap)
	{
		return false;
	}

	//result = m_pBitmap->Initialize();
	return true;
}

void BitmapEntity::SetBitmap(BitmapClass* bitmap)
{
	m_pBitmap = bitmap;
}

BitmapClass* BitmapEntity::GetBitmap()
{
	return m_pBitmap;
}