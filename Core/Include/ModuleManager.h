#pragma once

namespace ServerEngine
{
	class ModuleManager
	{
	private:
		map<string, IModuleFactoryPtr> m_moduleFactories;
		map<string, IModuleInterfacePtr> m_moduleInstances;

	public:
		virtual ~ModuleManager();

		static ModuleManager& Get();

	public:
		static void RegisterModule(string moduleName, IModuleFactoryPtr moduleFactory);

		template<typename TModuleInterface>
		static TModuleInterface& GetModuleChecked();

	public:
		IModuleInterfacePtr GetModule(string moduleName);

		bool IsModuleLoaded(string moduleName);

		IModuleInterfacePtr LoadModule(string moduleName);

		void UnloadModulesAtShutdown();

	private:
		IModuleInterfacePtr FindModule(string moduleName);
	};

	template<typename TModuleInterface>
	inline TModuleInterface& ModuleManager::GetModuleChecked()
	{
		ModuleManager& instance = ModuleManager::Get();
		string moduleName = typeid(TModuleInterface).name();

		if (instance.IsModuleLoaded(moduleName))
		{
			return (TModuleInterface&)(*instance.GetModule(moduleName));
		}
		else
		{
			return (TModuleInterface&)(*instance.LoadModule(moduleName));
		}
	}

	extern "C" void SetupModules();

#define SETUP_MODULES() \
	ServerEngine::SetupModules()

#define SHUTDOWN_MODULES() \
	ServerEngine::ModuleManager::Get().UnloadModulesAtShutdown()

#define MODULE_MAP_BEGIN() \
extern "C" void ServerEngine::SetupModules() \
{
#define REGISTER_MODULE_WITH_FACTORY(ModuleFactory, ModuleType) \
	ServerEngine::ModuleManager::RegisterModule(typeid(ModuleType).name(), make_shared<ModuleFactory>());
#define REGISTER_MODULE(ModuleType) \
	REGISTER_MODULE_WITH_FACTORY(ModuleFactoryImpl<ModuleType>, ModuleType)
#define MODULE_MAP_END() \
}
}
