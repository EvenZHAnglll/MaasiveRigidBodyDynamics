
#include <string.h>
#include "glut.h"

#include "World.h"
#include "Body.h"
#include "Joint.h"

namespace
{
	Body bodies[300];
	Joint joints[100];
	
	Body* bomb = NULL;

	float timeStep = 1.0f / 100.0f;
	int iterations = 10;
	Vec2 gravity(0.0f, -10.0f);

	int numBodies = 0;
	int numJoints = 0;

	int demoIndex = 3;
	float cam_x = 15;  //camera position

	float cam_z = 25;
	World world(gravity, iterations);
}

void DrawText(int x, int y, const char *string)
{
	int len, i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void DrawFloor()
{
	
	
	


	glBegin(GL_QUADS);
	//front grey
	glColor4f(0.4f, 0.4f, 0.4f, 0.9f);
	glVertex3f(5.0f, -0.00f, -5.3f);
	glVertex3f(5.0f, -0.00f, 4.7f);
	glVertex3f(-5.0f, -0.00f, 4.7f);
	glVertex3f(-5.0f, -0.00f, -5.3f);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(0.3f, 0.3f, 0.4f, 0.9f);
	for (int i = 0; i < 11; i++) {
		glVertex3f(-5.0f, 0.0f, -5.3f+(float)i);
		glVertex3f(5.0f, 0.0f, -5.3f+(float)i);
	}
	for (int i = 0; i < 11; i++) {
		glVertex3f(-5.0f + (float)i, 0.0f, -5.3f);
		glVertex3f(-5.0f + (float)i, 0.0f, 4.7f);
	}
	glEnd();


}
void DrawBody(Body* body)
{
	Mat22 R(body->rotation);
	Vec2 x = body->position;
	Vec2 h = 0.5f * body->width;

	Vec2 v1 = x + R * Vec2(-h.x, -h.y);
	Vec2 v2 = x + R * Vec2( h.x, -h.y);
	Vec2 v3 = x + R * Vec2( h.x,  h.y);
	Vec2 v4 = x + R * Vec2(-h.x,  h.y);

	

	
	glBegin(GL_QUADS);
	//front grey
	if (body->mass >45)
		glColor4f(0.9f, 0.7f, 0.7f, 0.9f);
	else
		glColor4f(0.75f, 0.75f, 0.75f, 0.9f);

	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glVertex2f(v3.x, v3.y);
	glVertex2f(v4.x, v4.y);

	//back red
	glColor4f(0.75f, 0.75f, 0.75f, 0.9f);
	glVertex3f(v1.x, v1.y, -1.0f);
	glVertex3f(v2.x, v2.y, -1.0f);
	glVertex3f(v3.x, v3.y, -1.0f);
	glVertex3f(v4.x, v4.y, -1.0f);
	//down green
	glColor4f(0.8f, 0.8f, 0.8f, 0.9f);
	glVertex2f(v1.x, v1.y);
	glVertex3f(v1.x, v1.y, -1.0f);
	glVertex3f(v2.x, v2.y, -1.0f);
	glVertex2f(v2.x, v2.y);
	//up blue
	glColor4f(0.8f, 0.8f, 0.8f, 0.9f);
	glVertex2f(v3.x, v3.y);
	glVertex3f(v3.x, v3.y, -1.0f);
	glVertex3f(v4.x, v4.y, -1.0f);
	glVertex2f(v4.x, v4.y);

	//left
	glColor4f(0.7f, 0.7f, 0.7f, 0.9f);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v4.x, v4.y);
	glVertex3f(v4.x, v4.y, -1.0f);
	glVertex3f(v1.x, v1.y, -1.0f);

	//right
	glColor4f(0.7f, 0.7f, 0.7f, 0.9f);
	glVertex2f(v2.x, v2.y);
	glVertex2f(v3.x, v3.y);
	glVertex3f(v3.x, v3.y, -1.0f);
	glVertex3f(v2.x, v2.y, -1.0f);
	glEnd();

	if (body == bomb)
		glColor4f(0.9f, 0.1f, 0.1f, 1.0f);
	else
		glColor4f(0.9f, 0.9f, 0.9f, 1.0f);

	glBegin(GL_LINE_LOOP);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glVertex2f(v3.x, v3.y);
	glVertex2f(v4.x, v4.y);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(v1.x, v1.y, -1.0f);
	glVertex3f(v2.x, v2.y, -1.0f);
	glVertex3f(v3.x, v3.y, -1.0f);
	glVertex3f(v4.x, v4.y, -1.0f);

	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2f(v1.x, v1.y);
	glVertex3f(v1.x, v1.y, -1.0f);
	glVertex3f(v2.x, v2.y, -1.0f);
	glVertex2f(v2.x, v2.y);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(v3.x, v3.y);
	glVertex3f(v3.x, v3.y, -1.0f);
	glVertex3f(v4.x, v4.y, -1.0f);
	glVertex2f(v4.x, v4.y);
	glEnd();
}

