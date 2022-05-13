#include "clogger.h"


void std_log();
void file_log();
void severety_log();
void detail_log();
void color_log();

int main() {
        std_log();
        printf("\n");
        file_log();
        printf("\n");
        severety_log();
        printf("\n");
        detail_log();
        printf("\n");
        color_log();
}

void std_log() {
        Journal journal = STD_JOURNAL;
        log(LOG_DEBUG, journal, "This is a Debug log");
        log(LOG_INFO, journal, "This is an Info log");
        log(LOG_ERROR, journal, "This is an Error log with fprint style arguments: %d, %s", 10, "Yoinks");
}

void file_log() {
        FILE *log_stream = fopen("test_log.txt", "w");
        Journal journal = {log_stream, LOG_ERROR, LOGGER_FLUSH |
                                                  LOGGER_TIME |
                                                  LOGGER_TRACE};
        log(LOG_INFO, journal, "This will get logged into a file only!");
        fclose(log_stream);
}

void severety_log() {
        FILE *log_stream = fopen("test_severety_log.txt", "w");
        Journal journal = {log_stream, LOG_ERROR, LOGGER_ECHO_STD |
                                                  LOGGER_FLUSH |
                                                  LOGGER_ECHO_FLUSH |
                                                  LOGGER_ONLY_SEVERE |
                                                  LOGGER_ECHO_ONLY_SEVERE |
                                                  LOGGER_TIME |
                                                  LOGGER_TRACE};
        log(LOG_INFO, journal, "This will not get logged, since its severety level is not high enough");
        log(LOG_ERROR, journal, "This will get logged, since its severety matches or exceeds the journals set severety");
        fclose(log_stream);
}

void detail_log() {
        Journal simple_journal = {NULL, LOG_ERROR, LOGGER_ECHO_STD|
                                                   LOGGER_ECHO_FLUSH |
                                                   LOGGER_ECHO_COLOR };

        Journal sophisticated_journal = {NULL, LOG_ERROR, LOGGER_ECHO_STD|
                                                          LOGGER_ECHO_FLUSH |
                                                          LOGGER_ECHO_COLOR |
                                                          LOGGER_TIME | 
                                                          LOGGER_TRACE};
        log(LOG_INFO, simple_journal, "This is a simple log without detail");
        log(LOG_INFO, sophisticated_journal, "This is a more detailed Log");
}

void color_log() {
        Journal monochrome_journal = {NULL, LOG_ERROR, LOGGER_ECHO_STD |
                                                       LOGGER_ECHO_FLUSH |
                                                       LOGGER_TIME |
                                                       LOGGER_TRACE};
        Journal color_journal = {NULL, LOG_ERROR, LOGGER_ECHO_STD |
                                                  LOGGER_ECHO_FLUSH |
                                                  LOGGER_ECHO_COLOR |
                                                  LOGGER_TIME |
                                                  LOGGER_TRACE};
        log(LOG_WARN, monochrome_journal, "This is a colorless Warning log");
        log(LOG_ERROR, monochrome_journal, "This is a colorless Error log");

        log(LOG_WARN, color_journal, "This is a colored Warning log");
        log(LOG_ERROR, color_journal, "This is a colored Warning log");
}
