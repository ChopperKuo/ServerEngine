#pragma once

namespace ServerEngine
{
	class NetModule : public IModuleInterface
	{
	private:
		virtual void StartModule() override;

		virtual void ShutdownModule() override;
	};
}
