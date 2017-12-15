#pragma once
#include "ComponentFamilies.hpp"

struct Camera
{
	static ECS::ComponentFamily GetComponentFamily()
	{
		return ECS::CF_Camera;
	}
};
