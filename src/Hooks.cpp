#include "Hooks.h"
#include "ScreenshotHandler.h"

namespace Hooks
{
	void Hooks::Install()
	{
		ScreenshotHandler::InstallHooks();
		logger::info("Installed all hooks");
	}
}
