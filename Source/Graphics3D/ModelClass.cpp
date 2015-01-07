#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexData = nullptr;
	m_textureData = nullptr;
	m_normalData = nullptr;
	m_faceData = nullptr;

	m_faceCount = m_textureCount = m_normalCount = m_faceCount = 0;
}