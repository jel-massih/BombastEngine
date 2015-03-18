#pragma once

namespace bPhysics
{
	class BpScene;
	class BpSceneDesc;
	class BpMaterial;

	class BpPhysicsCore
	{
	public:
		BpPhysicsCore();
		~BpPhysicsCore();

		BpScene* CreateScene(const BpSceneDesc& sceneDesc);

		BpMaterial* CreateMaterial(float dynamicFriction, float staticFriction, float restitution);

	private:
		BpScene* m_pScene;
	};
}