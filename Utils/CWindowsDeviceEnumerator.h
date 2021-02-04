#ifndef CWINDOWSDEVICEENUMERATOR_H
#define CWINDOWSDEVICEENUMERATOR_H

/// @cond INTERNAL

#if defined(_WIN32)
#include <Windows.h>
#include <dshow.h>
#include "UtilsGlobal.hpp"

#pragma comment(lib, "strmiids")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

#include <map>
#include <string>

struct Device {
    int id; // This can be used to open the device in OpenCV
    std::string devicePath;
    std::string deviceName; // This can be used to show the devices to the user
};

class UTILSSHARED_EXPORT CWindowsDeviceEnumerator
{

    public:

        CWindowsDeviceEnumerator() = default;
        std::map<int, Device> getDevicesMap(const GUID deviceClass);
        std::map<int, Device> getVideoDevicesMap();
        std::map<int, Device> getAudioDevicesMap();

    private:

        std::string ConvertBSTRToMBS(BSTR bstr);
        std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);

};
#endif

/// @endcond

#endif // CWINDOWSDEVICEENUMERATOR_H
