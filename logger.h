#ifndef LOGGER_H
#define LOGGER_H

/*!
 * \file logger.h
 * \brief Implementation of logger objects.
 * \author Raphaël Lefèvre
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <ostream>

/*!
 * \namespace log
 * \brief Namespace which contains logger-related implementations.
 */
namespace log
{
    /*!
     * \enum LOG_TYPE
     * \brief Describe the different log types.
     */
    enum LOG_TYPE {INFO = 0, WARNING = 1, ERROR = 2, FATAL = 3};

    /*!
     * \class Logger
     * \brief A full-featured logger.
     *
     * A Logger instance can write logs into:
     *    - A file
     *    - A `std::ostream` (or any derived)
     *    - A custom user stream (which defines `operator<<()` for `const char *`)
     *
     * A Logger instance can either:
     *    - Buffer the logs and dump the buffer afterwards
     *    - Directly write the logs
     *
     * There are 4 types of logs which can be individually enabled/disabled:
     *    - `INFO`
     *    - `WARNING`
     *    - `ERROR`
     *    - `FATAL`
     *
     * By default, all log types are enabled.
     *
     * A log message is structured as the concatenation of:
     *    - HEADER: There is one header per log type. They are customizable and defaulted at `"[LOG_TYPE]"`.
     *    - SEPARATOR: The separator is located between the header and the message. It is customizable and defaulted at `" - "`.
     *    - MESSAGE: The log message.
     *    - NEWLINE: The newline sequence. It is customizable and defaulted at `"\n"`.
     */
    class Logger
    {
        private:
            std::unordered_map<LOG_TYPE, bool> flags_logs_enabled_;

            std::unordered_map<LOG_TYPE, std::string> logs_headers_;
            std::string separator_;
            std::string newline_;

            std::vector<std::string> buffer_;

        public:
            // Constructors
            /*!
             * \brief Default constructor.
             */
            Logger();
            /*!
             * \brief Constructor.
             * \param[in] buffer_min_capacity The minimum buffer capacity (no memory reallocation guaranteed below this number of buffered logs)
             */
            Logger(std::size_t buffer_min_capacity);
            /*!
             * \brief Copy constructor.
             * \note Deleted.
             */
            Logger(const Logger &) = delete;
            /*!
             * \brief Move constructor.
             * \note Deleted.
             */
            Logger(Logger &&) = delete;
            /*!
             * \brief Copy assignment operator.
             * \return A reference to `*this`
             * \note Deleted.
             */
            Logger & operator=(const Logger &) = delete;
            /*!
             * \brief Move assignment operator.
             * \return A reference to `*this`
             * \note Deleted.
             */
            Logger & operator=(Logger &&) = delete;

            // Flags
            /*!
             * \brief Enable/disable a log type.
             * \param[in] log_type The type of logs to enable/disable
             * \param[in] enabled The activation flag value -- Default: `true`
             */
            void setEnabled(LOG_TYPE log_type, bool enabled = true);
            /*!
             * \brief Enable/disable all logs types.
             * \param[in] enabled The activation flags value -- Default: `true`
             */
            void setEnabledAll(bool enabled = true);
            /*!
             * \brief Check if a log type is enabled or not.
             * \param[in] log_type The log type to query
             * \return The activation flag value (`true` or `false` whether the log type is enabled or not)
             */
            bool isEnabled(LOG_TYPE log_type) const;

            // Format
            /*!
             * \brief Set a header format.
             * \param[in] log_type The log type for which the change is requested
             * \param[in] header The new header format
             */
            void setHeader(LOG_TYPE log_type, const std::string & header);
            /*!
             * \brief Get a header format.
             * \param[in] log_type The log type for which the header format is requested
             * \return The header format
             */
            std::string header(LOG_TYPE log_type) const;
            /*!
             * \brief Set the separator format.
             * \param[in] sep The new separator format
             */
            void setSeparator(const std::string & sep);
            /*!
             * \brief Get the separator format.
             * \return The separator format
             */
            std::string separator() const;
            /*!
             * \brief Set the newline sequence.
             * \param[in] nl The new newline sequence
             */
            void setNewline(const std::string & nl);
            /*!
             * \brief Get the newline sequence.
             * \return The newline sequence
             */
            std::string newline() const;

