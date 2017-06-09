#include "Core_PCH.h"

namespace ServerEngine
{

	ModuleManager::~ModuleManager()
	{

	}

	ModuleManager& ModuleManager::Get()
	{
		static ModuleManager instance;
		return instance;
	}

	void ModuleManager::RegisterModule(string moduleName, IModuleFactoryPtr moduleFactory)
	{
		ModuleManager& instance = ModuleManager::Get();

		if (instance.m_moduleFactories.find(moduleName) == instance.m_moduleFactories.end())
		{
			instance.m_moduleFactories[moduleName] = moduleFactory;
		}
	}

	IModuleInterfacePtr ModuleManager::GetModule(string moduleName)
	{
		if (m_moduleInstances.find(moduleName) != m_moduleInstances.end())
		{
			return m_moduleInstances[moduleName];
		}
		else
		{
			return nullptr;
		}
	}

	bool ModuleManager::IsModuleLoaded(string moduleName)
	{
		return m_moduleInstances.find(moduleName) != m_moduleInstances.end();
	}

	IModuleInterfacePtr ModuleManager::LoadModule(string moduleName)
	{
		IModuleInterfacePtr moduleInstance = nullptr;

		IModuleFactoryPtr moduleFactory;
		if (m_moduleFactories.find(moduleName) != m_moduleFactories.end())
		{
			moduleInstance = m_moduleFactories[moduleName]->CreateModule();
			moduleInstance->StartModule();

			m_moduleInstances[moduleName] = moduleInstance;
		}
		return moduleInstance;
	}

	void ModuleManager::UnloadModulesAtShutdown()
	{
		for (auto modulePair : m_moduleInstances)
		{
			modulePair.second->ShutdownModule();
		}
		m_moduleInstances.clear();
		m_moduleFactories.clear();
	}

	IModuleInterfacePtr ModuleManager::FindModule(string moduleName)
	{
		if (m_moduleInstances.find(moduleName) != m_moduleInstances.end())
		{
			return m_moduleInstances[moduleName];
		}
		else
		{
			return nullptr;
		}
	}
}
