#include "SceneNode.h"
#include "../BombastEngineStd.h"

SceneNodeProperties::SceneNodeProperties()
{
	m_actorId = INVALID_ACTOR_ID;
	m_radius = 0;
	m_renderPass = RenderPass_0;
	m_alphaType = AlphaOpaque;
}

void SceneNodeProperties::Transform(Mat4x4* toWorld, Mat4x4* fromWorld) const
{
	if (toWorld)
	{
		*toWorld = m_toWorld;
	}
	
	if (fromWorld)
	{
		*fromWorld = m_fromWorld;
	}
}