void DrawJoint(Joint* joint)
{
	Body* b1 = joint->body1;
	Body* b2 = joint->body2;

	Mat22 R1(b1->rotation);
	Mat22 R2(b2->rotation);

	Vec2 x1 = b1->position;
	Vec2 p1 = x1 + R1 * joint->localAnchor1;

	Vec2 x2 = b2->position;
	Vec2 p2 = x2 + R2 * joint->localAnchor2;

	glColor3f(0.5f, 0.5f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(x1.x, x1.y);
	glVertex2f(p1.x, p1.y);
	glVertex2f(x2.x, x2.y);
	glVertex2f(p2.x, p2.y);
	glEnd();

}

void LaunchBomb()
{
	//if (!bomb)
	//{
		bomb = bodies + numBodies;
		bomb->Set(Vec2(1.0f, 1.0f), 50.0f);
		bomb->friction = 0.2f;
		world.Add(bomb);
		++numBodies;
	//}

	bomb->position.Set(Random(-15.0f, 15.0f), 15.0f);
	bomb->rotation = Random(-1.5f, 1.5f);
	bomb->velocity = -1.5f * bomb->position;
	bomb->angularVelocity = Random(-20.0f, 20.0f);
}
void LaunchSuperBomb()
{

	bomb = bodies + numBodies;
	bomb->Set(Vec2(1.0f, 1.0f), 2000.0f);
	bomb->friction = 0.2f;
	world.Add(bomb);
	++numBodies;


	bomb->position.Set(Random(-15.0f, 15.0f), 15.0f);
	bomb->rotation = Random(-1.5f, 1.5f);
	bomb->velocity = -1.5f * bomb->position;
	bomb->angularVelocity = Random(-20.0f, 20.0f);
}

void LaunchNBomb()
{

    bomb = bodies + numBodies;
	bomb->Set(Vec2(1.2f, 1.2f), 500.0f);
	bomb->friction = 0.2f;
	world.Add(bomb);
	++numBodies;


	bomb->position.Set(Random(-15.0f, -20.0f), 5.0f);
	bomb->rotation = Random(-1.5f, 0.0f);
	bomb->velocity = Vec2(20.0f, 0.2f);
	bomb->angularVelocity = Random(-20.0f, 20.0f);
}

// Single box
void Demo1(Body* b, Joint* j)
{
	timeStep = 1.0f / 300.0f;
	b->Set(Vec2(10.0f, 10.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;
	 
	b->Set(Vec2(2.0f, 2.0f), 200.0f);
	b->position.Set(0.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;
}

// A simple pendulum
void Demo5(Body* b, Joint* j)
{
	Body* b1 = b + 0;
	b1->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b1->friction = 0.2f;
	b1->position.Set(0.0f, -0.5f * b1->width.y);
	b1->rotation = 0.0f;
	world.Add(b1);

	Body* b2 = b + 1;
	b2->Set(Vec2(1.0f, 1.0f), 100.0f);
	b2->friction = 0.2f;
	b2->position.Set(9.0f, 11.0f);
	b2->rotation = 0.0f;
	world.Add(b2);

	numBodies += 2;

	j->Set(b1, b2, Vec2(0.0f, 11.0f));
	world.Add(j);

	numJoints += 1;
}

// Varying friction coefficients
void Demo6(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 11.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(5.25f, 9.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(2.0f, 7.0f);
	b->rotation = 0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(-5.25f, 5.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 3.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	float friction[5] = {0.75f, 0.5f, 0.35f, 0.1f, 0.0f};
	for (int i = 0; i < 5; ++i)
	{
		b->Set(Vec2(0.5f, 0.5f), 25.0f);
		b->friction = friction[i];
		b->position.Set(-7.5f + 2.0f * i, 14.0f);
		world.Add(b);
		++b; ++numBodies;
	}
}

// A vertical stack
void Demo2(Body* b, Joint* j)
{
	timeStep = 1.0f / 300.0f;
	b->Set(Vec2(10.0f, 10.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;


	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(1.0f, 1.0f), 1.0f);
		b->friction = 0.2f;
		float x = Random(-0.4f, 0.4f);
		b->position.Set(x, 0.51f + 1.05f * i);
		world.Add(b);
		++b; ++numBodies;
	}
	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(1.0f, 1.0f), 1.0f);
		b->friction = 0.2f;
		float x = Random(-0.1f, 0.1f);
		b->position.Set(x+1.5f, 0.51f + 1.05f * i);
		world.Add(b);
		++b; ++numBodies;
	}
	//for (int i = 0; i < 10; ++i)
	//{
	//	b->Set(Vec2(1.0f, 1.0f), 1.0f);
	//	b->friction = 0.2f;
	//	float x = Random(-0.1f, 0.1f);
	//	b->position.Set(x + 1.5f*2, 0.51f + 1.05f * i);
	//	world.Add(b);
	//	++b; ++numBodies;
	//}
}
 
// A pyramid
void Demo3(Body* b, Joint* j)
{
	timeStep = 1.0f / 300.0f;
	b->Set(Vec2(10.0f, 10.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;


	Vec2 x(-4.0f, 1.75f);
	Vec2 y;

	for (int i = 0; i < 12; ++i)
	{
		y = x;

		for (int j = i; j <8; ++j)
		{
			b->Set(Vec2(1.0f, 1.5f), 5.0f);
			b->friction = 0.2f;
			b->position = y;
			world.Add(b);
			++b; ++numBodies;
			
			y += Vec2(1.125f, 0.0f);
		}

		//x += Vec2(0.5625f, 1.125f);
		x += Vec2(0.5625f, 2.0f);
	}
}

// A teeter
void Demo4(Body* b, Joint* j)
{
	timeStep = 1.0f / 100.0f;
	b->Set(Vec2(10.0f, 10.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;


	Vec2 x(-4.0f, 0.3f);
	Vec2 y;

	for (int i = 0; i < 10; ++i)
	{
		y = x;

		for (int j = 0; j < 9; ++j)
		{
			b->Set(Vec2(1.0f, 0.5f), 5.0f);
			b->friction = 0.2f;
			b->position = y;
			world.Add(b);
			++b; ++numBodies;

			y += Vec2(1.125f, 0.0f);
		}
		y = x + Vec2(-0.3f, 0.51f);
		for (int j = 0; j < 9; ++j)
		{
			b->Set(Vec2(1.0f, 0.5f), 5.0f);
			b->friction = 0.2f;
			b->position = y;
			world.Add(b);
			++b; ++numBodies;

			y += Vec2(1.125f, 0.0f);
		}

		//x += Vec2(0.5625f, 1.125f);
		x += Vec2(0.0f, 1.02f);
	}
}

// A suspension bridge
void Demo7(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	const int numPlanks = 15;
	float mass = 50.0f;

	for (int i = 0; i < numPlanks; ++i)
	{
		b->Set(Vec2(1.0f, 0.25f), mass);
		b->friction = 0.2f;
		b->position.Set(-8.5f + 1.25f * i, 5.0f);
		world.Add(b);
		++b; ++numBodies;
	}

	// Tuning
	float frequencyHz = 2.0f;
	float dampingRatio = 0.7f;

	// frequency in radians
	float omega = 2.0f * k_pi * frequencyHz;

	// damping coefficient
	float d = 2.0f * mass * dampingRatio * omega;

	// spring stifness
	float k = mass * omega * omega;

	// magic formulas
	float softness = 1.0f / (d + timeStep * k);
	float biasFactor = timeStep * k / (d + timeStep * k);

	for (int i = 0; i < numPlanks; ++i)
	{
		j->Set(bodies+i, bodies+i+1, Vec2(-9.125f + 1.25f * i, 5.0f));
		j->softness = softness;
		j->biasFactor = biasFactor;

		world.Add(j);
		++j; ++numJoints;
	}

	j->Set(bodies + numPlanks, bodies, Vec2(-9.125f + 1.25f * numPlanks, 5.0f));
	j->softness = softness;
	j->biasFactor = biasFactor;
	world.Add(j);
	++j; ++numJoints;
}

// Dominos
void Demo8(Body* b, Joint* j)
{
	Body* b1 = b;
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(12.0f, 0.5f), FLT_MAX);
	b->position.Set(-1.5f, 10.0f);
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(0.2f, 2.0f), 10.0f);
		b->position.Set(-6.0f + 1.0f * i, 11.125f);
		b->friction = 0.1f;
		world.Add(b);
		++b; ++numBodies;
	}

	b->Set(Vec2(14.0f, 0.5f), FLT_MAX);
	b->position.Set(1.0f, 6.0f);
	b->rotation = 0.3f;
	world.Add(b);
	++b; ++numBodies;

	Body* b2 = b;
	b->Set(Vec2(0.5f, 3.0f), FLT_MAX);
	b->position.Set(-7.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;

	Body* b3 = b;
	b->Set(Vec2(12.0f, 0.25f), 20.0f);
	b->position.Set(-0.9f, 1.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b3, Vec2(-2.0f, 1.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b4 = b;
	b->Set(Vec2(0.5f, 0.5f), 10.0f);
	b->position.Set(-10.0f, 15.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b2, b4, Vec2(-7.0f, 15.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b5 = b;
	b->Set(Vec2(2.0f, 2.0f), 20.0f);
	b->position.Set(6.0f, 2.5f);
	b->friction = 0.1f;
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b5, Vec2(6.0f, 2.6f));
	world.Add(j);
	++j; ++numJoints;

	Body* b6 = b;
	b->Set(Vec2(2.0f, 0.2f), 10.0f);
	b->position.Set(6.0f, 3.6f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b5, b6, Vec2(7.0f, 3.5f));
	world.Add(j);
	++j; ++numJoints;
}

// A multi-pendulum
void Demo9(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);

	Body * b1 = b;
	++b;
	++numBodies;

	float mass = 10.0f;

	// Tuning
	float frequencyHz = 4.0f;
	float dampingRatio = 0.7f;

	// frequency in radians
	float omega = 2.0f * k_pi * frequencyHz;

	// damping coefficient
	float d = 2.0f * mass * dampingRatio * omega;

	// spring stiffness
	float k = mass * omega * omega;

	// magic formulas
	float softness = 1.0f / (d + timeStep * k);
	float biasFactor = timeStep * k / (d + timeStep * k);

	const float y = 12.0f;

	for (int i = 0; i < 15; ++i)
	{
		Vec2 x(0.5f + i, y);
		b->Set(Vec2(0.75f, 0.25f), mass);
		b->friction = 0.2f;
		b->position = x;
		b->rotation = 0.0f;
		world.Add(b);

		j->Set(b1, b, Vec2(i, y));
		j->softness = softness;
		j->biasFactor = biasFactor;
		world.Add(j);

		b1 = b;
		++b;
		++numBodies;
		++j;
		++numJoints;
	}
}

void (*demos[])(Body* b, Joint* j) = {Demo1, Demo2, Demo3, Demo4, Demo5, Demo6, Demo7, Demo8, Demo9};
const char* demoStrings[9] = {
	"Demo 1: A Single Box",
	"Demo 2: Simple Pendulum",
	"Demo 3: Varying Friction Coefficients",
	"Demo 4: Randomized Stacking",
	"Demo 5: Pyramid Stacking",
	"Demo 6: A Teeter",
	"Demo 7: A Suspension Bridge",
	"Demo 8: Dominos",
	"Demo 9: Multi-pendulum"};

void InitDemo(int index)
{
	world.Clear();
	numBodies = 0;
	numJoints = 0;
	bomb = NULL;

	demoIndex = index;
	demos[index](bodies, joints);
}

void SimulationLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DrawText(5, 15, demoStrings[demoIndex]);
	//DrawText(5, 45, "Keys: 1-9 Demos, Space to Launch the Bomb");

	char buffer[64];
	//printf(buffer, "(A)ccumulation %s", World::accumulateImpulses ? "ON" : "OFF");
	//DrawText(5, 75, buffer);

	//printf(buffer, "(P)osition Correction %s", World::positionCorrection ? "ON" : "OFF");
	//DrawText(5, 105, buffer);

	//printf(buffer, "(W)arm Starting %s", World::warmStarting ? "ON" : "OFF");
	//DrawText(5, 135, buffer);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0f, -7.0f, -25.0f);


	world.Step(timeStep);
	DrawFloor();
	for (int i = 1; i < numBodies; ++i)
		DrawBody(bodies + i);

	for (int i = 0; i < numJoints; ++i)
		DrawJoint(joints + i);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
	gluLookAt(
		15, 7, 15,
		0, 5, 0,
		0, 1, 0
	);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	//case '5':
	//case '6':
	//case '7':
	//case '8':
	//case '9':
		InitDemo(key - '1');
		break;

	case 'a':
		World::accumulateImpulses = !World::accumulateImpulses;
		break;

	case 'p':
		World::positionCorrection = !World::positionCorrection;
		break;

	case 'w':
		World::warmStarting = !World::warmStarting;
		break;
	case 'q':
		cam_x -= 0.5;
		glutReshapeFunc(Reshape);
		break;
	case 'e':
		cam_x += 0.5;
		glutReshapeFunc(Reshape);
		break;
	case ' ':
		if(numBodies<300)
			LaunchBomb();
		break;
	case 'b':
		if (numBodies<300)
			LaunchSuperBomb();
		break;
	case 'n':
		if (numBodies<300)
			LaunchNBomb();
		break;
	}
}



int main(int argc, char** argv)
{
	InitDemo(3);
	glutInit(&argc, argv);

	glEnable(GLUT_MULTISAMPLE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	
	glutInitWindowSize(1600, 900);
	glutCreateWindow("Rigid Box");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glClearColor(0.05, 0.1, 0.15, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glutReshapeFunc(Reshape);
	glutDisplayFunc(SimulationLoop);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(SimulationLoop);

	glutMainLoop();

	return 0;
}
