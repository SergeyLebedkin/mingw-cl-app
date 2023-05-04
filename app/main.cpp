#include <iostream>
#include <string>
#include <vector>
#include <CL/opencl.h>

#include "ProgramSources.hpp"

// get platform info (string)
std::string GetPlatformInfoStr(const cl_platform_id platformId, const cl_platform_info platformInfo) {
    size_t paramValueSizeRet = 0;
    clGetPlatformInfo(platformId, platformInfo, 0, nullptr, &paramValueSizeRet);
    std::string paramValue(paramValueSizeRet, ' ');
    clGetPlatformInfo(platformId, platformInfo, paramValueSizeRet, paramValue.data(), nullptr);
    return paramValue;
}

// get device info (string)
std::string GetDeviceInfoStr(cl_device_id deviceId, cl_device_info deviceInfo) {
    size_t paramValueSizeRet = 0;
    clGetDeviceInfo(deviceId, deviceInfo, 0, nullptr, &paramValueSizeRet);
    std::string paramValue(paramValueSizeRet, ' ');
    clGetDeviceInfo(deviceId, deviceInfo, paramValueSizeRet, paramValue.data(), nullptr);
    return paramValue;
}

// get device info (size_t)
size_t GetDeviceInfoSizeT(cl_device_id deviceId, cl_device_info deviceInfo) {
    size_t paramValue = 0;
    clGetDeviceInfo(deviceId, deviceInfo, sizeof(paramValue), &paramValue, nullptr);
    return paramValue;
}

// get device info (cl_uint)
cl_uint GetDeviceInfoUInt(cl_device_id deviceId, cl_device_info deviceInfo) {
    cl_uint paramValue = 0;
    clGetDeviceInfo(deviceId, deviceInfo, sizeof(paramValue), &paramValue, nullptr);
    return paramValue;
}

// get device info (cl_ulong)
cl_uint GetDeviceInfoULong(cl_device_id deviceId, cl_device_info deviceInfo) {
    cl_ulong paramValue = 0;
    clGetDeviceInfo(deviceId, deviceInfo, sizeof(paramValue), &paramValue, nullptr);
    return paramValue;
}

// create openсl program from source
cl_program CreateProgram(cl_context context, cl_device_id deviceId, const char* programSource) {
    // create program and build program
    cl_int errcode = CL_SUCCESS;
    size_t programLength = strlen(programSource);

    // create program
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&programSource, &programLength, &errcode);
    // build program
    errcode = clBuildProgram(program, 1, &deviceId, nullptr, nullptr, nullptr);

    // check for errors
    if (errcode != CL_SUCCESS) {
        // get build log
        size_t buildLogSize;
        errcode = clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, nullptr, &buildLogSize);
        std::string buildLog(buildLogSize, ' ');
        errcode = clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, buildLog.data(), &buildLogSize);
        std::cerr << buildLog << std::endl;
        return nullptr;
    }

    // return result
    return program;
}

// create openсl image memory object
cl_mem CreateImage2D(cl_context context, size_t width, size_t height,
    cl_channel_order channel_order, cl_channel_type channel_type, cl_mem_flags flags) {
    // image description
    cl_image_desc imageDesc{};
    imageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
    imageDesc.image_width = width;
    imageDesc.image_height = height;
    imageDesc.image_depth = 0;
    imageDesc.image_array_size = 0;
    imageDesc.image_row_pitch = 0;
    imageDesc.image_slice_pitch = 0;
    imageDesc.num_mip_levels = 0;
    imageDesc.num_samples = 0;
    imageDesc.buffer = nullptr;

    // image format
    cl_image_format imageFormat{};
    imageFormat.image_channel_order = channel_order;
    imageFormat.image_channel_data_type = channel_type;

    // create image input
    cl_int errcode = CL_SUCCESS;
    cl_mem image = clCreateImage(context, flags, &imageFormat, &imageDesc, nullptr, &errcode);

    // show some error
    if (errcode != CL_SUCCESS) {
        std::cerr << "Failed to create texture 2d" << std::endl;
        return nullptr;
    }

    // return the result
    return image;
}

