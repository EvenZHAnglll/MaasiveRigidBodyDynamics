
#ifndef BODY_H
#define BODY_H

#include "MathUtils.h"

struct Body
{
	Body();
	void Set(const Vec2& w, float m);

	void AddForce(const Vec2& f)
	{
		force += f;
	}

	Vec2 position;
	float rotation;

	Vec2 velocity;
	float angularVelocity;

	Vec2 force;
	float torque;

	Vec2 width;

	float friction;
	float mass, invMass;
	float I, invI;
};

#endif
