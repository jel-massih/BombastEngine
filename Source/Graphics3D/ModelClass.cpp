#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexData = nullptr;
	m_textureData = nullptr;
	m_normalData = nullptr;
	m_faceData = nullptr;

	m_faceCount = m_textureCount = m_normalCount = m_faceCount = 0;
}

ModelClass::~ModelClass()
{
	SAFE_DELETE_ARRAY(m_vertexData);
	SAFE_DELETE_ARRAY(m_textureData);
	SAFE_DELETE_ARRAY(m_normalData);
	SAFE_DELETE_ARRAY(m_faceData);
}