#pragma once

#include <Inc/ML_Engine.h>

using namespace ML_Engine;

class Ball
{
public:
	Math::Vector3 mPosition = Math::Vector3::Zero; // pos

	Math::Quaternion mRotation = Math::Quaternion::Identity; // rot
	float mAngle = 0.0f;
	Math::Vector3 mSpinAxis = Math::Vector3::YAxis;

	Math::Vector3 mVelocity = Math::Vector3::Zero; // vel
	float radius = 0.5f; // rad
	float mass = 1.0f; // mass
	Math::Vector4 mColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // color
};