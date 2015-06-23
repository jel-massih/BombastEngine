#include "BpPhysicsCore.h"
#include "BpMaterial.h"
#include "Scene\BpScene.h"
#include "Foundation\BpReporting.h"
#include "Geometry\BpGeometrySphere.h"
#include "Geometry\BpGeometryBox.h"
#include "Geometry\BpShape.h"

using namespace bPhysics;

BpPhysicsCore::BpPhysicsCore()
	: m_pScene(nullptr)
{

}

BpPhysicsCore::~BpPhysicsCore()
{

}

BpScene* BpPhysicsCore::CreateScene(const BpSceneDesc& sceneDesc)
{
	m_pScene = new BpScene;

	m_pScene->InitFromSceneDesc(sceneDesc);
	
	return m_pScene;
}

BpMaterial* BpPhysicsCore::CreateMaterial(float dynamicFriction, float staticFriction, float restitution)
{
	BpMaterial* material = new BpMaterial;

	material->SetDynamicFriction(dynamicFriction);
	material->SetStaticFriction(staticFriction);
	material->SetRestitution(restitution);

	return material;
}

BpShape* BpPhysicsCore::CreateShape(BpGeometry* geometry, BpMaterial* const * materials, unsigned short materialCount, bool isExclusive)
{
	if (!materials)
	{
		BP_ERROR("CreateShape: Material Pointer is NULL");
		return nullptr;
	}

	if (materialCount == 0)
	{
		BP_ERROR("CreateShape: Material Count is 0");
		return nullptr;
	}

	switch (geometry->GetType())
	{
	case BpGeometryType::SPHERE:
		if (!static_cast<BpGeometrySphere*>(geometry)->IsValid())
		{
			BP_ERROR("CreateShape: Supplied BpGeometry is not valid");
			return nullptr;
		}
		break;
	case BpGeometryType::BOX:
		if (!static_cast<BpGeometryBox*>(geometry)->IsValid())
		{
			BP_ERROR("CreateShape: Supplied BpGeometry is not valid");
			return nullptr;
		}
		break;
	default:
		BP_ERROR("CreateShape: Invalid Geometry Type");
		return nullptr;
	}

	if (materialCount > 1)
	{
		BP_ERROR("CreateShape: Currently Only 1 Material is supported");
		return nullptr;
	}

	BpShape* bpShape = new BpShape(geometry, isExclusive);

	if (!bpShape)
	{
		return nullptr;
	}

	return bpShape;
}