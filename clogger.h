#include <stdio.h>

#ifndef _H_CLOGGER
#define _H_CLOGGER

#define log(...) _log(__FILE__, __LINE__, __VA_ARGS__)


typedef struct {
	FILE *restrict stream;
	int severety;
	int settings;
} Journal;

enum log_level_name {
	LOG_DEBUG = 0,
	LOG_INFO = 1,
	LOG_WARN = 2,
	LOG_ERROR = 3,
	LOG_CRIT = 4
};

enum logger_settings {
	LOGGER_ECHO_STD = 1,
	LOGGER_ECHO_COLOR = 2,
	LOGGER_ECHO_SEVERE_ERR = 4,
	LOGGER_TRACE = 8,
	LOGGER_TIME = 16,
	LOGGER_COLOR = 32,
	LOGGER_ONLY_SEVERE = 64,
	LOGGER_ECHO_ONLY_SEVERE = 128,
	LOGGER_FLUSH = 256,
	LOGGER_ECHO_FLUSH = 512,
};

extern const int STD_SETTINGS;
extern const Journal STD_JOURNAL;

int _log(const char *file,
	 const int line,
	 const int severety,
	 const Journal filename, 
	 const char *restrict msg, 
	 ...);
#endif
