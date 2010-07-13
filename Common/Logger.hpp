/*!
 * \file Logger.hpp
 * \brief Simple logging object
 * \author mstefanc
 * \date 2010-06-10
 */
#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "LoggerAux.hpp"
#include "LoggerImpl.hpp"
#include "ScopeLogger.hpp"

#define LOGGER Utils::Logger::Logger::instance()

#define TRACE    Utils::Logger::Trace
#define DEBUG    Utils::Logger::Debug
#define INFO     Utils::Logger::Info
#define NOTICE   Utils::Logger::Notice
#define WARNING  Utils::Logger::Warning
#define ERROR    Utils::Logger::Error
#define CRITICAL Utils::Logger::Critical
#define FATAL    Utils::Logger::Fatal

/// Start message printing
#define LOG(level) (Utils::Logger::ScopeLogger(LOGGER, __FILE__, __LINE__, level).get())


#endif /* LOGGER_HPP_ */