            // Log & dump
            /*!
             * \brief Buffer a log message.
             * \param[in] log_type The log type
             * \param[in] message The log message
             */
            void log(LOG_TYPE log_type, const std::string & message);
            /*!
             * \brief Write a log message.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[out] os The output stream where to flush the log message
             */
            void log(LOG_TYPE log_type, const std::string & message, std::ostream & os) const;
            /*!
             * \brief Write a log message into a file.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[in] file_name The file name
             * \param[in] mode The open mode -- Default: `std::ios_base::out`
             * \return `false` if the file could not be opened, `true` otherwise
             */
            bool log(LOG_TYPE log_type, const std::string & message, const std::string & file_name, std::ios_base::openmode mode = std::ios_base::out) const;
            /*!
             * \brief Dump the logs buffer.
             * \param[out] os The output stream where to flush the buffer
             */
            void dump(std::ostream & os);
            /*!
             * \brief Dump the logs buffer into a file.
             * \param[in] file_name The file name
             * \param[in] mode The open mode -- Default: `std::ios_base::out`
             * \return `false` if the file could not be opened, `true` otherwise
             */
            bool dump(const std::string & file_name, std::ios_base::openmode mode = std::ios_base::out);

            // Log & dump for user custom output streams
            /*!
             * \brief Write a log message.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[out] stream The stream where to flush the log message
             */
            template <typename StreamT>
            void log(LOG_TYPE log_type, const std::string & message, StreamT & stream) const
            {
                if(flags_logs_enabled_.at(log_type))
                    stream << logs_headers_.at(log_type).c_str() << separator_.c_str() << message.c_str() << newline_.c_str();
            }
            /*!
             * \brief Dump the logs buffer.
             * \param[out] stream The stream where to flush the buffer
             */
            template <typename StreamT>
            void dump(StreamT & stream)
            {
                for(const auto & line : buffer_)
                    stream << line.c_str() << newline_.c_str();
                buffer_.clear();
            }
    };

    /*!
     * \class SLogger
     * \brief A minimal `static` logger.
     *
     * A SLogger is a minimal logger that cannot buffer the logs messages. The logs are written the same way as a Logger instance does except that the flushing cannot be delegated.<br/>
     * A SLogger is `static` and not instantiable.<br/>
     * Like the Logger instances, a SLogger can individually enable/disable different logs types.<br/>
     * By default, all logs types are enabled.
     *
     * The log messages are prepended with the following non-customizable sequence `"[LOG_TYPE] - "`.<br/>
     * Like the Logger instances, the newline sequence is customizable and defaulted at `"\n"`.
     */
    class SLogger
    {
        public:
            static bool INFO_ENABLED_FLAG;    /*!< Activation flag for logs type `INFO` -- Default: `true` */
            static bool WARNING_ENABLED_FLAG; /*!< Activation flag for logs type `WARNING` -- Default: `true` */
            static bool ERROR_ENABLED_FLAG;   /*!< Activation flag for logs type `ERROR` -- Default: `true` */
            static bool FATAL_ENABLED_FLAG;   /*!< Activation flag for logs type `FATAL` -- Default: `true` */
            static std::string NEWLINE;       /*!< The newline sequence -- Default: `"\n"` */

            /*!
             * \brief Write a log message.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[out] os The output stream where to flush the log message
             */
            static void log(LOG_TYPE log_type, const std::string & message, std::ostream & os);
            /*!
             * \brief Write a log message into a file.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[in] file_name The file name
             * \param[in] mode The open mode -- Default: `std::ios_base::out`
             * \return `false` if the file could not be opened, `true` otherwise
             */
            static bool log(LOG_TYPE log_type, const std::string & message, const std::string & file_name, std::ios_base::openmode mode = std::ios_base::out);

            /*!
             * \brief Write a log message.
             * \param[in] log_type The log type
             * \param[in] message The log message
             * \param[out] stream The stream where to flush the log message
             */
            template <typename StreamT>
            static void log(LOG_TYPE log_type, const std::string & message, StreamT & stream)
            {
                switch (log_type)
                {
                    case INFO: if(INFO_ENABLED_FLAG) stream << "[INFO] - " << message.c_str() << NEWLINE.c_str();
                        break;
                    case WARNING: if(WARNING_ENABLED_FLAG) stream << "[WARNING] - " << message.c_str() << NEWLINE.c_str();
                        break;
                    case ERROR: if(ERROR_ENABLED_FLAG) stream << "[ERROR] - " << message.c_str() << NEWLINE.c_str();
                        break;
                    case FATAL: if(FATAL_ENABLED_FLAG) stream << "[WARNING] - " << message.c_str() << NEWLINE.c_str();
                }
            }

            /*!
             * \brief Default constructor.
             * \note Deleted.
             */
            SLogger() = delete;
            /*!
             * \brief Copy constructor.
             * \note Deleted.
             */
            SLogger(const SLogger &) = delete;
            /*!
             * \brief Move constructor.
             * \note Deleted.
             */
            SLogger(SLogger &&) = delete;
            /*!
             * \brief Copy assignment operator.
             * \return A reference to `*this`
             * \note Deleted.
             */
            SLogger & operator=(const SLogger &) = delete;
            /*!
             * \brief Move assignment operator.
             * \return A reference to `*this`
             * \note Deleted.
             */
            SLogger & operator=(SLogger &&) = delete;
    };
}

#endif // LOGGER_H
