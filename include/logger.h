#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "definitions.h"

class Logger
{
  public:
    static Logger *null;
    
  public:
    Logger(void(*)(const char*));
    ~Logger();
    
    void fatal(const char*, ...);
    void report(const char*, ...);
    void warning(const char*, ...);
    Logger& setFlags(bool, bool, bool);
    
  private:
    const static Size buffer_size = 256;
    bool 
    	fatalFlag,
    	warningFlag,
    	reportFlag;
    char *buffer;
    void (*core)(const char*);
};

#endif