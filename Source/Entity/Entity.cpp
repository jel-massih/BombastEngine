#include "Entity.h"

Entity::Entity()
{
	m_posX = 0;
	m_posY = 0;
	m_width = 0;
	m_height = 0;
}

Entity::~Entity()
{
}

Entity::Entity(Entity const& other)
{
}

bool Entity::Initialize()
{
	// @TODO: Initialize m_name to Arbitrary Value

	return true;
}

void Entity::Shutdown()
{
	if (m_name)
	{
		SAFE_DELETE(m_name);
	}
}

void Entity::GetPosition(int& outX, int& outY)
{
	//@TODO: Check to see if this is how to do this. Seems like dangerous since returning reference to this objects memory
	outX = m_posX;
	outY = m_posY;
}

void Entity::SetPosition(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

void Entity::GetSize(int& outWidth, int& outHeight)
{
	outWidth = m_width;
	outHeight = m_height;
}

void Entity::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Entity::GetName(TCHAR& out)
{
	out = *m_name;
}