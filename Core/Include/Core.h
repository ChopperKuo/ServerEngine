#pragma once

// Standard header files.
#include <string>
#include <memory>
#include <map>

using namespace std;

// Core header files.
#include "ModuleInterface.h"
#include "ModuleFactory.h"
#include "ModuleManager.h"

// Core Modules.
#include "NetModule.h"

#define SAFE_DELETE(ptr) if(ptr != nullptr) { delete ptr; ptr = nullptr; }
#define SAFE_DELETE_ARRAY(ptr) if(ptr != nullptr) { delete[] ptr; ptr = nullptr; }
