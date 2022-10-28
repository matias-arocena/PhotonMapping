#include "Settings.h"

int Settings::width = 1024;
int Settings::height = 1024;
int Settings::maxDepth = 10;
glm::vec3 Settings::backgroundColor = { 0, 0, 0 };
int Settings::fov = 90;
int Settings::smoothness = 8;
int Settings::globalPhotonQuantity = 500000;
int Settings::causticPhotonQuantity = 100000;