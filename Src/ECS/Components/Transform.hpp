#pragma once

#include "Velocity.hpp"
#include "ComponentFamilies.hpp"

struct Transform
{
	Transform()
	{
		PosX = PosY = PosZ = 0.f;
		RotX = RotY = RotZ = 0.f;
		SclX = SclY = SclZ = 0.f;
	}

	static ECS::ComponentFamily GetComponentFamily()
	{
		return ECS::CF_Transform;
	}

	float PosX, PosY, PosZ;
	float RotX, RotY, RotZ;
	float SclX, SclY, SclZ;

	Transform& operator+=(const Velocity& Vel)
	{
		PosX += Vel.VelX;
		PosY += Vel.VelY;
		PosZ += Vel.VelZ;
		return *this;
	}

};
