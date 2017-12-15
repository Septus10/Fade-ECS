#pragma once

#include "ComponentFamilies.hpp"

struct Velocity
{
	Velocity()
	{
		VelX = VelY = VelZ = 0.f;
		JumpVelocity = 0.f;
		IsJumping = false;
		IsFalling = false;
	}

	static ECS::ComponentFamily GetComponentFamily()
	{
		return ECS::CF_Velocity;
	}

	float VelX, VelY, VelZ, JumpVelocity;
	bool IsJumping, IsFalling;

	Velocity operator*(float Multiplier)
	{
		Velocity New(*this);
		New.VelX *= Multiplier;
		New.VelY *= Multiplier;
		New.VelZ *= Multiplier;
		return New;
	}

	Velocity operator*(double Multiplier)
	{
		return operator*(static_cast<float>(Multiplier));
	}
};