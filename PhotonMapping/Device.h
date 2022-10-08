#pragma once

#include <embree3/rtcore.h>

class Device
{
public:
	static Device& getInstance();

	~Device();

	Device(Device const&) = delete;
	void operator=(Device const&) = delete;

	RTCDevice getDevice();
private:
	Device();

	RTCDevice device;
};

