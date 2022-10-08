#include "Device.h"

Device::Device()
{
    device = rtcNewDevice(nullptr);
}

Device& Device::getInstance()
{
	static Device instance;
	return instance;
}

Device::~Device()
{
    rtcReleaseDevice(device);
}

RTCDevice Device::getDevice()
{
    return device;
}
