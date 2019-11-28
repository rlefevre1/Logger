# Logger
Logger implementation for log reports

---

### `Logger`'s description
**Brief:** A full-featured logger.

A Logger instance can write logs into:
    - A file
    - A `std::ostream` (or any derived)
    - A custom user stream (which defines `operator<<()` for `const char *`)

 A Logger instance can either:
    - Buffer the logs and dump the buffer afterwards
    - Directly write the logs

 There are 4 types of logs which can be individually enabled/disabled:
    - `INFO`
    - `WARNING`
    - `ERROR`
    - `FATAL`

 By default, all log types are enabled.

 A log message is structured as the concatenation of:
    - HEADER: There is one header per log type. They are customizable and defaulted at `"[LOG_TYPE]"`.
    - SEPARATOR: The separator is located between the header and the message. It is customizable and defaulted at `" - "`.
    - MESSAGE: The log message.
    - NEWLINE: The newline sequence. It is customizable and defaulted at `"\n"`.

### `SLogger`'s description
**Brief:** A minimal `static` logger.

A SLogger is a minimal logger that cannot buffer the logs messages. The logs are written the same way as a Logger instance does except that the flushing cannot be delegated.<br/>
A SLogger is `static` and not instantiable.<br/>
As for Logger instances, a SLogger can individually enable/disable different logs types.<br/>
By default, all logs types are enabled.

The log messages are prepended with the following non-customizable sequence `"[LOG_TYPE] - "`.<br/>
As for Logger instances, the newline sequence is customizable and defaulted at `"\n"`.
