#ifndef PYUTILSDOCSTRING_HPP
#define PYUTILSDOCSTRING_HPP


constexpr auto _moduleDocString = "Module offering various helper tools.\n";

//----------------------
//----- CException -----
//-----------------------
constexpr auto _exceptionDocString =
        "Base class for all Ikomia exceptions.\n";

constexpr auto _ctorExcDocString =
        "Constructor with parameters\n\n"
        "arg1 (int): Exception code\n\n"
        "arg2 (str): Exception message\n\n"
        "arg3 (str): Function from where exception is thrown\n\n"
        "arg4 (str): File from where exception is thrown\n\n"
        "arg5 (int): Line in source file from where exception is thrown\n";

constexpr auto _messageDocString =
        "Get formatted exception message.\n\n"
        "Returns:\n"
        "   str: Exception message\n";

//-----------------------
//----- CMemoryInfo -----
//-----------------------
constexpr auto _memoryInfoDocString =
        "Cross-platform functions to retrieve memory information.";

constexpr auto _availableMemoryDocString =
        "Get current amount of available memory.\n\n"
        "Returns:\n"
        "   int: available amount of memory in MB\n";

constexpr auto _memoryLoadDocString =
        "Get the current memory load.\n\n"
        "Returns:\n"
        "   int: memory load in percentage\n";

constexpr auto _totalMemoryDocString =
        "Get total amount of memory installed on the machine.\n\n"
        "Returns:\n"
        "   int: total amount of memory in MB\n";

//------------------
//----- CTimer -----
//------------------
constexpr auto _timerDocString =
        "Cross-platform timer with nanoseconds precision\n";

constexpr auto _getElapsedMsDocString =
        "Get elapsed time in ms since the last call or since the start (first call).\n\n"
        "Returns:\n"
        "   float: elapsed time in ms\n";

constexpr auto _getElapsedNsDocString =
        "Get elapsed time in ns since the last call or since the start (first call).\n\n"
        "Returns:\n"
        "   float: elapsed time in ns\n";

constexpr auto _getElapsedUsDocString =
        "Get elapsed time in µs since the last call or since the start (first call).\n\n"
        "Returns:\n"
        "   float: elapsed time in µs\n";

constexpr auto _getTotalElapsedMsDocString =
        "Get elapsed time in ms since the start.\n\n"
        "Returns:\n"
        "   float: elapsed time in ms\n";

constexpr auto _getTotalElapsedNsDocString =
        "Get elapsed time in ns since the start.\n\n"
        "Returns:\n"
        "   float: elapsed time in ns\n";

constexpr auto _getTotalElapsedUsDocString =
        "Get elapsed time in µs since the start.\n\n"
        "Returns:\n"
        "   float: elapsed time in µs\n";

constexpr auto _printElapsedTimeDocString =
        "Print elapsed time in ms since the last call or since the start (first call).\n"
        "String printed to the standard output stream.\n\n"
        "Args:\n"
        "   name (str): label identifying timed process\n";

constexpr auto _printTotalElapsedTimeDocString =
        "Print elapsed time in ms since the start.\n"
        "String printed to the standard output stream.\n\n"
        "Args:\n"
        "   name (str): label identifying timed process\n";

constexpr auto _startDocString = "Launch timer.\n";


#endif // PYUTILSDOCSTRING_HPP
