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
    float transparency;

public:
    Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 colorTransparent, float refraction, float reflection, float transparency, float specularExponent, float specularFactor);
    Material(aiMaterial* material);

    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    glm::vec3 getAmbient();
    glm::vec3 getColorTransparent();
    float getRefraction();
    float getReflection();
    float getTransparency();
    float getSpecularExponent();
    float getSpecularFactor();

};

