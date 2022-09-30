#pragma once
#include <embree3/rtcore.h>

class Model
{
public:
	Model(RTCDevice device);
	~Model();

	RTCGeometry GetGeometry();

private:
	RTCGeometry Geometry;
	float* VertexBuffer;
	unsigned* IndexBuffer;
};

