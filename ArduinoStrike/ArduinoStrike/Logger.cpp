#include "Logger.h"

atomic<bool> Logger::initialized(false);
boost::shared_ptr<boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>> Logger::sink;

void Logger::Init(const string& file)
{
    if (!initialized.exchange(true))
    {
        const string date_format = "%d.%m.%Y %H:%M:%S";

        using file_backend = boost::log::sinks::text_file_backend;
        auto backend = boost::make_shared<file_backend>(
            boost::log::keywords::file_name = file,
            boost::log::keywords::open_mode = std::ios::trunc,
            boost::log::keywords::auto_flush = true
        );

        sink = boost::make_shared<boost::log::sinks::asynchronous_sink<file_backend>>(backend);

        sink->set_formatter(
            boost::log::expressions::stream
            << "["
            << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", date_format)
            << "] ["
            << boost::log::trivial::severity
            << "] "
            << boost::log::expressions::smessage
        );

        boost::log::core::get()->add_sink(sink);
        boost::log::add_common_attributes();

        boost::log::core::get()->flush();

        BOOST_LOG_TRIVIAL(info) << "Logger initialized successfully.";
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

void Logger::Shutdown()
{
    if (initialized)
    {
        if (sink)
        {
            sink->flush();
            sink->stop();
            boost::log::core::get()->remove_sink(sink);
            sink.reset();
        }
        initialized = false;
    }
}