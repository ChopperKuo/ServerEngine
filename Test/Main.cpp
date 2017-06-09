#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "Core.h"
#include "ModuleRegistry.h"

using namespace ServerEngine;

void main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SETUP_MODULES();

	NetModule& Module = ModuleManager::GetModuleChecked<NetModule>();

	SHUTDOWN_MODULES();

	system("pause");
}
