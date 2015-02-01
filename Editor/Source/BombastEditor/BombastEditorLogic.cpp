#include "BombastEditorLogic.h"
#include "Physics/Physics.h"

BombastEditorLogic::BombastEditorLogic()
{
	m_pGamePhysics = CreateGamePhysics();
}

BombastEditorLogic::~BombastEditorLogic()
{
}