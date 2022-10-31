#include "PhotonMap.h"

#include <FreeImage.h>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Random.h"
#include "SquareLight.h"
#include "Settings.h"
#include "Scene.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>
#include <fstream>
#include <ostream>


void Photon::loadFromString(std::string photonString)
{
	std::vector<std::string> fields = split(photonString, ';');
    position.x = std::stof(fields[0]);
	position.y = std::stof(fields[1]);
    position.z = std::stof(fields[2]);
    power.r = std::stof(fields[3]);
    power.g = std::stof(fields[4]);
    power.b = std::stof(fields[5]);
    incidentDirection.x = std::stof(fields[6]);
    incidentDirection.y = std::stof(fields[7]);
    incidentDirection.z = std::stof(fields[8]);

}

std::ostream& operator<<(std::ostream& o, const Photon& a)
{
    o << a.position.x << ";"
        << a.position.y << ";"
        << a.position.z << ";"
        << a.power.r << ";"
        << a.power.g << ";"
        << a.power.b << ";"
        << a.incidentDirection.x << ";"
        << a.incidentDirection.y << ";"
        << a.incidentDirection.z;

    return o;
}

void Photon::trace(glm::vec3 origin, glm::vec3 direction, glm::vec3 power, int depth, float currentRefract, std::shared_ptr<PhotonMap> photonMap, bool isCausticMap)
{
    if (depth > Settings::maxDepth)
    {
        return;
    }

    std::shared_ptr<Ray> ray = std::make_shared<Ray>(origin, direction);

    Scene::getInstance().throwRay(ray);
    RTCRayHit* hit = ray->getRayHit();


    std::shared_ptr<Mesh> mesh = NULL;
    std::shared_ptr<SquareLight> squareLight = NULL;

    mesh = Scene::getInstance().getMeshWithGeometryID(hit->hit.geomID);
    if (mesh == NULL)
    {
        return;
    }

    {
        std::shared_ptr<Photon> photon = std::make_shared<Photon>();
        photon->power = power;
        photon->incidentDirection = direction;
        ray->getHit(photon->position);

        glm::vec3 normal = ray->getNormal();

        auto material = mesh->getMaterial();

        if (material->getOpacity() < 1.0f && isCausticMap)
        {
            // La normal se tiene que invertir cuando se esta adentro del objeto refractado
            // Esta solucion esta mal, funciona solo cuando el ior del material vidrio es distinto de 1
            // Y tampoco toma en cuenta cuando dos objetos transparentes se chocan
            glm::vec3 newDirection = { 0,0,0 };
            if (currentRefract != 1.f)
            {
                newDirection = glm::refract(direction, -normal, material->getRefraction());
            }
            else
            {
                newDirection = glm::refract(direction, normal, currentRefract / material->getRefraction());
            }

            Photon::trace(photon->position + newDirection * 0.01f, newDirection, photon->power, depth + 1, material->getRefraction(), photonMap, isCausticMap);

        }
        else
        {
            ReflectionType type = photon->russianRoulete(material, Random::getRussianRouletteValue());
            if (type == ReflectionType::Diffuse && !isCausticMap)
            {

                if (depth != 1 && material->getOpacity() != .0f)
                {
                    photonMap->addPhoton(photon);
                }


                glm::vec3 reflectedDirection = glm::sphericalRand(1.);
                // Vector esta atras de la superficie
                if (glm::dot(normal, reflectedDirection) < 0)
                {
                    reflectedDirection = -reflectedDirection;
                }

                Photon::trace(photon->position + reflectedDirection * 0.01f, reflectedDirection, photon->power * material->getDiffuse(), depth + 1, currentRefract, photonMap, isCausticMap);
            }
            else if (type == ReflectionType::Specular)
            {
                auto newDirection = glm::reflect(direction, normal);
                Photon::trace(photon->position + newDirection * 0.01f, newDirection, photon->power, depth + 1, currentRefract, photonMap, isCausticMap);
            }
            else // Absorb
            {
                
                if (depth != 1 && material->getOpacity() == 1.0f)
                {
                    photonMap->addPhoton(photon);
                }
                return;
            }

        }
    }
}

ReflectionType Photon::russianRoulete(std::shared_ptr<Material> material, float value)
{

    glm::vec3 diffuseCoefficient = material->getDiffuseCoefficient();
    glm::vec3 specularCoefficient = material->getSpecularCoefficient();
    float maxColorPower = std::max({ power.r, power.g, power.b });
    float diffuseProbability = std::max({ diffuseCoefficient.r * power.r, diffuseCoefficient.g * power.g, diffuseCoefficient.b * power.b }) / maxColorPower;
    float specularProbability = std::max({ specularCoefficient.r * power.r, specularCoefficient.g * power.g, specularCoefficient.b * power.b }) / maxColorPower;
    
    if (value < diffuseProbability)
    {
        return ReflectionType::Diffuse;
    }
    else if (value < diffuseProbability + specularProbability)
    {
        return ReflectionType::Specular;
    }
    else
    {
        return ReflectionType::Absortion;
    }
}

std::vector<std::string> Photon::split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void PhotonMap::addPhoton(std::shared_ptr<Photon> p)
{
    photons.push_back(p);
}

void PhotonMap::build()
{
    kdtree.setPoints(photons);
    kdtree.buildTree();
}

const int PhotonMap::getSize() const
{
    return static_cast<int>(photons.size());
}

const std::shared_ptr<Photon> PhotonMap::getPhoton(int i) const
{
    return photons[i];
}

std::vector<int> PhotonMap::queryKNearestPhotons(const glm::vec3& p, int k, float& maxDist2)
{
    return kdtree.searchKNearest(p, k, maxDist2);
}

std::vector<int> PhotonMap::queryKNearestPhotons(const glm::vec3& p, float radius, float& maxDist2)
{
    return kdtree.searchKNearest(p, radius, maxDist2);
}

std::vector<glm::vec3> PhotonMap::getMapBuffer()
{

    if (photons.size() == 0)
    {
        return std::vector<glm::vec3>(Settings::width * Settings::height, glm::vec3{0,0,0});
    }
    std::vector<std::shared_ptr<Ray>> camRays = Scene::getInstance().getCamera()->generateRaysCamera();

    std::vector<glm::vec3> buffer(camRays.size(), glm::vec3{0,0,0});

    #pragma omp parallel for
    for (int i = 0; i < camRays.size(); ++i)
    {
        Scene::getInstance().throwRay(camRays[i]);

        glm::vec3 HitCoordinates;

        if (camRays[i]->getHit(HitCoordinates))
        {
            float r2;
            std::vector<int> photonIndices = queryKNearestPhotons(HitCoordinates, 500, r2);
            int photonIndex = photonIndices[0];

            glm::vec3 totalPower = Settings::backgroundColor;
            if (photonIndices.size() > 0)
            {
                for (int photonIndex : photonIndices)
                {
                    totalPower += getPhoton(photonIndex)->power;

                }
            }

            float area = (glm::pi<float>() * r2);

            totalPower = totalPower / area;
            buffer[i] = glm::clamp(totalPower, {0,0,0}, {1,1,1}) * 255.f;

        }
    }
    return buffer;

}

void PhotonMap::exportKdTree(const std::string& path)
{
    kdtree.saveKdTreeToFile(path);
}

void PhotonMap::importKdTree(const std::string& path)
{
    std::string line;
    std::ifstream file;
    file.open(path);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::shared_ptr<Photon> p = std::make_shared<Photon>();
            p->loadFromString(line);

            addPhoton(p);
        }
        file.close();
    }
}

