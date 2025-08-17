#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
using namespace std;

class Logger
{
public:
    static void Init(const string& file = "arduino_debug.log");
    static void LogMessage(const string& message, boost::log::trivial::severity_level level = boost::log::trivial::info);
    static void Shutdown();

private:
    static atomic<bool> initialized;
    static boost::shared_ptr<boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>> sink;
};