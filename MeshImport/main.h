#pragma once

class vec2
{
public:

	vec2();
	vec2(float ax, float ay) { x = ax, y = ay; }

	float x = 0, y = 0;
};

class vec3
{
public:
	vec3();
	vec3(float ax, float ay, float az) { x = ax, y = ay, z = az; }

	float x = 0, y = 0, z = 0;
};