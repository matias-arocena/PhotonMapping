#include "SquareLight.h"

#include "Random.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

#include <glm/gtc/random.hpp>

SquareLight::SquareLight(float intensity, unsigned maximumEmittedPhotons, glm::vec3 color, glm::vec3 center, glm::vec3 normal, glm::vec3 v) : Light(intensity, maximumEmittedPhotons, color), center{ center }, normal{ glm::normalize(normal) }, v{ v } {
    u = glm::cross(normal, u);

    std::shared_ptr<Material> material = std::make_shared<Material>(color, color, color, color, 1.f, 1.f, 0.f, 0.f, 0.f);
    this->material = material;
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
    float v_rand = Random::getValueFromMinus1To1();
    float u_rand = Random::getValueFromMinus1To1();
    return center + v * v_rand + u * u_rand;
}

std::vector<glm::vec3> SquareLight::getCorners()
{
    std::vector<glm::vec3> res;
    res.push_back(center + v);
    res.push_back(center - v);
    res.push_back(center + u);
    res.push_back(center - u);
    res.push_back(center);
    return res;
}

glm::vec3 SquareLight::getPhotonDirection() const
{
    glm::vec3 randVec = glm::sphericalRand(1.);
    // Vector esta atras de la luz
    if (glm::dot(normal, randVec) < 0)
    {
        randVec = -randVec;
    }
    return randVec;
}

glm::vec3 SquareLight::getNormal()
{
    return normal;
}

//std::vector<glm::vec3> SquareLight::getRandomPositions(unsigned rowQty)
//{
//    std::vector<glm::vec3> points;
//
//    for (unsigned i = 1; i < rowQty - 1; i++)
//    {
//        for (unsigned j = 1; j < rowQty - 1; j++)
//        {
//            points.push_back(getPosition());
//        }
//    }
//
//    return points;
//}

std::vector<glm::vec3> SquareLight::getRandomPositions(int rowQty)
{
    std::vector<glm::vec3> points;
    glm::vec3 vSeg = v / (float)rowQty;
    glm::vec3 uSeg = u / (float)rowQty;

    for (int i = -rowQty/2; i < rowQty/2; i++)
    {
        for (int j = -rowQty/2; j < rowQty/2; j++)
        {
            points.push_back(center + vSeg * (float)i + uSeg * (float)j);
        }
    }

    return points;

}

