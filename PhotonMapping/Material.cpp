#include "Material.h"
#include <embree3/rtcore_device.h>
#include <assimp/scene.h>


Material::Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float refraction, float reflection, float transparency)
{
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->reflection = glm::clamp(reflection, 0.f, 1.f);
	this->refraction = refraction;
	this->specular = specular;
	this->transparency = transparency;
}

glm::vec3 assimpColorToGlm(const aiColor3D &color)
{

	return glm::vec3(color.r, color.g, color.b);
}

Material::Material(aiMaterial* material)
{	
	/*
		Crea un material a partir de un material de assimp
	*/

	// Albedo
	aiColor3D difColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, difColor);

	// Specular
	aiColor3D speColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, speColor);

	// Ior
	material->Get(AI_MATKEY_REFRACTI, this->refraction);

	// Refraction - Transparency ? (Alpha en blender)
	material->Get(AI_MATKEY_OPACITY, this->transparency);

	// Store in glm vec3
	this->diffuse = assimpColorToGlm(difColor);
	this->specular = assimpColorToGlm(speColor);
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
