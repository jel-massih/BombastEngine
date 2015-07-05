#pragma once

#include <vector>
#include "../Geometry/BpGeometry.h"

namespace bPhysics
{
	class BpShape;

	struct BpContactPair
	{
		int A;
		int B;
	};

	class BpBroadPhase
	{
	public:
		void InsertShape(BpShape* shape, const BpAABB& aabb);
		void RemoveShape(BpShape* shape);

		//Generate Contact List
		void UpdatePairs();

		void Update(int id, const BpAABB& aabb);

		bool TestOverlab(int a, int b) const;

	private:
		std::vector<BpContactPair*> m_contactPairs;

		BpDynamicAABBTree m_tree;
		int m_currentIndex;
	};

}