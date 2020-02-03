#ifndef SD_HANDLER_H
#define SD_HANDLER_H

#include <stdint.h>

#define SD_CHIP_SELECT PE12

void sd_init();

void sd_reboot();

//returns true if logging is working
uint8_t sd_active();
uint8_t sd_buffer_dirty();

//logs to the current log file. Buffered, so.. bear that in mind?
void ST_PATSlog(const char* message);


//flushes the buffered output. Should we be hand buffering our logs?
void flush();


#endif
