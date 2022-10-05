#include "Material.h"
#include <embree3/rtcore_device.h>
#include <assimp/scene.h>


Material::Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 colorTransparent, float refraction, float reflection, float transparency, float specularExponent, float specularFactor)
{
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->reflection = glm::clamp(reflection, 0.f, 1.f);
	this->refraction = refraction;
	this->specular = specular;
	this->transparency = transparency;
	this->specularExponent = specularExponent;
	this->specularFactor = specularFactor;
	this->colorTransparent = colorTransparent;
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

	// Color transparent
	aiColor3D traColor(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_TRANSPARENT, traColor);

	// Specular exponent
	material->Get(AI_MATKEY_SHININESS, specularExponent);

	// Specular factor
	//material->Get(AI_MATKEY_SHININESS_STRENGTH, specularFactor);
	specularFactor = 0.4;

	// Ior
	material->Get(AI_MATKEY_REFRACTI, this->refraction);

	// Refraction - Transparency ? (Alpha en blender)
	material->Get(AI_MATKEY_OPACITY, this->transparency);

	// Store in glm vec3
	this->diffuse = assimpColorToGlm(difColor);
	this->specular = assimpColorToGlm(speColor);
	this->colorTransparent = assimpColorToGlm(traColor);
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

glm::vec3 Material::getColorTransparent()
{
	return this->colorTransparent;
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

float Material::getSpecularExponent()
{
	return this->specularExponent;
}

float Material::getSpecularFactor()
{
	return this->specularFactor;
}
