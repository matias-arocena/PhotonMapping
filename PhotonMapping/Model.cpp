#include "Model.h"

Model::Model(RTCDevice device)
{
	Geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

	VertexBuffer = (float*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 3);
	VertexBuffer[0] = 0.f; VertexBuffer[1] = 0.f; VertexBuffer[2] = 0.f;
	VertexBuffer[3] = 1.f; VertexBuffer[4] = 0.f; VertexBuffer[5] = 0.f;
	VertexBuffer[6] = 0.f; VertexBuffer[7] = 1.f; VertexBuffer[8] = 0.f;

	IndexBuffer = (unsigned*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), 1);
	IndexBuffer[0] = 0; IndexBuffer[1] = 1; IndexBuffer[2] = 2;

	rtcCommitGeometry(Geometry);
}

Model::~Model()
{
	rtcReleaseGeometry(Geometry);
}

RTCGeometry Model::GetGeometry()
{
	return Geometry;
}
