#pragma once

namespace bPhysics
{

	enum BpVisualizationParams
	{
		MIN_VALUE = 0,

		//Overall Scale
		BASE_SCALE = MIN_VALUE,

		//Actor Axis Visualization
		ACTOR_AXES,

		//Shape Axes Visualization
		COLLISION_AXES,
		//General Shape Visualization
		COLLISION_SHAPES,

		MAX_VALUE,
		VALUE_COUNT
	};

}