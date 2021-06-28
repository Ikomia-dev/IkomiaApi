// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef OCL_H
#define OCL_H

#include <QDebug>

// New cl header
#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
#include <boost/align.hpp>

// OSX Implementation
#if defined(__MACH__) || defined (__APPLE__) || defined(MACOSX)
#include <OpenGL.h>
constexpr auto CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
// Linux/BSD implementation
#elif (defined(linux) || defined(__linux__) || defined(__linux)) || (defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
#include <GL/glx.h>
#include <cstdlib>
#define fopen_s(file, fileName, mode) ((*(file)) = fopen((fileName), (mode)))==NULL
constexpr auto CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
// Windows Implementation
#elif defined(_WIN32) || defined(__CYGWIN__)
#include <GL/gl.h>
#include <direct.h>
constexpr auto CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include "VolumeRenderGlobal.h"
#include "UtilsTools.hpp"

constexpr auto NVIDIA_PLATFORM = "NVIDIA";
constexpr auto AMD_PLATFORM = "AMD";
constexpr auto MESA_PLATFORM = "Clover";
constexpr auto INTEL_PLATFORM = "Intel";
constexpr auto APPLE_PLATFORM = "Apple";

constexpr int SDK_SUCCESS = 0;
constexpr int SDK_FAILURE = 1;

namespace ocl
{
    namespace utils
    {
        /**
         * getOpencLErrorCodeStr
         * global function to get corrosponding string for a error code
         * @param input Templated the error code
         * @return const char* the string for the error code
         */
        inline std::string  getOpenCLErrorCodeStr(int input)
        {
            int errorCode = (int)input;
            switch(errorCode)
            {
            case CL_DEVICE_NOT_FOUND:
                return "CL_DEVICE_NOT_FOUND";
            case CL_DEVICE_NOT_AVAILABLE:
                return "CL_DEVICE_NOT_AVAILABLE";
            case CL_COMPILER_NOT_AVAILABLE:
                return "CL_COMPILER_NOT_AVAILABLE";
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
            case CL_OUT_OF_RESOURCES:
                return "CL_OUT_OF_RESOURCES";
            case CL_OUT_OF_HOST_MEMORY:
                return "CL_OUT_OF_HOST_MEMORY";
            case CL_PROFILING_INFO_NOT_AVAILABLE:
                return "CL_PROFILING_INFO_NOT_AVAILABLE";
            case CL_MEM_COPY_OVERLAP:
                return "CL_MEM_COPY_OVERLAP";
            case CL_IMAGE_FORMAT_MISMATCH:
                return "CL_IMAGE_FORMAT_MISMATCH";
            case CL_IMAGE_FORMAT_NOT_SUPPORTED:
                return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
            case CL_BUILD_PROGRAM_FAILURE:
                return "CL_BUILD_PROGRAM_FAILURE";
            case CL_MAP_FAILURE:
                return "CL_MAP_FAILURE";
            case CL_MISALIGNED_SUB_BUFFER_OFFSET:
                return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
            case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
                return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
            case CL_INVALID_VALUE:
                return "CL_INVALID_VALUE";
            case CL_INVALID_DEVICE_TYPE:
                return "CL_INVALID_DEVICE_TYPE";
            case CL_INVALID_PLATFORM:
                return "CL_INVALID_PLATFORM";
            case CL_INVALID_DEVICE:
                return "CL_INVALID_DEVICE";
            case CL_INVALID_CONTEXT:
                return "CL_INVALID_CONTEXT";
            case CL_INVALID_QUEUE_PROPERTIES:
                return "CL_INVALID_QUEUE_PROPERTIES";
            case CL_INVALID_COMMAND_QUEUE:
                return "CL_INVALID_COMMAND_QUEUE";
            case CL_INVALID_HOST_PTR:
                return "CL_INVALID_HOST_PTR";
            case CL_INVALID_MEM_OBJECT:
                return "CL_INVALID_MEM_OBJECT";
            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
            case CL_INVALID_IMAGE_SIZE:
                return "CL_INVALID_IMAGE_SIZE";
            case CL_INVALID_SAMPLER:
                return "CL_INVALID_SAMPLER";
            case CL_INVALID_BINARY:
                return "CL_INVALID_BINARY";
            case CL_INVALID_BUILD_OPTIONS:
                return "CL_INVALID_BUILD_OPTIONS";
            case CL_INVALID_PROGRAM:
                return "CL_INVALID_PROGRAM";
            case CL_INVALID_PROGRAM_EXECUTABLE:
                return "CL_INVALID_PROGRAM_EXECUTABLE";
            case CL_INVALID_KERNEL_NAME:
                return "CL_INVALID_KERNEL_NAME";
            case CL_INVALID_KERNEL_DEFINITION:
                return "CL_INVALID_KERNEL_DEFINITION";
            case CL_INVALID_KERNEL:
                return "CL_INVALID_KERNEL";
            case CL_INVALID_ARG_INDEX:
                return "CL_INVALID_ARG_INDEX";
            case CL_INVALID_ARG_VALUE:
                return "CL_INVALID_ARG_VALUE";
            case CL_INVALID_ARG_SIZE:
                return "CL_INVALID_ARG_SIZE";
            case CL_INVALID_KERNEL_ARGS:
                return "CL_INVALID_KERNEL_ARGS";
            case CL_INVALID_WORK_DIMENSION:
                return "CL_INVALID_WORK_DIMENSION";
            case CL_INVALID_WORK_GROUP_SIZE:
                return "CL_INVALID_WORK_GROUP_SIZE";
            case CL_INVALID_WORK_ITEM_SIZE:
                return "CL_INVALID_WORK_ITEM_SIZE";
            case CL_INVALID_GLOBAL_OFFSET:
                return "CL_INVALID_GLOBAL_OFFSET";
            case CL_INVALID_EVENT_WAIT_LIST:
                return "CL_INVALID_EVENT_WAIT_LIST";
            case CL_INVALID_EVENT:
                return "CL_INVALID_EVENT";
            case CL_INVALID_OPERATION:
                return "CL_INVALID_OPERATION";
            case CL_INVALID_GL_OBJECT:
                return "CL_INVALID_GL_OBJECT";
            case CL_INVALID_BUFFER_SIZE:
                return "CL_INVALID_BUFFER_SIZE";
            case CL_INVALID_MIP_LEVEL:
                return "CL_INVALID_MIP_LEVEL";
            case CL_INVALID_GLOBAL_WORK_SIZE:
                return "CL_INVALID_GLOBAL_WORK_SIZE";
            case CL_INVALID_PROPERTY:
                return "CL_INVALID_PROPERTY";
            case CL_INVALID_IMAGE_DESCRIPTOR:
                return "CL_INVALID_IMAGE_DESCRIPTOR";
            case CL_INVALID_COMPILER_OPTIONS:
                return "CL_INVALID_COMPILER_OPTIONS";
            case CL_INVALID_LINKER_OPTIONS:
                return "CL_INVALID_LINKER_OPTIONS";
            case CL_INVALID_DEVICE_PARTITION_COUNT:
                return "CL_INVALID_DEVICE_PARTITION_COUNT";
            /*case CL_INVALID_PIPE_SIZE:
                return "CL_INVALID_PIPE_SIZE";
            case CL_INVALID_DEVICE_QUEUE:
                return "CL_INVALID_DEVICE_QUEUE";
            //case CL_INVALID_SPEC_ID:
                //return "CL_INVALID_SPEC_ID";
            //case CL_MAX_SIZE_RESTRICTION_EXCEEDED:
                //return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
            case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
                return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
            case CL_PLATFORM_NOT_FOUND_KHR:
                return "CL_PLATFORM_NOT_FOUND_KHR";
                //case CL_INVALID_PROPERTY_EXT:
                //    return "CL_INVALID_PROPERTY_EXT";
            case CL_DEVICE_PARTITION_FAILED_EXT:
                return "CL_DEVICE_PARTITION_FAILED_EXT";
            case CL_INVALID_PARTITION_COUNT_EXT:
                return "CL_INVALID_PARTITION_COUNT_EXT";*/
            default:
                return "unknown error code";
            }
        }

