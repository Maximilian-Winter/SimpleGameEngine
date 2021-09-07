#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "DataFileContainer.h"
#include "Singleton.h"


class Logger : public Singleton<Logger>
{
	friend class Singleton <Logger>;
public:
	Logger()
	{
	}
	~Logger()
	{
	}

	void Log(std::string logMessageName, std::string logMessage)
	{
		LogFile.AddStringValueByName("Log", logMessageName + std::to_string(i), logMessage);
		i++;
	}

	void SaveLog()
	{
		LogFile.SaveDataContainerToFile("Log.log", "Log File");
	}
protected:
	DataFileContainer LogFile;
	int i = 0;
};

#endif