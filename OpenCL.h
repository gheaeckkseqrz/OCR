#ifndef __OPENCL_H__
#define __OPENCL_H__

#include <vector>
#ifdef __APPLE__
#include "OpenCL/opencl.h"
#else
#include "CL/cl.h"
#endif

class OpenCL
{
 public:
  OpenCL();

 private:
  std::vector<cl_platform_id> getPlatformList();
  std::vector<cl_device_id> getDeviceList(cl_platform_id platformId);
  

 private:
  typedef std::pair<cl_platform_id, std::vector<cl_device_id>> clDeviceList_t;
  std::vector<clDeviceList_t> _deviceList;

  cl_context _context;
  cl_kernel _kernel;
  cl_int _error;
  cl_mem _letterBuffer;  
};

#endif
