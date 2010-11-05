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

#define LTRACE    Utils::Logger::Trace
#define LDEBUG    Utils::Logger::Debug
#define LINFO     Utils::Logger::Info
#define LNOTICE   Utils::Logger::Notice
#define LWARNING  Utils::Logger::Warning
#define LERROR    Utils::Logger::Error
#define LCRITICAL Utils::Logger::Critical
#define LFATAL    Utils::Logger::Fatal

/// Start message printing
#define LOG(level) (Utils::Logger::ScopeLogger(LOGGER, __FILE__, __LINE__, level).get())

#endif /* LOGGER_HPP_ */
