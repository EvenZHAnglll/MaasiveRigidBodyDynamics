
#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>
#include "MathUtils.h"
#include "Arbiter.h"

struct Body;
struct Joint;

struct World
{
	World(Vec2 gravity, int iterations) : gravity(gravity), iterations(iterations) {}

	void Add(Body* body);
	void Add(Joint* joint);
	void Clear();

	void Step(float dt);

	void BroadPhase();

	std::vector<Body*> bodies;
	std::vector<Joint*> joints;
	std::map<ArbiterKey, Arbiter> arbiters;
	Vec2 gravity;
	int iterations;
	static bool accumulateImpulses;
	static bool warmStarting;
	static bool positionCorrection;
};

#endif
