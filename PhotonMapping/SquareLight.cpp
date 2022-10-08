#include "SquareLight.h"

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

glm::vec3 SquareLight::getNormal()
{
    return normal;
}

std::vector<glm::vec3> SquareLight::getRandomPositions(unsigned rowQty)
{
    std::vector<glm::vec3> points;

    float div = 2.f / RAND_MAX;
    for (unsigned i = 1; i < rowQty - 1; i++)
    {
        for (unsigned j = 1; j < rowQty - 1; j++)
        {
            float v_rand = (1 - rand() * div); // -1 .. 1
            float u_rand = (1 - rand() * div); // -1 .. 1
            points.push_back(center + v * v_rand + u * u_rand);
        }
    }

    return points;
}

