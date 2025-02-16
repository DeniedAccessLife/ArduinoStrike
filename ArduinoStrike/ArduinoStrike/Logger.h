#pragma once

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
using namespace std;

class Logger
{
public:
    static void Init(const string& file = "arduino_debug.log");
    static void LogMessage(const string& message, boost::log::trivial::severity_level level = boost::log::trivial::info);

private:
    inline static bool initialized = false;
};