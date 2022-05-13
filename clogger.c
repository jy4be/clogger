#include <stdarg.h>  
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "clogger.h"


const int STD_SETTINGS = LOGGER_ECHO_STD |
                         LOGGER_ECHO_COLOR |
                         LOGGER_ECHO_SEVERE_ERR |
                         LOGGER_TRACE |
                         LOGGER_TIME |
                         LOGGER_FLUSH |
                         LOGGER_ECHO_FLUSH;

const Journal STD_JOURNAL = {NULL, LOG_ERROR, STD_SETTINGS};

const char *_log_level_strings[] = {"DEBUG   ", "INFO    ", "WARN    ", "ERROR   ", "CRITICAL"};
const char *_log_ansi_colors[]   = {"\033[37;2m", "\033[37m", "\033[33m", "\033[31m", "\033[40;39;1m"};

int log_to_stream(const char *restrict time_string,
                  const char *restrict color_string,
                  const char *restrict level_string,
                  const char *restrict trace_string,
                  FILE *restrict stream,
                  const bool is_color,
                  const char *restrict msg,
                  va_list fprint_args);

inline bool is_setting(int setting, int flag);

void get_trace_string(char *buffer, 
                const int buffer_size,
                const char *restrict file,
                const int line);

void get_time_string(char *buffer);

void set_logger_info(const Journal journal,
                     const char *restrict file,
                     const int line,
                     const int severety,
                     char *restrict time_string,
                     char *restrict level_string,
                     char *restrict trace_string);


int _log(const char *file,
         const int line,
         const int severety,
         const Journal journal,
         const char *restrict msg,
         ...)
{
        int fprint_return = -1;
        char time_string[11]  = "";
        char level_string[9]  = "";
        char trace_string[64] = "";
        
        set_logger_info(journal, file, line, severety, time_string, level_string, trace_string);

        bool is_color      = is_setting(journal.settings, LOGGER_COLOR);
        bool is_echo       = is_setting(journal.settings, LOGGER_ECHO_STD);
        bool is_echo_error = is_setting(journal.settings, LOGGER_ECHO_SEVERE_ERR);
        bool is_echo_color = is_setting(journal.settings, LOGGER_ECHO_COLOR);
        bool is_flush      = is_setting(journal.settings, LOGGER_FLUSH);
        bool is_echo_flush = is_setting(journal.settings, LOGGER_ECHO_FLUSH);

        bool is_severety_sufficient      = severety >= journal.severety ||
                                           !is_setting(journal.settings, LOGGER_ONLY_SEVERE);

        bool is_echo_severety_sufficient = severety >= journal.severety ||
                                           !is_setting(journal.settings, LOGGER_ECHO_ONLY_SEVERE);

        va_list fprint_args;
        va_start (fprint_args, msg);

        if (journal.stream && is_severety_sufficient)
                fprint_return = log_to_stream(time_string,
                              _log_ansi_colors[severety],
                              level_string,
                              trace_string,
                              journal.stream,
                              is_color,
                              msg,
                              fprint_args);

        if (journal.stream && is_flush)
                fflush(journal.stream);

        va_start(fprint_args, msg);

        if (is_echo && is_echo_severety_sufficient) {
                if (severety >= journal.severety && is_echo_error)
                        fprint_return = log_to_stream(time_string,
                                      _log_ansi_colors[severety],
                                      level_string,
                                      trace_string,
                                      stderr,
                                      is_echo_color,
                                      msg,
                                      fprint_args);
                else
                        fprint_return = log_to_stream(time_string,
                                      _log_ansi_colors[severety],
                                      level_string,
                                      trace_string,
                                      stdout,
                                      is_echo_color,
                                      msg,
                                      fprint_args);
        }

        if(is_echo_flush) {
                fflush(stdout);
                fflush(stderr);
        }

        va_end(fprint_args);

        return fprint_return;
}

void set_logger_info(const Journal journal,
                     const char *restrict file,
                     const int line,
                     const int severety,
                     char *restrict time_string,
                     char *restrict level_string,
                     char *restrict trace_string)
{
        if (is_setting(journal.settings, LOGGER_TIME)) 
                get_time_string(time_string);

        strncpy(level_string, _log_level_strings[severety], 8);

        if (is_setting(journal.settings, LOGGER_TRACE)) 
                get_trace_string(trace_string, 64, file, line);
}

void get_time_string(char *buffer)
{

        time_t raw_time;
        struct tm *time_data;

        time(&raw_time);
        time_data = localtime(&raw_time);

        strftime(buffer, 11, "[%T]", time_data);
}

void get_trace_string(char *buffer, 
                const int buffer_size,
                const char *restrict file,
                const int line)
{
        snprintf(buffer, buffer_size, " %s:%d", file, line); 
}

inline bool is_setting(int setting, int flag)
{
        return setting & flag ? true : false;
}

int log_to_stream(const char *restrict time_string,
                  const char *restrict color_string,
                  const char *restrict level_string,
                  const char *restrict trace_string,
                  FILE *restrict stream,
                  const bool is_color,
                  const char *restrict msg,
                  va_list fprint_args)
{
        int fprint_return;

        if (!is_color)
                fprintf(stream, "%s[%s]%s: ",
                                time_string,
                                level_string,
                                trace_string);
        else 
                fprintf(stream, "\033[37;2m%s\033[0m%s[%s]\033[0m\033[37;2m%s: \033[0m",
                                time_string,
                                color_string, 
                                level_string,
                                trace_string);
        fprint_return = vfprintf(stream, msg, fprint_args);
        fprintf(stream, "\n");

        return fprint_return;   
}

