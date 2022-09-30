#include <embree3/rtcore.h>
#include <limits>
#include <iostream>

int main()
{
	RTCDevice device = rtcNewDevice(nullptr);
	RTCScene scene = rtcNewScene(device);

	RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

	float* vb = (float*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 3);
	vb[0] = 0.f; vb[1] = 0.f; vb[2] = 0.f;
	vb[3] = 1.f; vb[4] = 0.f; vb[5] = 0.f;
	vb[6] = 0.f; vb[7] = 1.f; vb[8] = 0.f;

	unsigned* ib = (unsigned*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), 1);
	ib[0] = 0; ib[1] = 1; ib[2] = 2;

	rtcCommitGeometry(geometry);
	rtcAttachGeometry(scene, geometry);
	rtcReleaseGeometry(geometry);
	rtcCommitScene(scene);

	RTCRayHit rayhit;
	rayhit.ray.org_x = 0.f; rayhit.ray.org_y = 0.f; rayhit.ray.org_z = -3.f;
	rayhit.ray.dir_x = 0.f; rayhit.ray.dir_y = 0.f; rayhit.ray.dir_z = 1.f;
	rayhit.ray.tnear = 0.f;
	rayhit.ray.tfar = std::numeric_limits<float>::infinity();
	rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

	RTCIntersectContext context;
	rtcInitIntersectContext(&context);

	rtcIntersect1(scene, &context, &rayhit);

	if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
		std::cout << "Intersection at t = " << rayhit.ray.tfar << std::endl;
	}
	else
	{
		std::cout << "No intersection." << std::endl;
	}

	rtcReleaseScene(scene);
	rtcReleaseDevice(device);
}