        inline size_t       shrRoundUp(size_t group_size, size_t global_size)
        {
            size_t r = global_size % group_size;
            if(r == 0)
                return global_size;
            else
                return global_size + group_size - r;
        }

        inline void*        aligned_malloc(size_t size, size_t align)
        {
            void *result;
            #ifdef _MSC_VER
            result = _aligned_malloc(size, align);
            #elif (defined(linux) || defined(__linux__) || defined(__linux)) || (defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
            result = boost::alignment::aligned_alloc(align, size);
            #else
            result = _mm_malloc(size, align);
            #endif
            return result;
        }
        inline void         aligned_free(void *ptr)
        {
            #ifdef _MSC_VER
                _aligned_free(ptr);
            #elif (defined(linux) || defined(__linux__) || defined(__linux)) || (defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
              free(ptr);
            #else
                _mm_free(ptr);
            #endif

        }
    }
    /**
     * buildProgramData
     * struct to build the kernel
     */
    struct buildProgramData
    {
        std::string     kernelName;     /**< kernelName name of the kernel */
        std::string     flagsFileName;  /**< flagFileName name of the file of flags */
        std::string     flagsStr;       /**< flagsStr flags string */
        std::string     binaryName;     /**< binaryName name of the binary */
        std::string     source;
        cl_device_id*   devices;        /**< devices array of device to build kernel for */
        int             deviceId;       /**< deviceId Id of the device to buid kernel for */

        /**
         * Constructor
         */
        buildProgramData()
        {
            kernelName = std::string("");
            flagsFileName = std::string("");
            flagsStr = std::string("");
            binaryName = std::string("");
        }
    };

    /**
     * KernelWorkGroupInfo
     * class implements the functionality to query
     * various Kernel Work Group related parameters
     */
    class KernelWorkGroupInfo
    {
        public:
            cl_ulong                localMemoryUsed;        /**< localMemoryUsed amount of local memory used by kernel */
            size_t                  kernelWorkGroupSize;    /**< kernelWorkGroupSize Supported WorkGroup size as per OpenCL Runtime*/
            std::array<size_t, 3>   compileWorkGroupSize;   /**< compileWorkGroupSize WorkGroup size as mentioned in kernel source */

            /**
             * Constructor
             */
            KernelWorkGroupInfo():
                localMemoryUsed(0),
                kernelWorkGroupSize(0)
            {
            }

            inline void setKernelWorkGroupInfo(cl::Kernel &kernel,cl::Device &device)
            {
                //Get Kernel Work Group Info
                kernelWorkGroupSize     = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
                localMemoryUsed         = kernel.getWorkGroupInfo<CL_KERNEL_LOCAL_MEM_SIZE>(device);
                compileWorkGroupSize    = kernel.getWorkGroupInfo<CL_KERNEL_COMPILE_WORK_GROUP_SIZE>(device);
            }
    };

