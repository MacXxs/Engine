#pragma once

#include "Globals.h"

#include <queue>
#include <string>

#define ENGINE_LOG(format, ...) engineLog->Log(__FILE__, __LINE__, format, __VA_ARGS__);

class EngineLog
{
public:

	EngineLog() {};

	~EngineLog()
	{
		logLines = {};
	}

	void Log(const char file[], int line, const char* format, ...);

public:
	std::queue<std::string> logLines;
};

extern EngineLog* engineLog;