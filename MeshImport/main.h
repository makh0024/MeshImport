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

class VertFormat
{
public:
	VertFormat() {};
	VertFormat(vec3 pos, vec3 norm, vec2 text) { position = pos, normal = norm, texture = text; }

	vec3 position = vec3(0.f, 0.f, 0.f), normal = vec3(0.f, 0.f, 0.f);
	vec2 texture = vec2(0.f, 0.f);
};