int main(int argc, char **argv) {
    // get platforms
    cl_uint numPlatforms = 0;
    clGetPlatformIDs(0, nullptr, &numPlatforms);
    std::vector<cl_platform_id> platformIds(numPlatforms);
    clGetPlatformIDs(numPlatforms, platformIds.data(), &numPlatforms);
    for(cl_platform_id& platformId : platformIds) {
        std::cout << "CL_PLATFORM_NAME:       " << GetPlatformInfoStr(platformId, CL_PLATFORM_NAME) << std::endl;
        std::cout << "CL_PLATFORM_VENDOR:     " << GetPlatformInfoStr(platformId, CL_PLATFORM_VENDOR) << std::endl;
        std::cout << "CL_PLATFORM_VERSION:    " << GetPlatformInfoStr(platformId, CL_PLATFORM_VERSION) << std::endl;
        std::cout << "CL_PLATFORM_PROFILE:    " << GetPlatformInfoStr(platformId, CL_PLATFORM_PROFILE) << std::endl;
        std::cout << "CL_PLATFORM_EXTENSIONS: " << GetPlatformInfoStr(platformId, CL_PLATFORM_EXTENSIONS) << std::endl;

        // get devices
        cl_uint numDevices = 0;
        clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices);
        std::vector<cl_device_id> deviceIds(numDevices);
        clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, numDevices, deviceIds.data(), nullptr);
        for(cl_device_id& deviceId : deviceIds) {
            std::cout << "\t" << "CL_DEVICE_NAME:                     " << GetDeviceInfoStr(deviceId, CL_DEVICE_NAME) << std::endl;
            std::cout << "\t" << "CL_DEVICE_VENDOR:                   " << GetDeviceInfoStr(deviceId, CL_DEVICE_VENDOR) << std::endl;
            std::cout << "\t" << "CL_DEVICE_VERSION:                  " << GetDeviceInfoStr(deviceId, CL_DEVICE_VERSION) << std::endl;
            std::cout << "\t" << "CL_DRIVER_VERSION:                  " << GetDeviceInfoStr(deviceId, CL_DRIVER_VERSION) << std::endl;
            std::cout << "\t" << "CL_DEVICE_PROFILE:                  " << GetDeviceInfoStr(deviceId, CL_DEVICE_PROFILE) << std::endl;
            std::cout << "\t" << "CL_DEVICE_MAX_COMPUTE_UNITS:        " << GetDeviceInfoUInt(deviceId, CL_DEVICE_MAX_COMPUTE_UNITS) << std::endl;
            std::cout << "\t" << "CL_DEVICE_IMAGE2D_MAX_HEIGHT:       " << GetDeviceInfoSizeT(deviceId, CL_DEVICE_IMAGE2D_MAX_HEIGHT) << std::endl;
            std::cout << "\t" << "CL_DEVICE_IMAGE2D_MAX_WIDTH:        " << GetDeviceInfoSizeT(deviceId, CL_DEVICE_IMAGE2D_MAX_WIDTH) << std::endl;
            std::cout << "\t" << "CL_DEVICE_MAX_WORK_GROUP_SIZE:      " << GetDeviceInfoSizeT(deviceId, CL_DEVICE_MAX_WORK_GROUP_SIZE) << std::endl;
            std::cout << "\t" << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE: " << GetDeviceInfoULong(deviceId, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) << std::endl;
            std::cout << "\t" << "CL_DEVICE_LOCAL_MEM_SIZE:           " << GetDeviceInfoULong(deviceId, CL_DEVICE_LOCAL_MEM_SIZE) << std::endl;
            std::cout << "\t" << "CL_DEVICE_MAX_MEM_ALLOC_SIZE:       " << GetDeviceInfoULong(deviceId, CL_DEVICE_MAX_MEM_ALLOC_SIZE) << std::endl;
        }
    }

    // error code
    cl_int errcode = CL_SUCCESS;
    // get default platform id
    cl_platform_id platformId = nullptr;
    errcode = clGetPlatformIDs(1, &platformId, nullptr);
    // get default device id (GPU)
    cl_device_id deviceId = nullptr;
    errcode = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, nullptr);
    // create context
    cl_context context = clCreateContext(nullptr, 1, &deviceId, nullptr, nullptr, &errcode);
    // create command queue
    cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, deviceId, nullptr, &errcode);
    // create program and build program
    cl_program programWriteImage = CreateProgram(context, deviceId, cCLProgram_WriteImage);
    // create kernel
    cl_kernel kernelWriteImage = clCreateKernel(programWriteImage, "WriteImage", &errcode);

    // create some images
    cl_mem imageInput = CreateImage2D(context, 2048, 2048, CL_RGBA, CL_UNSIGNED_INT8, CL_MEM_READ_ONLY);
    cl_mem imageOutput = CreateImage2D(context, 2048, 2048, CL_RGBA, CL_UNSIGNED_INT8, CL_MEM_READ_WRITE);

    // create image data
    cl_uint value = 5;
    auto imageInputData = new uint8_t[2048 * 2048 * 4]{};

    // write image data
    const static size_t imageInputOrigin[] = { 0, 0, 0 };
    const static size_t imageInputRegion[] = { 2048, 2048, 1 };
    errcode = clEnqueueWriteImage(commandQueue, imageInput, CL_TRUE, imageInputOrigin, imageInputRegion, 0, 0, imageInputData, 0, nullptr, nullptr);
        
    // execute kernel
    size_t globalWorkOffset[] = { 0, 0 };
    size_t globalWorkSize[] = { 2048, 2048 };
    size_t localWorkSize[] = { 32, 32 };
    errcode = clSetKernelArg(kernelWriteImage, 0, sizeof(imageOutput), &imageOutput);
    errcode = clSetKernelArg(kernelWriteImage, 1, sizeof(value), &value);
    errcode = clEnqueueNDRangeKernel(commandQueue, kernelWriteImage, 2, globalWorkOffset, globalWorkSize, localWorkSize, 0, nullptr, nullptr);
    errcode = clFlush(commandQueue);
    errcode = clFinish(commandQueue);

    // read image data
    const static size_t imageOutputOrigin[] = { 0, 0, 0 };
    const static size_t imageOutputRegion[] = { 2048, 2048, 1 };
    errcode = clEnqueueReadImage(commandQueue, imageOutput, CL_TRUE, imageOutputOrigin, imageOutputRegion, 0, 0, imageInputData, 0, nullptr, nullptr);

    // delete image data
    delete[] imageInputData;

    // release images
    errcode = clReleaseMemObject(imageOutput);
    errcode = clReleaseMemObject(imageInput);
    // release kernel
    errcode = clReleaseKernel(kernelWriteImage);
    // release program
    errcode = clReleaseProgram(programWriteImage);
    // release context
    errcode = clReleaseCommandQueue(commandQueue);
    // release context
    errcode = clReleaseContext(context);
    return 0;
}