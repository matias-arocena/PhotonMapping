#pragma once
#include <glm/glm.hpp>


class Material
{
private:
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float refraction;
    float reflection;
    float transparency;

public:
    Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float refraction, float reflection, float transparency);
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    glm::vec3 getAmbient();
    float getRefraction();
    float getReflection();
    float getTransparency();

};

