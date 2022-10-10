#include "SquareLight.h"

#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

SquareLight::SquareLight(float intensity, unsigned maximumEmittedPhotons, glm::vec3 color, glm::vec3 center, glm::vec3 normal, glm::vec3 v) : Light(intensity, maximumEmittedPhotons, color), center{ center }, normal{ glm::normalize(normal) }, v{ v } {
    u = glm::cross(normal, u);
}

void SquareLight::createEmbreeMesh(RTCDevice device)
{
    geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
    RTCError error = rtcGetDeviceError(device);
    vertexBuffer = (float*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 4);
    error = rtcGetDeviceError(device);
    indexBuffer = (unsigned*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4, 4 * sizeof(unsigned), 1);
    error = rtcGetDeviceError(device);
    u = glm::cross(glm::normalize(normal), v);

    glm::vec3 vertx[4];
    vertx[0] = center + u - v;
    vertx[1] = center + u + v;
    vertx[2] = center - u + v;
    vertx[3] = center - u - v;

    // Set vertex and index buffer
    for (int i = 0; i < 4; i++)
    {
        vertexBuffer[i * 3]     = vertx[i].x;
        vertexBuffer[i * 3 + 1] = vertx[i].y;
        vertexBuffer[i * 3 + 2] = vertx[i].z;

        indexBuffer[i] = i;
    }

    rtcCommitGeometry(geometry);
}

glm::vec3 SquareLight::getPosition() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float>dist(-1.f, 1.f);

    float v_rand = dist(gen);
	float u_rand = dist(gen);
    return center + v * v_rand + u * u_rand;
}

glm::vec3 SquareLight::getPhotonDirection() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>dist(0, 255);

	int rand1 = dist(gen);
	int rand2 = dist(gen) % 128;
    return glm::normalize(glm::vec3(normal.x + cos(2 * rand1 * (1. / 256.) * M_PI) * sin(rand2 * (1. / 256.) * M_PI), normal.y + sin(2 * rand1 * (1. / 256.) * M_PI) * sin(rand2 * (1. / 256.) * M_PI), //direccion
        normal.z + cos(rand2 * (1. / 256.) * M_PI)));
}

glm::vec3 SquareLight::getNormal()
{
    return normal;
}

std::vector<glm::vec3> SquareLight::getRandomPositions(unsigned rowQty)
{
    std::vector<glm::vec3> points;

    for (unsigned i = 1; i < rowQty - 1; i++)
    {
        for (unsigned j = 1; j < rowQty - 1; j++)
        {
            points.push_back(getPosition());
        }
    }

    return points;
}

