#pragma once

namespace bPhysics
{
	class BpScene;
	class BpSceneDesc;

	class BpPhysicsCore
	{
	public:
		BpPhysicsCore();
		~BpPhysicsCore();

		BpScene* CreateScene(const BpSceneDesc& sceneDesc);

	private:
		BpScene* m_pScene;
	};
}