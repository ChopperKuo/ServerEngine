#pragma once

namespace ServerEngine
{
	class IModuleInterface
	{
	public:
		virtual void StartModule() = 0;
		virtual void ShutdownModule() = 0;
	};

	typedef shared_ptr<IModuleInterface> IModuleInterfacePtr;
}
