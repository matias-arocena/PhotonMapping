#include "Material.h"

Material::Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float refraction, float reflection, float transparency)
{
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->reflection = glm::clamp(reflection, 0.f, 1.f);
	this->refraction = refraction;
	this->specular = specular;
	this->transparency = transparency;
}

glm::vec3 Material::getDiffuse()
{
	return this->diffuse;
}

glm::vec3 Material::getSpecular()
{
	return this->specular;
}

glm::vec3 Material::getAmbient()
{
	return this->ambient;
}

float Material::getRefraction()
{
	return this->refraction;
}

float Material::getReflection()
{
	return this->reflection;
}

float Material::getTransparency()
{
	return this->transparency;
}
