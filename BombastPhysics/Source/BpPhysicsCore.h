#pragma once

#include "Geometry\BpGeometry.h"
#include "BpMaterial.h"

namespace bPhysics
{
	class BpScene;
	class BpSceneDesc;
	class BpMaterial;
	class BpShape;

	class BpPhysicsCore
	{
	public:
		BpPhysicsCore();
		~BpPhysicsCore();

		BpScene* CreateScene(const BpSceneDesc& sceneDesc);

		BpMaterial* CreateMaterial(float dynamicFriction, float staticFriction, float restitution);

		inline BpShape* CreateShape(const BpGeometry& geometry, const BpMaterial& material)
		{
			BpMaterial* mat = const_cast<BpMaterial*>(&material);
			return CreateShape(geometry, &mat, 1);
		}

		virtual BpShape* CreateShape(const BpGeometry& geometry, BpMaterial* const * materials, unsigned short materialCount);

	private:
		BpScene* m_pScene;
	};
}