#pragma once

#include "../BpPhysicsStd.h"
#include "../Foundation/BpMath.h"
#include <vector>

namespace bPhysics
{
	class BpShape;
	class BpRigidActor;

	enum DebugShapeType
	{
		LINE,
		SPHERE,
		BOX,
		CAPSULE
	};

	class BpDebugShape
	{
	public:
		BpDebugShape(DebugShapeType shapeType) : shapeType(shapeType), color(BpVec3(0, 255, 0)) {}
		BpDebugShape(DebugShapeType shapeType, const BpShape* pShape, const BpRigidActor* pOwner, BpVec3 color = BpVec3(255, 0, 0)) : shapeType(shapeType), m_pOwner(pOwner), m_pShape(pShape), color(color) {}

		BpMat4x4 GetTransform() const;

		BpVec3 color;
		DebugShapeType shapeType;

	private:
		const BpShape* m_pShape;
		const BpRigidActor* m_pOwner;
	};

	class BpDebugLine : public BpDebugShape
	{
	public:
		BpDebugLine() : BpDebugShape(DebugShapeType::LINE) {}
		BpDebugLine(BpVec3 extent, const BpShape* pShape, const BpRigidActor* pOwner, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::LINE, pShape, pOwner, color), extent(extent) {}

		BpVec3 extent;
	};

	class BpDebugSphere : public BpDebugShape
	{
	public:
		BpDebugSphere() : BpDebugShape(DebugShapeType::SPHERE) {}
		BpDebugSphere(float radius, const BpShape* pShape, const BpRigidActor* pOwner, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::SPHERE, pShape, pOwner, color), radius(radius)  {}

		float radius;
	};

	class BpDebugBox : public BpDebugShape
	{
	public:
		BpDebugBox() : BpDebugShape(DebugShapeType::BOX) {}
		BpDebugBox(BpVec3 extent, const BpShape* pShape, const BpRigidActor* pOwner, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::BOX, pShape, pOwner, color), extent(extent) {}

		BpVec3 extent;
	};

	class BpDebugCapsule : public BpDebugShape
	{
	public:
		BpDebugCapsule() : BpDebugShape(DebugShapeType::CAPSULE) {}
		BpDebugCapsule(float radius, float halfHeight, const BpShape* pShape, const BpRigidActor* pOwner, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::CAPSULE, pShape, pOwner, color), radius(radius), halfHeight(halfHeight) {}

		float radius;
		float halfHeight;
	};

	class BpDebugRenderBuffer
	{
	public:
		~BpDebugRenderBuffer()
		{
			for (std::vector<BpDebugShape*>::iterator it = m_debugShapes.begin(); it != m_debugShapes.end(); it++)
			{
				BP_SAFE_DELETE(*it);
			}
			m_debugShapes.clear();
		}


	public:
		std::vector<BpDebugShape*> m_debugShapes;
	};

	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugSphere& sphere);
	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugBox& box);
	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugCapsule& capsule);
}