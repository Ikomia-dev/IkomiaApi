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

#include "CMemoryInfo.h"
#include <QProcess>

#ifdef _WIN32
#include <windows.h>
#endif

CMemoryInfo::CMemoryInfo()
{
#ifdef _WIN32
    getWindowsMemoryInfo();
#elif __linux__
    getLinuxMemoryInfo();
#elif __APPLE__
    getAppleMemoryInfo();
#else
    #error CMemoryInfo: platform not supported
#endif
}

unsigned long long CMemoryInfo::totalMemory() const
{
    return m_total;
}

unsigned long long CMemoryInfo::availableMemory() const
{
    return m_available;
}

int CMemoryInfo::memoryLoad() const
{
    return m_memoryLoad;
}

void CMemoryInfo::getWindowsMemoryInfo()
{
#ifdef _WIN32
    MEMORYSTATUSEX memStatus;
    ZeroMemory(&memStatus, sizeof(MEMORYSTATUSEX));
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memStatus))
    {
        m_memoryLoad = memStatus.dwMemoryLoad;
        m_total = memStatus.ullTotalPhys;
        m_available = memStatus.ullAvailPhys;
        m_totalPageFile = memStatus.ullTotalPageFile;
        m_availablePageFile = memStatus.ullAvailPageFile;
    }
#endif
}

void CMemoryInfo::getLinuxMemoryInfo()
{
#ifdef __linux__    
    QProcess p;
    QStringList arguments;

    arguments << "/MemTotal/ {print $2} " << "/proc/meminfo";
    p.start("awk", arguments);
    p.waitForFinished();
    QString value = p.readAllStandardOutput();
    m_total = value.toULongLong() * 1024;
    p.close();
    arguments.clear();

    arguments << "/MemFree/ {print $2} " << "/proc/meminfo";
    p.start("awk", arguments);
    p.waitForFinished();
    value = p.readAllStandardOutput();
    m_available = value.toULongLong() * 1024;
    p.close();
    arguments.clear();

    arguments << "/Active:/ {print $2} " << "/proc/meminfo";
    p.start("awk", arguments);
    p.waitForFinished();
    value = p.readAllStandardOutput();
    m_totalPageFile = value.toULongLong() * 1024;
    p.close();
    arguments.clear();

    arguments << "/Inactive:/ {print $2} " << "/proc/meminfo";
    p.start("awk", arguments);
    p.waitForFinished();
    value = p.readAllStandardOutput();
    m_availablePageFile = value.toULongLong() * 1024;
    p.close();

    m_memoryLoad = 100 - (static_cast<int>((static_cast<float>(m_available) / static_cast<float>(m_total) * 100.0)));
#endif
}

void CMemoryInfo::getAppleMemoryInfo()
{
#ifdef __APPLE__
    QProcess p;
    //p.start("sysctl", QStringList() << "kern.version" << "hw.memsize");
    p.start("sysctl", QStringList() << "hw.memsize");
    p.waitForFinished();
    QString value = p.readAllStandardOutput();
    m_total = getNumberFromQString(value);//value.toULongLong();
    p.close();

    //p.start("sysctl", QStringList() << "kern.version" << "hw.usermem");
    p.start("sysctl", QStringList() << "hw.usermem");
    p.waitForFinished();
    value = p.readAllStandardOutput();
    m_available = m_total - getNumberFromQString(value);
    p.close();

    m_memoryLoad = static_cast<int>((static_cast<float>(m_available) / static_cast<float>(m_total) * 100.0));
#endif
}

unsigned long long CMemoryInfo::getNumberFromQString(const QString &xString)
{
  QRegExp xRegExp("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
  xRegExp.indexIn(xString);
  QStringList xList = xRegExp.capturedTexts();
  if (true == xList.empty())
  {
    return 0.0;
  }
  return xList.begin()->toULongLong();
}
