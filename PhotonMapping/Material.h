#pragma once
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/material.h>


class Material
{
private:
    glm::vec3 diffuse;
    glm::vec3 specular;
    float specularExponent;
    float specularFactor;
    glm::vec3 ambient;
    glm::vec3 colorTransparent;
    float refraction;
    float reflection;
    float opacity;

    glm::vec3 diffuseCoefficient;
    glm::vec3 specularCoefficient;
    glm::vec3 transmissionCoefficient;


public:
    Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 colorTransparent, float refraction, float reflection, float opacity, float specularExponent, float specularFactor);
    Material(aiMaterial* material);

    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    glm::vec3 getAmbient();
    glm::vec3 getColorTransparent();
    float getRefraction();
    float getReflection();
    float getOpacity();
    float getSpecularExponent();
    float getSpecularFactor();

    glm::vec3 getDiffuseCoefficient();
    glm::vec3 getSpecularCoefficient();
    glm::vec3 getTransmissionCoefficient();



};

