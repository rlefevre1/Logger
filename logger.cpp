#include "logger.h"

#include <fstream>

namespace logs
{
    // Constructors
    Logger::Logger() : flags_logs_enabled_ {{INFO,     true}, {WARNING,        true}, {ERROR,      true}, {FATAL,      true}},
                       logs_headers_       {{INFO, "[INFO]"}, {WARNING, "[WARNING]"}, {ERROR, "[ERROR]"}, {FATAL, "[FATAL]"}},
                       separator_(" - "), newline_("\n")
    {}
    Logger::Logger(std::size_t buffer_min_capacity) : Logger()
    {
        buffer_.reserve(buffer_min_capacity);
    }

    // Flags
    void Logger::setEnabled(LOG_TYPE log_type, bool enabled)
    {
        flags_logs_enabled_[log_type] = enabled;
    }
    void Logger::setEnabledAll(bool enabled)
    {
        for(auto it = flags_logs_enabled_.begin(); it != flags_logs_enabled_.end(); ++it)
            it->second = enabled;
    }
    bool Logger::isEnabled(LOG_TYPE log_type) const
    {
        return flags_logs_enabled_.at(log_type);
    }

    // Format
    void Logger::setHeader(LOG_TYPE log_type, const std::string & header)
    {
        logs_headers_[log_type] = header;
    }
    std::string Logger::header(LOG_TYPE log_type) const
    {
        return logs_headers_.at(log_type);
    }
    void Logger::setSeparator(const std::string & sep)
    {
        separator_ = sep;
    }
    std::string Logger::separator() const
    {
        return separator_;
    }
    void Logger::setNewline(const std::string & nl)
    {
        newline_ = nl;
    }
    std::string Logger::newline() const
    {
        return newline_;
    }

    // Log & dump
    void Logger::log(LOG_TYPE log_type, const std::string & message)
    {
        if(flags_logs_enabled_[log_type])
            buffer_.push_back(logs_headers_[log_type] + separator_ + message);
    }
    void Logger::log(LOG_TYPE log_type, const std::string & message, std::ostream & os) const
    {
        if(flags_logs_enabled_.at(log_type))
            os << logs_headers_.at(log_type) << separator_ << message << newline_;
    }
    bool Logger::log(LOG_TYPE log_type, const std::string & message, const std::string & file_name, std::ios_base::openmode mode) const
    {
        if(flags_logs_enabled_.at(log_type))
        {
            std::ofstream ofs(file_name, mode);
            if(!ofs)
                return false;

            ofs << logs_headers_.at(log_type) << separator_ << message << newline_;
            ofs.close();
        }
        return true;
    }
    void Logger::dump(std::ostream & os)
    {
        for(const auto & line : buffer_)
            os << line << newline_;
        buffer_.clear();
    }
    bool Logger::dump(const std::string & file_name, std::ios_base::openmode mode)
    {
        std::ofstream ofs(file_name, mode);
        if(!ofs)
            return false;

        for(const auto & line : buffer_)
            ofs << line << newline_;
        ofs.close();
        buffer_.clear();
        return true;
    }

    bool SLogger::INFO_ENABLED_FLAG = true;
    bool SLogger::WARNING_ENABLED_FLAG = true;
    bool SLogger::ERROR_ENABLED_FLAG = true;
    bool SLogger::FATAL_ENABLED_FLAG = true;
    std::string SLogger::NEWLINE = "\n";

    void SLogger::log(LOG_TYPE log_type, const std::string & message, std::ostream & os)
    {
        switch (log_type)
        {
            case INFO: if(INFO_ENABLED_FLAG) os << "[INFO] - " << message << NEWLINE;
                break;
            case WARNING: if(WARNING_ENABLED_FLAG) os << "[WARNING] - " << message << NEWLINE;
                break;
            case ERROR: if(ERROR_ENABLED_FLAG) os << "[ERROR] - " << message << NEWLINE;
                break;
            case FATAL: if(FATAL_ENABLED_FLAG) os << "[WARNING] - " << message << NEWLINE;
        }
    }
    bool SLogger::log(LOG_TYPE log_type, const std::string & message, const std::string & file_name, std::ios_base::openmode mode)
    {
        switch (log_type)
        {
            case INFO: if(INFO_ENABLED_FLAG)
                       {
                           std::ofstream ofs(file_name, mode);
                           if(!ofs)
                               return false;
                           ofs << "[INFO] - " << message << NEWLINE;
                           ofs.close();
                       }
                break;
            case WARNING: if(WARNING_ENABLED_FLAG)
                          {
                              std::ofstream ofs(file_name, mode);
                              if(!ofs)
                                  return false;
                              ofs << "[WARNING] - " << message << NEWLINE;
                              ofs.close();
                          }
                break;
            case ERROR: if(ERROR_ENABLED_FLAG)
                        {
                            std::ofstream ofs(file_name, mode);
                            if(!ofs)
                                return false;
                            ofs << "[ERROR] - " << message << NEWLINE;
                            ofs.close();
                        }
                break;
            case FATAL: if(FATAL_ENABLED_FLAG)
                        {
                            std::ofstream ofs(file_name, mode);
                            if(!ofs)
                                return false;
                            ofs << "[FATAL] - " << message << NEWLINE;
                            ofs.close();
                        }
        }
        return true;
    }
}