    /**
     * COpenCLDeviceInfo
     * class implements the functionality to query
     * various Device related parameters
     */
    class COpenCLDeviceInfo
    {
        public :
            cl_device_type dType;                           /**< dType device type*/
            cl_uint venderId;                               /**< vendorId VendorId of device*/
            cl_uint maxComputeUnits;                        /**< maxComputeUnits maxComputeUnits of device*/
            cl_uint maxWorkItemDims;                        /**< maxWorkItemDims maxWorkItemDimensions VendorId of device*/
            std::vector<size_t> maxWorkItemSizes;           /**< maxWorkItemSizes maxWorkItemSizes of device*/
            size_t maxWorkGroupSize;                        /**< maxWorkGroupSize max WorkGroup Size of device*/
            cl_uint preferredCharVecWidth;                  /**< preferredCharVecWidth preferred Char VecWidth of device*/
            cl_uint preferredShortVecWidth;                 /**< preferredShortVecWidth preferred Short VecWidth of device*/
            cl_uint preferredIntVecWidth;                   /**< preferredIntVecWidth preferred Int VecWidth of device*/
            cl_uint preferredLongVecWidth;                  /**< preferredLongVecWidth preferred Long VecWidth of device*/
            cl_uint preferredFloatVecWidth;                 /**< preferredFloatVecWidth preferredFloatVecWidth of device*/
            cl_uint preferredDoubleVecWidth;                /**< preferredDoubleVecWidth preferred Double VecWidth of device*/
            cl_uint preferredHalfVecWidth;                  /**< preferredHalfVecWidth preferred Half VecWidth of device*/
            cl_uint nativeCharVecWidth;                     /**< nativeCharVecWidth native Char VecWidth of device*/
            cl_uint nativeShortVecWidth;                    /**< nativeShortVecWidth nativeShortVecWidth of device*/
            cl_uint nativeIntVecWidth;                      /**< nativeIntVecWidth nativeIntVecWidth of device*/
            cl_uint nativeLongVecWidth;                     /**< nativeLongVecWidth nativeLongVecWidth of device*/
            cl_uint nativeFloatVecWidth;                    /**< nativeFloatVecWidth native Float VecWidth of device*/
            cl_uint nativeDoubleVecWidth;                   /**< nativeDoubleVecWidth native Double VecWidth of device*/
            cl_uint nativeHalfVecWidth;                     /**< nativeHalfVecWidth native Half VecWidth of device*/
            cl_uint maxClockFrequency;                      /**< maxClockFrequency max Clock Frequency of device*/
            cl_uint addressBits;                            /**< addressBits address Bits of device*/
            cl_ulong maxMemAllocSize;                       /**< maxMemAllocSize max Mem Alloc Size of device*/
            cl_bool imageSupport;                           /**< imageSupport image Support of device*/
            cl_uint maxReadImageArgs;                       /**< maxReadImageArgs max ReadImage Args of device*/
            cl_uint maxWriteImageArgs;                      /**< maxWriteImageArgs max Write Image Args of device*/
            size_t image2dMaxWidth;                         /**< image2dMaxWidth image 2dMax Width of device*/
            size_t image2dMaxHeight;                        /**< image2dMaxHeight image 2dMax Height of device*/
            size_t image3dMaxWidth;                         /**< image3dMaxWidth image3d MaxWidth of device*/
            size_t image3dMaxHeight;                        /**< image3dMaxHeight image 3dMax Height of device*/
            size_t image3dMaxDepth;                         /**< image3dMaxDepth image 3dMax Depth of device*/
            size_t maxSamplers;                             /**< maxSamplers maxSamplers of device*/
            size_t maxParameterSize;                        /**< maxParameterSize maxParameterSize of device*/
            cl_uint memBaseAddressAlign;                    /**< memBaseAddressAlign memBase AddressAlign of device*/
            cl_uint minDataTypeAlignSize;                   /**< minDataTypeAlignSize minDataType AlignSize of device*/
            cl_device_fp_config singleFpConfig;             /**< singleFpConfig singleFpConfig of device*/
            cl_device_fp_config doubleFpConfig;             /**< doubleFpConfig doubleFpConfig of device*/
            cl_device_mem_cache_type globleMemCacheType;    /**< globleMemCacheType globleMem CacheType of device*/
            cl_uint globalMemCachelineSize;                 /**< globalMemCachelineSize globalMem Cacheline Size of device*/
            cl_ulong globalMemCacheSize;                    /**< globalMemCacheSize globalMem CacheSize of device*/
            cl_ulong globalMemSize;                         /**< globalMemSize globalMem Size of device*/
            cl_ulong maxConstBufSize;                       /**< maxConstBufSize maxConst BufSize of device*/
            cl_uint maxConstArgs;                           /**< maxConstArgs max ConstArgs of device*/
            cl_device_local_mem_type localMemType;          /**< localMemType local MemType of device*/
            cl_ulong localMemSize;                          /**< localMemSize localMem Size of device*/
            cl_bool errCorrectionSupport;                   /**< errCorrectionSupport errCorrectionSupport of device*/
            cl_bool hostUnifiedMem;                         /**< hostUnifiedMem hostUnifiedMem of device*/
            size_t timerResolution;                         /**< timerResolution timerResolution of device*/
            cl_bool endianLittle;                           /**< endianLittle endian Little of device*/
            cl_bool available;                              /**< available available of device*/
            cl_bool compilerAvailable;                      /**< compilerAvailable compilerAvailable of device*/
            cl_device_exec_capabilities execCapabilities;   /**< execCapabilities exec Capabilities of device*/
            cl_command_queue_properties queueProperties;    /**< queueProperties queueProperties of device*/
            cl_platform_id platform;                        /**< platform platform of device*/
            std::string name;                               /**< name name of device*/
            std::string vendorName;                         /**< venderName vender Name of device*/
            std::string driverVersion;                      /**< driverVersion driver Version of device*/
            std::string profileType;                        /**< profileType profile Type of device*/
            std::string deviceVersion;                      /**< deviceVersion device Version of device*/
            std::string openclCVersion;                     /**< openclCVersion opencl C Version of device*/
            std::string extensions;                         /**< extensions extensions of device*/

    #ifdef CL_VERSION_2_0
            cl_device_svm_capabilities svmcaps;	/**< SVM Capabilities of device*/
            cl_uint maxQueueSize;				/**< MAXIMUM QUEUE SIZE*/
    #endif

