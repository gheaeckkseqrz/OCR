#include <iostream>
#include "OpenCL.h"

std::string getPlatformName (cl_platform_id id)
{
  size_t size = 0;
  clGetPlatformInfo (id, CL_PLATFORM_NAME, 0, nullptr, &size);
  std::string result;
 result.resize (size);
 clGetPlatformInfo (id, CL_PLATFORM_NAME, size, const_cast<char*> (result.data ()), nullptr);
 return result;
}

std::string getDeviceName (cl_device_id id)
{
  size_t size = 0;
  clGetDeviceInfo (id, CL_DEVICE_NAME, 0, nullptr, &size);
  std::string result;
  result.resize (size);
  clGetDeviceInfo (id, CL_DEVICE_NAME, size, const_cast<char*> (result.data ()), nullptr);
  return result;
}

OpenCL::OpenCL()
{ 
  for (auto platformId : getPlatformList())
    _deviceList.push_back(clDeviceList_t(platformId, getDeviceList(platformId)));

  std::cout << "Found " << _deviceList.size() << " platforms" << std::endl;
  for (auto p : _deviceList)
    {
      std::cout << "Platform : " << getPlatformName(p.first) << std::endl;
      for (auto d : p.second)
	std::cout << "\tDevice : " << getDeviceName(d) << std::endl;
    }
  
}

std::vector<cl_platform_id> OpenCL::getPlatformList()
{
  cl_uint platformIdCount = 0;
  clGetPlatformIDs (0, nullptr, &platformIdCount);
  std::vector<cl_platform_id> platformIds (platformIdCount);
  clGetPlatformIDs (platformIdCount, platformIds.data (), nullptr);
  return platformIds;
}

std::vector<cl_device_id> OpenCL::getDeviceList(cl_platform_id platformId)
{
  cl_uint deviceIdCount = 0;
  clGetDeviceIDs (platformId, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);
  std::vector<cl_device_id> deviceIds (deviceIdCount);
  clGetDeviceIDs (platformId, CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds.data (), nullptr);
  return deviceIds;
}
