#ifndef PCH_H
#define PCH_H

// Define macros before including any system headers:
// WIN32_LEAN_AND_MEAN - Exclude rarely-used Windows components to speed up compilation
#define WIN32_LEAN_AND_MEAN 

// NOMINMAX - Prevent Windows from defining min and max as macros (avoids conflicts with std::min/std::max)
#define NOMINMAX 

// _WINSOCKAPI_ - Prevent the inclusion of WinSock.h (to avoid conflicts with winsock2.h)
#define _WINSOCKAPI_

// Include Winsock headers before windows.h to ensure proper networking support
#include <winsock2.h>
#include <ws2tcpip.h>

// Include Windows API headers
#include <windows.h>

// Standard C++ headers for input/output operations
#include <iostream>
#include <sstream>
#include <fstream>

// Standard C++ headers for string manipulation and data structures
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

// Headers for numerical formatting, file system operations, and regular expressions
#include <regex>
#include <iomanip>
#include <filesystem>

// Headers for multithreading support
#include <thread>

// Boost.Asio - High-performance networking library
#include <boost/asio.hpp>

#endif // PCH_H