            /**
             * Constructor
             */
            COpenCLDeviceInfo()
            {
                dType = CL_DEVICE_TYPE_GPU;
                venderId = 0;
                maxComputeUnits = 0;
                maxWorkItemDims = 0;
                maxWorkGroupSize = 0;
                preferredCharVecWidth = 0;
                preferredShortVecWidth = 0;
                preferredIntVecWidth = 0;
                preferredLongVecWidth = 0;
                preferredFloatVecWidth = 0;
                preferredDoubleVecWidth = 0;
                preferredHalfVecWidth = 0;
                nativeCharVecWidth = 0;
                nativeShortVecWidth = 0;
                nativeIntVecWidth = 0;
                nativeLongVecWidth = 0;
                nativeFloatVecWidth = 0;
                nativeDoubleVecWidth = 0;
                nativeHalfVecWidth = 0;
                maxClockFrequency = 0;
                addressBits = 0;
                maxMemAllocSize = 0;
                imageSupport = CL_FALSE;
                maxReadImageArgs = 0;
                maxWriteImageArgs = 0;
                image2dMaxWidth = 0;
                image2dMaxHeight = 0;
                image3dMaxWidth = 0;
                image3dMaxHeight = 0;
                image3dMaxDepth = 0;
                maxSamplers = 0;
                maxParameterSize = 0;
                memBaseAddressAlign = 0;
                minDataTypeAlignSize = 0;
                singleFpConfig = CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN;
                doubleFpConfig = CL_FP_FMA |
                                 CL_FP_ROUND_TO_NEAREST |
                                 CL_FP_ROUND_TO_ZERO |
                                 CL_FP_ROUND_TO_INF |
                                 CL_FP_INF_NAN |
                                 CL_FP_DENORM;
                globleMemCacheType = CL_NONE;
                globalMemCachelineSize = CL_NONE;
                globalMemCacheSize = 0;
                globalMemSize = 0;
                maxConstBufSize = 0;
                maxConstArgs = 0;
                localMemType = CL_LOCAL;
                localMemSize = 0;
                errCorrectionSupport = CL_FALSE;
                hostUnifiedMem = CL_FALSE;
                timerResolution = 0;
                endianLittle = CL_FALSE;
                available = CL_FALSE;
                compilerAvailable = CL_FALSE;
                execCapabilities = CL_EXEC_KERNEL;
                queueProperties = 0;
                platform = 0;
                name = "";
                vendorName = "";
                driverVersion = "";
                profileType = "";
                deviceVersion = "";
                openclCVersion = "";
                extensions = "";
            }

            /**
             * Destructor
             */
            ~COpenCLDeviceInfo()
            {
            }

