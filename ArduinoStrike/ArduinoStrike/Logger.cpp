#include "Logger.h"

void Logger::Init(const string& file)
{
    if (!initialized)
    {
        auto sink = boost::log::add_file_log(
            boost::log::keywords::file_name = file,
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = boost::log::expressions::stream << "[" << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%d.%m.%Y %H:%M:%S") << "] [" << boost::log::trivial::severity << "] " << boost::log::expressions::smessage);

        sink->locked_backend()->auto_flush(true);
        boost::log::add_common_attributes();
        initialized = true;
    }
}

void Logger::LogMessage(const string& message, boost::log::trivial::severity_level level)
{
    switch (level)
    {
        case boost::log::trivial::trace:
            BOOST_LOG_TRIVIAL(trace) << message;
            break;

        case boost::log::trivial::debug:
            BOOST_LOG_TRIVIAL(debug) << message;
            break;

        case boost::log::trivial::info:
            BOOST_LOG_TRIVIAL(info) << message;
            break;

        case boost::log::trivial::warning:
            BOOST_LOG_TRIVIAL(warning) << message;
            break;

        case boost::log::trivial::error:
            BOOST_LOG_TRIVIAL(error) << message;
            break;

        case boost::log::trivial::fatal:
            BOOST_LOG_TRIVIAL(fatal) << message;
            break;

        default: BOOST_LOG_TRIVIAL(info) << message;
    }
}