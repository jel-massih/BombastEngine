#pragma once

#include <vector>

namespace bPhysics
{

	class BpShape;
	class BpContactConstraint;

	class BpIsland
	{
	public:
		void Solve();

		void Add(BpShape* shape);
		void Add(BpContactConstraint* contact);
		void Initialize();

		std::vector<BpShape*> m_shapes;
		//velocities

		std::vector<BpContactConstraint*> m_contacts;
		//contact states
	};

}