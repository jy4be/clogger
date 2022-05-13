# clogger

## About
A small logging library in C. 

It aims to be just enough to be used in small projects wihch can get away with a simple "One file" logging solution. 
It has no shared mutable values, and as such is inherently thread-safe.

The Idea of this logger is that most projects of smaller scales require a log to simply be written to a file, and sometimes be mirrored to the console.
And this is all the functionality this logging tool has: It wrie a Log to a file and echo it to `stdout` and `stderr`.

It shines because of its ease of use: it only uses one function and requires little to no setup if just want to log something to the console.

## Format
The standart format for a log is as such:
```
[LOG_LEVEL]: MESSAGE
```

If `LOGGER_TRACE` and `LOGGER_TIME` are set, the log will look like this:
```
[HH:MM:SS][LOG_LEVEL][FILENAME:LINE]: MESSAGE
```



## Usage
### Functions & Macros
```C
int log(const int severity, const Journal journal, const char *restric msg, ...)
```

----

All logs are logged into a concept called a "Journal".

Using the macro `log()`, you can write a message with fprint-style arguments to a Journal.

### Log Severeties
The logger has five builtin severties:

0. `LOG_DEBUG`
1. `LOG_INFO`
2. `LOG_WARN`
3. `LOG_ERROR`
4. `LOG_CRIT`

They all have an ansi Color code assigned to them.

### Journal
A Journal contains all the Info on where the logs shuld be written to.
It is a struct with the following members:
```C
struct {
    FILE *restrict stream;
    int severity;
    int settings;
};
```
- **stream** is the file stream the log will get written to, can be `NULL`
- **severity** is the Journals severity level; all logs which have a severity level which is on par or exceeding this value will be treated as severe
- **settings** are the Journals Settings. They are defined in an enum in `clogger.h`, and are to be bitwise-ored together. For mor Info, see **Settings** below

The logger comes with a standart Journal, namely `STD_JOURNAL`, which will essentially just print the log to `stdout`

#### Settings
A Journal can have the following Settings:
| Name | Function|
|------|--------:|
| `LOGGER_ECHO_STD` | Echo the Log to `stdout`|
| `LOGGER_ECHO_COLOR` |Echo the Log with ansi color codes, increasing readability|
| `LOGGER_ECHO_SEVERE_ERR` |Instead of echoing everything to `stdout`, severe Logs will get written `stderr`|
| `LOGGER_TRACE` |Additionally log filename and line the Log was written on in the Message|
| `LOGGER_TIME` |Additionally log the time at which the Log was written in the Message|
| `LOGGER_COLOR` |Write the Log to the Filestream with ansi color codes|
| `LOGGER_ONLY_SEVERE` |Instead of logging all Logs to the Filestream, log only Logs treated as severe|
| `LOGGER_ECHO_ONLY_SEVERE` |Instead of echoing all logs, echo only Logs treated as severe|
| `LOGGER_FLUSH` |Flush the Filestream after logging|
| `LOGGER_ECHO_FLUSH`| Flush `stdout`/`stderr` after logging|

These values can be bitwise-ORed together and be passed to the Journal as Settings.



### Examples
```C
#include "clogger.h"
...
log(LOG_INFO, STD_JOURNAL, "This will be Logged as an Info-Log");
```
This is the simplest usecase for this tool: no setup, just start logging directly.

```C
#include "clogger.h"
...
FILE *log_stream = fopen("log.txt", "w");
int myJournal_settings = LOGGER_ECHO_STD | LOGGER_ECHO_COLOR | LOGGER_TRACE | LOGGER_TIME | LOGGER_FLUSH | LOGGER_ECHO_FLUSH;

Journal myJournal = {log_stream, LOG_ERROR, myJournal_settings};
log(LOG_WARN, myJournal, "This is a Warning log with a little bit more setup, and also an fprint-style argument: %d", 420);

fclose(log_stream);
```
This is a more General Use-case of how to use this logger.

----

For more examples see [exapmle.c](https://github.com/jy4be/clogger/blob/main/examples/example.c)
