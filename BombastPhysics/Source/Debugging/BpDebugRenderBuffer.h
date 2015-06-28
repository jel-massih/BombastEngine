#pragma once

#include "../BpPhysicsStd.h"
#include "../Foundation/BpMath.h"
#include <vector>

namespace bPhysics
{
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
		BpDebugShape(DebugShapeType shapeType, BpVec3 position, BpVec3 color = BpVec3(255, 0, 0)) : shapeType(shapeType), position(position), color(color) {}

		BpVec3 position;
		BpVec3 color;
		DebugShapeType shapeType;
	};

	class BpDebugLine : public BpDebugShape
	{
	public:
		BpDebugLine() : BpDebugShape(DebugShapeType::LINE) {}
		BpDebugLine(BpVec3 position, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::LINE, position, color), extent(extent) {}

		BpVec3 extent;
	};

	class BpDebugSphere : public BpDebugShape
	{
	public:
		BpDebugSphere() : BpDebugShape(DebugShapeType::SPHERE) {}
		BpDebugSphere(BpVec3 center, float radius, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::SPHERE, center, color), radius(radius)  {}

		float radius;
	};

	class BpDebugBox : public BpDebugShape
	{
	public:
		BpDebugBox() : BpDebugShape(DebugShapeType::BOX) {}
		BpDebugBox(BpVec3 center, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::BOX, center, color), extent(extent) {}

		BpVec3 extent;
	};

	class BpDebugCapsule : public BpDebugShape
	{
	public:
		BpDebugCapsule() : BpDebugShape(DebugShapeType::CAPSULE) {}
		BpDebugCapsule(BpVec3 center, float radius, float halfHeight, BpVec3 color = BpVec3(255, 0, 0)) : BpDebugShape(DebugShapeType::CAPSULE, center, color), radius(radius), halfHeight(halfHeight) {}

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