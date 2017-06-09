#pragma once

namespace ServerEngine
{
	class IModuleFactory
	{
	public:
		virtual IModuleInterfacePtr CreateModule() = 0;
	};

	typedef shared_ptr<IModuleFactory> IModuleFactoryPtr;

	template<typename TModule>
	class ModuleFactoryImpl : public IModuleFactory
	{
	private:
		virtual IModuleInterfacePtr CreateModule()
		{
			return make_shared<TModule>();
		}
	};
}
