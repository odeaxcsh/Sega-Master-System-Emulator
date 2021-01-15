#ifndef __Logger_CPP__
#define __Logger_CPP__

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "../include/logger.h"

Logger *Logger::null = new Logger([](const char*) -> void { return; });  

Logger::Logger(void (*core)(const char*))
{
	this->core = core;
	this->warningFlag = false;
	this->fatalFlag = false;
	this->reportFlag = false;
	this->buffer = static_cast<char*>(malloc(sizeof(char) * buffer_size));
}

void Logger::fatal(const char *format, ...)
{
	if(fatalFlag) {
		strcpy(buffer, "<Fatal> ");
		 va_list args;
		 va_start (args, format);
		 vsprintf (buffer + 7, format, args);
		 core(buffer);
		va_end(args);
	}
}

void Logger::warning(const char *format, ...)
{
	if(warningFlag) {
		strcpy(buffer, "<Warning> ");
		 va_list args;
		 va_start (args, format);
		 vsprintf (buffer + 11, format, args);
		 core(buffer);
		va_end(args);
	}
}

void Logger::report(const char *format, ...)
{
	if(reportFlag) {
		strcpy(buffer, "<Report> ");
		 va_list args;
		 va_start (args, format);
		 vsprintf (buffer + 9, format, args);
		 core(buffer);
		va_end(args);
	}
}

Logger& Logger::setFlags(bool err, bool war, bool rep)
{
	fatalFlag = err;
	reportFlag = rep;
	warningFlag = war;
	return *this;
}

Logger::~Logger()
{
	free(buffer);
}

#endif