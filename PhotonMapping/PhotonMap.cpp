#include "PhotonMap.h"

#include <sstream>

void Photon::LoadFromString(std::string photonString)
{
	std::vector<std::string> fields = split(photonString, ';');
    position.x = std::atof(fields[0].c_str());
	position.y = std::atof(fields[1].c_str());
    position.z = std::atof(fields[2].c_str());
    power[0] = fields[3][0];
    power[1] = fields[4][0];
    power[2] = fields[5][0];
    power[3] = fields[6][0];
    phi = fields[7][0];
    theta = fields[8][0];

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
        << a.power[0] << ";"
        << a.power[1] << ";"
        << a.power[2] << ";"
        << a.power[3] << ";"
        << a.phi << ";"
        << a.theta;

    return o;
}