            inline int setDeviceInfo(cl::Device& device)
            {
                cl_int status = CL_SUCCESS;
                //Get device type
                status = device.getInfo(CL_DEVICE_TYPE, &dType);
                //Get vender ID
                status = device.getInfo(CL_DEVICE_VENDOR_ID, &venderId);
                //Get max compute units
                status = device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &maxComputeUnits);
                //Get max work item dimensions
                status = device.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDims);
                //Get max work item sizes
                maxWorkItemSizes = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
                // Maximum work group size
                status = device.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
                // Preferred vector sizes of all data types
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, &preferredCharVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, &preferredShortVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, &preferredIntVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, &preferredLongVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, &preferredFloatVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, &preferredDoubleVecWidth);
                status = device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, &preferredHalfVecWidth);
                // Clock frequency
                status = device.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &maxClockFrequency);
                // Address bits
                status = device.getInfo(CL_DEVICE_ADDRESS_BITS, &addressBits);
                // Maximum memory alloc size
                status = device.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxMemAllocSize);
                // Image support
                status = device.getInfo(CL_DEVICE_IMAGE_SUPPORT, &imageSupport);
                // Maximum read image arguments
                status = device.getInfo(CL_DEVICE_MAX_READ_IMAGE_ARGS, &maxReadImageArgs);
                // Maximum write image arguments
                status = device.getInfo(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, &maxWriteImageArgs);
                // 2D image and 3D dimensions
                status = device.getInfo(CL_DEVICE_IMAGE2D_MAX_WIDTH, &image2dMaxWidth);
                status = device.getInfo(CL_DEVICE_IMAGE2D_MAX_HEIGHT, &image2dMaxHeight);
                status = device.getInfo(CL_DEVICE_IMAGE3D_MAX_WIDTH, &image3dMaxWidth);
                status = device.getInfo(CL_DEVICE_IMAGE3D_MAX_HEIGHT, &image3dMaxHeight);
                status = device.getInfo(CL_DEVICE_IMAGE3D_MAX_DEPTH, &image3dMaxDepth);
                // Maximum samplers
                status = device.getInfo(CL_DEVICE_MAX_SAMPLERS, &maxSamplers);
                // Maximum parameter size
                status = device.getInfo(CL_DEVICE_MAX_PARAMETER_SIZE, &maxParameterSize);
                // Memory base address align
                status = device.getInfo(CL_DEVICE_MEM_BASE_ADDR_ALIGN, &memBaseAddressAlign);
                // Minimum data type align size
                status = device.getInfo(CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, &minDataTypeAlignSize);
                // Single precision floating point configuration
                status = device.getInfo(CL_DEVICE_SINGLE_FP_CONFIG, &singleFpConfig);
                // Double precision floating point configuration
                status = device.getInfo(CL_DEVICE_DOUBLE_FP_CONFIG, &doubleFpConfig);
                // Global memory cache type
                status = device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, &globleMemCacheType);
                // Global memory cache line size
                status = device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, &globalMemCachelineSize);
                // Global memory cache size
                status = device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, &globalMemCacheSize);
                // Global memory size
                status = device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &globalMemSize);
                // Maximum constant buffer size
                status = device.getInfo(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, &maxConstBufSize);
                // Maximum constant arguments
                status = device.getInfo(CL_DEVICE_MAX_CONSTANT_ARGS, &maxConstArgs);
                // Local memory type
                status = device.getInfo(CL_DEVICE_LOCAL_MEM_TYPE, &localMemType);
                // Local memory size
                status = device.getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &localMemSize);
                // Error correction support
                status = device.getInfo(CL_DEVICE_ERROR_CORRECTION_SUPPORT, &errCorrectionSupport);
                // Profiling timer resolution
                status = device.getInfo(CL_DEVICE_PROFILING_TIMER_RESOLUTION, &timerResolution);
                // Endian little
                status = device.getInfo(CL_DEVICE_ENDIAN_LITTLE, &endianLittle);
                // Device available
                status = device.getInfo(CL_DEVICE_AVAILABLE, &available);
                // Device compiler available
                status = device.getInfo(CL_DEVICE_COMPILER_AVAILABLE, &compilerAvailable);
                // Device execution capabilities
                status = device.getInfo(CL_DEVICE_EXECUTION_CAPABILITIES, &execCapabilities);
                // Device queue properities
                status = device.getInfo(CL_DEVICE_QUEUE_PROPERTIES, &queueProperties);
                // Platform
                status = device.getInfo(CL_DEVICE_PLATFORM, &platform);
                // Device name
                name = device.getInfo<CL_DEVICE_NAME>(&status);
                // Vender name
                status = device.getInfo(CL_DEVICE_VENDOR, &vendorName);
                // Driver name
                status = device.getInfo(CL_DRIVER_VERSION, &driverVersion);
                // Device profile
                status = device.getInfo(CL_DEVICE_PROFILE, &profileType);
                // Device version
                status = device.getInfo(CL_DEVICE_VERSION, &deviceVersion);
                // Device extensions
                status = device.getInfo(CL_DEVICE_EXTENSIONS, &extensions);
                // Device parameters of OpenCL 1.1 Specification
    #ifdef CL_VERSION_1_1
                std::string deviceVerStr(deviceVersion);
                size_t vStart = deviceVerStr.find(" ", 0);
                size_t vEnd = deviceVerStr.find(" ", vStart + 1);
                std::string vStrVal = deviceVerStr.substr(vStart + 1, vEnd - vStart - 1);
                if(vStrVal.compare("1.0") > 0)
                {
                    // Native vector sizes of all data types
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, &nativeCharVecWidth);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, &nativeShortVecWidth);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, &venderId);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, &nativeLongVecWidth);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, &nativeFloatVecWidth);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, &nativeDoubleVecWidth);
                    status = device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, &nativeHalfVecWidth);
                    // Host unified memory
                    status = device.getInfo(CL_DEVICE_HOST_UNIFIED_MEMORY, &hostUnifiedMem);
                    // Device OpenCL C version
                    status = device.getInfo(CL_DEVICE_OPENCL_C_VERSION, &openclCVersion);
                }
    #endif
    #ifdef CL_VERSION_2_0
                if (checkOpenCL2_XCompatibility())
                {
                    status = device.getInfo(CL_DEVICE_SVM_CAPABILITIES, &svmcaps);
                    status = device.getInfo(CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE, &maxQueueSize);
                }
    #endif
                return status;
            }

            /**
             * detectSVM
             * Check if the device supports Shared virtual memory(SVM)
             * @return bool
             */
            inline bool detectSVM()
            {
                bool svmSupport = false;

            #ifdef CL_VERSION_2_0
                if (this->svmcaps & (CL_DEVICE_SVM_COARSE_GRAIN_BUFFER | CL_DEVICE_SVM_FINE_GRAIN_BUFFER | CL_DEVICE_SVM_FINE_GRAIN_SYSTEM | CL_DEVICE_SVM_ATOMICS))
                {
                    svmSupport = true;
                }
            #endif

                return svmSupport;
            }

            /**
             * detectOpenCL2_xCompatibility
             * Check if the device supports OpenCL 2.x
             * @return @bool
             */
            inline bool checkOpenCL2_XCompatibility()
            {
                bool isOpenCL2_XSupported = false;

                /*int majorRev, minorRev;
                if (sscanf(this->deviceVersion.c_str(), "OpenCL %d.%d", &majorRev, &minorRev) == 2)
                {
                  if (majorRev >= 2) {
                    isOpenCL2_XSupported = true;
                  }
                }*/

                return isOpenCL2_XSupported;
            }
    };

    class COpenCLFile
    {
        public:
            /**
             *Default constructor
             */
            COpenCLFile(): m_source("") {}

            /**
             * Destructor
             */
            ~COpenCLFile() {}

            /**
             * Opens the CL program file
             * @return true if success else false
             */
            inline bool open(const char* /*fileName*/)
            {
                /*size_t      size;
                char*       str;
                // Open file stream
                std::fstream f(fileName, (std::fstream::in | std::fstream::binary));
                // Check if we have opened file stream
                if (f.is_open())
                {
                    size_t  sizeFile;
                    // Find the stream size
                    f.seekg(0, std::fstream::end);
                    size = sizeFile = (size_t)f.tellg();
                    f.seekg(0, std::fstream::beg);
                    str = new char[size + 1];
                    if (!str)
                    {
                        f.close();
                        return  false;
                    }
                    // Read file
                    f.read(str, sizeFile);
                    f.close();
                    str[size] = '\0';
                    m_source  = str;
                    delete[] str;
                    return true;
                }*/
                return false;
            }

            /**
             * writeBinaryToFile
             * @param fileName Name of the file
             * @param binary char binary array
             * @param numBytes number of bytes
             * @return true if success else false
             */
            inline int writeBinaryToFile(const char* /*fileName*/, const char* /*binary*/, size_t /*numBytes*/)
            {
                /*FILE *output = NULL;

                if(fopen_s(&output, fileName, "wb"))
                {
                    return SDK_FAILURE;
                }

                fwrite(binary, sizeof(char), numBytes, output);
                fclose(output);*/

                return SDK_SUCCESS;
            }


            /**
             * readBinaryToFile
             * @param fileName name of file
             * @return true if success else false
             */
            inline int readBinaryFromFile(const char* /*fileName*/)
            {
                /*FILE * input = NULL;
                size_t size = 0;
                char* binary = NULL;

                if(fopen_s(&input, fileName, "rb"))
                {
                    return SDK_FAILURE;
                }

                fseek(input, 0L, SEEK_END);
                size = ftell(input);
                rewind(input);
                binary = (char*)malloc(size);

                if(binary == NULL)
                {
                    return SDK_FAILURE;
                }

                fread(binary, sizeof(char), size, input);
                fclose(input);
                m_source.assign(binary, size);
                free(binary);*/
                return SDK_SUCCESS;
            }


            /**
             * Replaces Newline with spaces
             */
            inline void replaceNewlineWithSpaces()
            {
                auto pos = m_source.find_first_of('\n', 0);
                while(pos != std::string::npos)
                {
                    m_source.replace(pos, 1, " ");
                    pos = m_source.find_first_of('\n', pos + 1);
                }
                pos = m_source.find_first_of('\r', 0);
                while(pos != std::string::npos)
                {
                    m_source.replace(pos, 1, " ");
                    pos = m_source.find_first_of('\r', pos + 1);
                }
            }

            /**
             * source
             * Returns a pointer to the string object with the source code
             */
            inline const std::string&  source() const
            {
                return m_source;
            }

        private:
            /**
             * Disable copy constructor
             */
            COpenCLFile(const COpenCLFile&);

            /**
             * Disable operator=
             */
            COpenCLFile& operator=(const COpenCLFile&);

            std::string     m_source;    //!< source code of the CL program
    };

    class VOLUMERENDERSHARED_EXPORT COpenCL
    {
        public:

            COpenCL()
            {
                m_deviceType = CL_DEVICE_TYPE_GPU;
                m_bInteropCLGL = false;
            }
            COpenCL(cl_device_type deviceType)
            {
                m_deviceType = deviceType;
                m_bInteropCLGL = false;
            }
            COpenCL(bool bInteropCLGL)
            {
                m_deviceType = CL_DEVICE_TYPE_GPU;
                m_bInteropCLGL = bInteropCLGL;
            }
            COpenCL(cl_device_type deviceType, bool bInteropCLGL)
            {
                m_deviceType = deviceType;
                m_bInteropCLGL = bInteropCLGL;
            }
            ~COpenCL()
            {
                clean();
            }

            inline short                init(const std::string &name = "")
            {
                try
                {
                    initPlatform(name);
                    initDeviceList();
                    initContextList();
                    initCommandQueueList();
                }
                catch(cl::Error& e)
                {
                    throw e;
                    return SDK_FAILURE;
                }
                return SDK_SUCCESS;
            }
            inline short                buildProgram(cl::Program &program, const buildProgramData &buildData)
            {
                COpenCLFile kernelFile;
                std::string kernelPath;
                cl::Context context = m_contextList.back();

                try
                {
                    if(buildData.binaryName.size() != 0)
                    {
                        kernelPath.append(buildData.binaryName.c_str());
                        if(kernelFile.readBinaryFromFile(kernelPath.c_str()))
                        {
                            auto str = "Failed to load kernel file : " + kernelPath;
                            throw cl::Error(CL_INVALID_KERNEL, str.c_str());
                        }

                        //const char * binary = kernelFile.source().c_str();
                        //size_t binarySize = kernelFile.source().size();
                        //cl::Program::Binaries binaries(1, std::make_pair(binary, binarySize));
                        //program = cl::Program(context, binaries);
                    }
                    else if(buildData.kernelName.size() != 0)
                    {
                        kernelPath.append(buildData.kernelName.c_str());
                        if(!kernelFile.open(kernelPath.c_str()))
                        {
                            auto str = "Failed to load kernel file : " + kernelPath;
                            throw cl::Error(CL_INVALID_KERNEL, str.c_str());
                        }

                        cl::Program::Sources source{kernelFile.source()};
                        program = cl::Program(context, source);
                    }
                    else
                    {
                        cl::Program::Sources source{buildData.source};
                        program = cl::Program(context, source);
                    }
                    std::string flagsStr = std::string(buildData.flagsStr.c_str());
                    // Get additional options
                    if(buildData.flagsFileName.size() != 0)
                    {
                        COpenCLFile flagsFile;
                        std::string flagsPath;
                        flagsPath.append(buildData.flagsFileName.c_str());
                        if(!flagsFile.open(flagsPath.c_str()))
                        {
                            auto str = "Failed to load flags file: " + flagsPath;
                            throw cl::Error(CL_INVALID_BUILD_OPTIONS, str.c_str());
                        }

                        flagsFile.replaceNewlineWithSpaces();
                        const char * flags = flagsFile.source().c_str();
                        flagsStr.append(flags);
                    }
                    if(flagsStr.size() != 0)
                    {
                        Utils::print(std::string("Build Options are : ") + flagsStr, QtDebugMsg);
                    }

                    //create a cl program executable for all the devices specified
                    program.build(flagsStr.c_str());
                }
                catch(cl::Error& e)
                {
                    Utils::print(e.what() + std::string("(") + std::to_string(e.err()) + std::string(")"), QtCriticalMsg);
                    std::string val = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_device);
                    Utils::print("Log:\n" + val, QtCriticalMsg);
                    return SDK_FAILURE;
                }
                return SDK_SUCCESS;
            }
            inline void                 initProgram(const buildProgramData& buildData)
            {
                cl::Program program;
                buildProgram(program, buildData);
                m_programList.push_back(program);
            }
            inline void                 initKernelList(size_t id, const std::string& name)
            {
                cl::Kernel kernel(m_programList.back(), name.c_str());
                m_kernelMap.insert(std::make_pair(id, kernel));
            }
            inline void                 clean()
            {
                cleanProgramAndKernel();
                m_commandQueueList.clear();
                m_contextList.clear();
                m_deviceList.clear();
            }
            inline void                 cleanProgramAndKernel()
            {
                m_kernelMap.clear();
                m_programList.clear();
            }

            inline cl::Context&         getContext(size_t id = 0)
            {
                assert(id < m_contextList.size());
                return m_contextList.at(id);
            }
            inline cl::CommandQueue&	getQueue(size_t id = 0)
            {
                assert(id < m_commandQueueList.size());
                return m_commandQueueList.at(id);
            }
            inline cl::Kernel&          getKernel(size_t id = 0)
            {
                assert(m_kernelMap.find(id) != m_kernelMap.end());
                return m_kernelMap.at(id);
            }
            inline cl::Device&          getDevice(size_t id = 0)
            {
                return m_deviceList[id];
            }
            inline COpenCLDeviceInfo&	getDeviceInfo()
            {
                return m_deviceInfo;
            }
            inline size_t               getNbKernel()
            {
                return m_kernelMap.size();
            }

        private:

            inline void                initPlatform(const std::string& name = "")
            {
                cl_int err;

                if(!name.empty())
                {
                    m_platformName = name;
                    m_platform = getPlatform(name, err);

                    if (err == CL_SUCCESS)
                        return;
                }

                m_platform = getPlatform(NVIDIA_PLATFORM, err);
                if (err == CL_SUCCESS)
                {
                    m_platformName = NVIDIA_PLATFORM;
                    return;
                }

                m_platform = getPlatform(AMD_PLATFORM, err);
                if (err == CL_SUCCESS)
                {
                    m_platformName = AMD_PLATFORM;
                    return;
                }

                m_platform = getPlatform(INTEL_PLATFORM, err);
                if (err == CL_SUCCESS)
                {
                    m_platformName = INTEL_PLATFORM;
                    return;
                }

                m_platform = getPlatform(APPLE_PLATFORM, err);
                if (err == CL_SUCCESS)
                {
                    m_platformName = APPLE_PLATFORM;
                    return;
                }

                m_platform = getPlatform(MESA_PLATFORM, err);
                if (err == CL_SUCCESS)
                {
                    m_platformName = MESA_PLATFORM;
                    return;
                }

                throw cl::Error(err, "No CL platforms detected.");
            }
            inline void                initDeviceList()
            {
                try
                {
                    m_platform.getDevices(m_deviceType, &m_deviceList);
                }
                catch(cl::Error& /*e*/)
                {
                    Utils::print("No GPU detected.", QtCriticalMsg);
                    // Try CPU
                    m_deviceType = CL_DEVICE_TYPE_CPU;
                    m_platform.getDevices(m_deviceType, &m_deviceList);
                }


                if(m_deviceList.size() == 0)
                {
                    throw cl::Error(CL_DEVICE_NOT_FOUND, "No CL devices found for current platform");
                }

                for(auto& it : m_deviceList)
                {
                    m_deviceInfo.setDeviceInfo(it);
                    m_device = it;
                    Utils::print(std::string("Device Name: ") + m_deviceInfo.name, QtInfoMsg);
                    Utils::print(std::string("OpenCL version: ") + m_deviceInfo.openclCVersion, QtInfoMsg);
                    Utils::print(std::string("Max CU: ") + std::to_string(m_deviceInfo.maxComputeUnits), QtInfoMsg);
                }
            }
            inline void                initCLContext()
            {
                m_deviceInfo.setDeviceInfo(m_device);
                cl::Context context(m_device);
                m_contextList.push_back(context);
            }
            inline void                initCLGLContext()
            {
#if defined(__APPLE__) || defined(__MACOSX)
                // Apple
                // Get current OpenGL context
                CGLContextObj gl_context = CGLGetCurrentContext();
                // Get shared group associated with current GL context (list all compatible devices)
                CGLShareGroupObj cgl_share_group = CGLGetShareGroup(gl_context);
                // Create context properties
                cl_context_properties cps[] = {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                    (cl_context_properties)cgl_share_group,
                    0
                };

                cl_int error = 0;
                if(m_deviceType == CL_DEVICE_TYPE_GPU) // If gpu was found
                {
                    // Create CL context for GPU
                    cl_context cl_gl_context = clCreateContextFromType(cps, CL_DEVICE_TYPE_GPU, 0, 0, &error);

                    if(!cl_gl_context)
                        throw cl::Error(error);

                    // Retrieve GPU associated with current display
                    cl_device_id clDeviceId;
                    /*size_t devicesSize = 0;
                    error = clGetGLContextInfoAPPLE(cl_gl_context, gl_context, CL_CGL_DEVICES_FOR_SUPPORTED_VIRTUAL_SCREENS_APPLE, 0, nullptr, &devicesSize);
                    int numDevices = int(devicesSize / sizeof(cl_device_id));
                    if(numDevices == 0)
                        throw cl::Error(error, "No CLGL devices");
                    cl_device_id* clDevices = new cl_device_id[numDevices];
                    error = clGetGLContextInfoAPPLE(cl_gl_context, gl_context, CL_CGL_DEVICES_FOR_SUPPORTED_VIRTUAL_SCREENS_APPLE, numDevices*sizeof(cl_device_id), clDevices, nullptr);
                    clDeviceId = clDevices[0];
                    for(int i=0; i<numDevices; ++i)
                    {
                        m_device = cl::Device(clDevices[i]);
                        m_deviceInfo.setDeviceInfo(m_device);
                        Utils::print(std::string("Screen device: ") + m_deviceInfo.name, QtDebugMsg);
                    }*/
                    error = clGetGLContextInfoAPPLE(cl_gl_context, gl_context, CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE, sizeof(cl_device_id), &clDeviceId, nullptr);
                    if(error != CL_SUCCESS)
                        Utils::print("Failed to get OpenCL device for current screen: " + error, QtWarningMsg);
                    else
                    {
                        // Create a device with this GPU
                        m_device = cl::Device(clDeviceId);
                        m_deviceInfo.setDeviceInfo(m_device);
                    }
                    // Create a CL-GL Context for this GPU
                    if(checkExtnAvailability(m_device, CL_GL_SHARING_EXT))
                    {
                        Utils::print(std::string("OpenCL Device Name: ") + m_deviceInfo.name, QtInfoMsg);
                        cl::Context context(m_device, cps);
                        m_contextList.push_back(context);
                    }
                    else
                        throw cl::Error(CL_DEVICE_NOT_FOUND, "No CLGL devices found for current platform.");
                }
                else // CL_DEVICE_TYPE_CPU
                {
                    if(checkExtnAvailability(m_device, CL_GL_SHARING_EXT))
                    {
                        Utils::print(std::string("OpenCL Device Name: ") + m_deviceInfo.name, QtInfoMsg);
                        cl::Context context(m_device, cps);
                        m_contextList.push_back(context);
                    }
                    else
                        throw cl::Error(CL_DEVICE_NOT_FOUND, "No CLGL devices found for current platform.");
                }
#else
#ifdef _WIN32
                // Windows
                cl_context_properties cps[] = {
                    CL_GL_CONTEXT_KHR,
                    (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR,
                    (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM,
                    (cl_context_properties)m_platform(),
                    0
                };
#else
                // Linux
                cl_context_properties cps[] = {
                    CL_GL_CONTEXT_KHR,
                    (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR,
                    (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM,
                    (cl_context_properties)m_platform(),
                    0
                };

                Utils::print(std::string("Current GLX context: ") + std::to_string(reinterpret_cast<intptr_t>(glXGetCurrentContext())), QtDebugMsg);
                Utils::print(std::string("Current GLX Display: ") + std::to_string(reinterpret_cast<intptr_t>(glXGetCurrentDisplay())), QtDebugMsg);
#endif
                // Lookup current OpenCL device for current OpenGL context
                cl_device_id gpuId;
                size_t size_ret;
                cl_int ret;

                // Fix AMD issue for Windows
                if(m_platformName != AMD_PLATFORM)
                {
                    // Check if platform supports opengl context sharing
                    if(!checkExtnAvailability(m_platform, CL_GL_SHARING_EXT))
                        throw cl::Error(CL_DEVICE_NOT_FOUND, "Current platform doesn't support OpenGL sharing.");
                }

                //cl_int ret = clGetGLContextInfoKHR(cps, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &gpuId, &size_ret);
                // load clGetGLContextInfoKHR() extension function
                clGetGLContextInfoKHR_fn pclGetGLContextInfoKHR = reinterpret_cast<clGetGLContextInfoKHR_fn>(clGetExtensionFunctionAddressForPlatform(m_platform(), "clGetGLContextInfoKHR"));
                if(pclGetGLContextInfoKHR)
                    ret = pclGetGLContextInfoKHR(cps, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &gpuId, &size_ret);

                if(ret != CL_SUCCESS || size_ret == 0)
                    throw cl::Error(CL_DEVICE_NOT_FOUND, "No CLGL devices found for current platform.");

                m_device = cl::Device(gpuId, false);
                m_deviceInfo.setDeviceInfo(m_device);
                Utils::print(std::string("Device Name: ") + m_deviceInfo.name, QtInfoMsg);

                if(checkExtnAvailability(m_device, CL_GL_SHARING_EXT))
                {
                    cl::Context context(m_device, cps);
                    m_contextList.push_back(context);
                }
                else
                    throw cl::Error(CL_DEVICE_NOT_FOUND, "Current device doesn't support OpenGL sharing.");

                // We need to check if there is more than one device first
                // If more than one CL device find out which one is associated with GL context

                /*if(m_deviceList.size() > 1)
                {
                    bool bInteropDevice = false;
                    for(auto&& it : m_deviceList)
                    {
                        if(checkExtnAvailability(it, CL_GL_SHARING_EXT))
                        {
                            m_device = it;
                            bInteropDevice = true;
                            m_deviceInfo.setDeviceInfo(it);
                            Utils::print("Device Name: " + m_deviceInfo.name, QtCriticalMsg);
                            break;
                        }
                    }

                    if(bInteropDevice == false)
                        throw cl::Error(CL_DEVICE_NOT_FOUND, "No CLGL devices found for current platform.");

                }
                else
                {
                    //cl_device_id devices[32];
                    //size_t size;
                    //clGetGLContextInfoKHR_fn pclGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
                            //clGetExtensionFunctionAddressForPlatform(m_platform, "clGetGLContextInfoKHR");
                    //pclGetGLContextInfoKHR(cps, CL_DEVICES_FOR_GL_CONTEXT_KHR, 32*sizeof(cl_device_id), devices, &size);
                    //int count = size / sizeof(cl_device_id);

                    if(checkExtnAvailability(m_device, CL_GL_SHARING_EXT))
                    {
                        cl::Context context(m_device, cps);
                        m_contextList.push_back(context);
                    }
                    else
                        throw cl::Error(CL_DEVICE_NOT_FOUND, "No CLGL devices found for current platform.");
                }*/
#endif
            }
            inline void                initContextList()
            {
                if(m_bInteropCLGL)
                    initCLGLContext();
                else
                    initCLContext();
            }
            inline void                initCommandQueueList()
            {
                //cl_command_queue_properties prop = 0;
                for(const auto& it : m_contextList)
                {
                    cl::CommandQueue commandQueue(it);
                    m_commandQueueList.push_back(commandQueue);
                }
            }

            inline std::string         strErrCode(const cl_int& error)
            {
                return std::string("(") + std::to_string(error) + std::string(")");
            }

            inline cl::Platform        getPlatform(std::string pName, cl_int &error)
            {
                using namespace cl;

                Platform ret_val;
                error = 0;
                try
                {
                    // Get available platforms
                    std::vector<Platform> platforms;
                    Platform::get(&platforms);
                    bool isFound = false;
                    for(const auto& it : platforms)
                    {
                        std::string temp = it.getInfo<CL_PLATFORM_NAME>();
                        if (temp.find(pName)!=std::string::npos)
                        {
                            ret_val = it;
                            Utils::print(std::string("Found platform: ") + temp, QtInfoMsg);
                            isFound = true;
                            break;
                        }
                    }
                    if (!isFound)
                    {
                        // Going towards + numbers to avoid conflict with OpenCl error codes
                        Utils::print(std::string("Requested platform (") + pName + std::string(") was not found."), QtInfoMsg);
                        error = +1; // requested platform not found
                    }
                }
                catch(cl::Error& err)
                {
                    Utils::print(err.what() + strErrCode(err.err()), QtCriticalMsg);
                    error = err.err();
                }
                return ret_val;
            }

            inline bool                checkExtnAvailability(cl::Device pDevice, std::string pName)
            {
                bool ret_val = true;
                try
                {
                    // find extensions required
                    std::string exts = pDevice.getInfo<CL_DEVICE_EXTENSIONS>();
                    auto pos = exts.find(pName);
                    if(pos != std::string::npos)
                        ret_val = true;
                    else
                        ret_val = false;
                }
                catch (cl::Error& err)
                {
                    Utils::print(err.what() + strErrCode(err.err()), QtCriticalMsg);
                }
                return ret_val;
            }
            inline bool                checkExtnAvailability(cl::Platform pPlatform, std::string pName)
            {
                bool ret_val = true;
                try
                {
                    // find extensions required
                    std::string exts = pPlatform.getInfo<CL_PLATFORM_EXTENSIONS>();
                    auto pos = exts.find(pName);
                    if(pos != std::string::npos)
                        ret_val = true;
                    else
                        ret_val = false;
                }
                catch (cl::Error& err)
                {
                    Utils::print(err.what() + strErrCode(err.err()), QtCriticalMsg);
                }
                return ret_val;
            }

        private:

            std::vector<cl::Device>         m_deviceList;
            std::vector<cl::Context>        m_contextList;
            std::vector<cl::CommandQueue>	m_commandQueueList;
            std::vector<cl::Program>		m_programList;
            std::map<size_t, cl::Kernel>    m_kernelMap;
            std::string                     m_platformName = "";

            COpenCLDeviceInfo				m_deviceInfo;

            cl_device_type					m_deviceType;

            cl::Platform                    m_platform;
            cl::Device                      m_device;

            bool							m_bInteropCLGL = false;
    };
}
#endif
