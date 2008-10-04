// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "..\stdafx.h"

#include "targetver.h"

#include <cppunit\CompilerOutputter.h>
#include <cppunit\TestResultCollector.h>
#include <cppunit\extensions\TestFactoryRegistry.h>
#include <cppunit\ui\text\TestRunner.h>
#include <cppunit\TestCase.h>
#include <cppunit\extensions\HelperMacros.h>

#ifdef _DEBUG
  #pragma comment(lib, "cppunit\\lib\\cppunitd.lib")
#else
  #pragma comment(lib, "cppunit\\lib\\cppunit.lib")
#endif

#pragma comment(lib, "wininet.lib")