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

#include "PyUtils.h"
#include "PyUtilsDocString.hpp"
#include "boost/python.hpp"
#include "CException.h"
#include "CMemoryInfo.h"
#include "CTimer.hpp"
#include "UtilsDefine.hpp"
#include "UtilsTools.hpp"

BOOST_PYTHON_MODULE(pyutils)
{
    using namespace  Ikomia::Utils;
    using namespace boost::python;

    // Enable user-defined docstrings and python signatures, while disabling the C++ signatures
    docstring_options local_docstring_options(true, true, false);

    // Set the docstring of the current module scope
    scope().attr("__doc__") = _moduleDocString;

    enum_<PluginState>("PluginState", "Enum - List of plugin states for version compatibility")
        .value("VALID", PluginState::VALID)
        .value("DEPRECATED", PluginState::DEPRECATED)
        .value("UPDATED", PluginState::UPDATED)
    ;

    def("getApiVersion", &Utils::IkomiaApp::getCurrentVersionNumber, _getCurrentVersionDocString);
    def("getCompatibilityState", &Utils::Plugin::getStdPythonState, _pythonStateDocString, args("version"));

    //----- Binding CException -----
    class_<CException>("CException", _exceptionDocString, init<>("Default constructor"))
        .def(init<int, const std::string&, const std::string&, const std::string&, int>(_ctorExcDocString))
        .def("message", &CException::getMessage, _messageDocString, args("self"))
    ;

    //----- Binding CMemoryInfo -----
    class_<CMemoryInfo>("CMemoryInfo", _memoryInfoDocString, init<>("Default constructor"))
        .def("totalMemory", &CMemoryInfo::totalMemory, _totalMemoryDocString, args("self"))
        .def("availableMemory", &CMemoryInfo::availableMemory, _availableMemoryDocString, args("self"))
        .def("memoryLoad", &CMemoryInfo::memoryLoad, _memoryLoadDocString, args("self"))
    ;

    //----- Binding CTimer -----
    class_<CTimer>("CTimer", _timerDocString, init<>("Default constructor"))
        .def("start", &CTimer::start, _startDocString, args("self"))
        .def("printElapsedTime_ms", &CTimer::printElapsedTime_ms, _printElapsedTimeDocString, args("self", "name"))
        .def("printTotalElapsedTime_ms", &CTimer::printTotalElapsedTime_ms, _printTotalElapsedTimeDocString, args("self", "name"))
        .def("get_elapsed_ms", &CTimer::get_elapsed_ms, _getElapsedMsDocString, args("self"))
        .def("get_total_elapsed_ms", &CTimer::get_total_elapsed_ms, _getTotalElapsedMsDocString, args("self"))
        .def("get_elapsed_us", &CTimer::get_elapsed_us, _getElapsedUsDocString, args("self"))
        .def("get_total_elapsed_us", &CTimer::get_total_elapsed_us, _getTotalElapsedUsDocString, args("self"))
        .def("get_elapsed_ns", &CTimer::get_elapsed_ns, _getElapsedNsDocString, args("self"))
        .def("get_total_elapsed_ns", &CTimer::get_total_elapsed_ns, _getTotalElapsedNsDocString, args("self"))
    ;
}
