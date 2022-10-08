#include "PhotonMap.h"

#include <FreeImage.h>
#include <sstream>
#include "Settings.h"
#include "Scene.h"

void Photon::LoadFromString(std::string photonString)
{
	std::vector<std::string> fields = split(photonString, ';');
    position.x = std::stof(fields[0]);
	position.y = std::stof(fields[1]);
    position.z = std::stof(fields[2]);
    power.r = std::stof(fields[3]);
    power.g = std::stof(fields[4]);
    power.b = std::stof(fields[5]);
    phi = fields[6][0];
    theta = fields[7][0];

}

void Photon::trace(glm::vec3 from, glm::vec3 to, glm::vec3 power)
{
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

std::ostream& operator<<(std::ostream& o, const Photon& a)
{
	o << a.position.x << ";"
	    << a.position.y << ";"
        << a.position.z << ";"
        << a.power.r << ";"
        << a.power.g << ";"
        << a.power.b << ";"
        << a.phi << ";"
        << a.theta;

    return o;
}

void PhotonMap::addPhoton(const Photon& p)
{
    photons.push_back(p);
}

void PhotonMap::build()
{
    kdtree.setPoints(photons.data(), static_cast<int>(photons.size()));
    kdtree.buildTree();
}

const int PhotonMap::getSize() const
{
    return static_cast<int>(photons.size());
}

const Photon& PhotonMap::getPhoton(int i) const
{
    return photons[i];
}

std::vector<int> PhotonMap::queryKNearestPhotons(const glm::vec3& p, int k, float& maxDist2) const
{
    return kdtree.searchKNearest(p, k, maxDist2);
}

std::vector<glm::vec3> PhotonMap::getMapBuffer(Scene& scene)
{
   std::vector<glm::vec3> buffer;

    std::vector<Ray> camRays = scene.getCamera()->generateRaysCamera();
    for (Ray camRay : camRays)
    {
        scene.ThrowRay(camRay);

        glm::vec3 HitCoordinates;

        if (camRay.GetHit(HitCoordinates))
        {     
            float r2;
            std::vector<int> photonIndices = queryKNearestPhotons(HitCoordinates, 1, r2);

            int photonIndex = photonIndices[0];

            if (r2 < 0.001f)
            {
                Photon photon = photons[photonIndex];
                buffer.push_back(photon.power);
            }
            else
            {
                buffer.push_back(glm::vec3{ 0,0,0 });
            }
        }
        else
        {
            buffer.push_back(glm::vec3{ 0,0,0 });
            //std::cout << "No hit" << std::endl;
        }
    }
    return buffer;

}

