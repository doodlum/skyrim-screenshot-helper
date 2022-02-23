#pragma once

class ScreenshotHandler
{
public:

	[[nodiscard]] static ScreenshotHandler* GetSingleton()
	{
		static ScreenshotHandler singleton;
		return std::addressof(singleton);
	}

	static void InstallHooks()
	{
		Hooks::Install();
	}

	static void ModifyFileName(char dest[257]);


protected:

	struct Hooks
	{
		struct TakeScreenshot
		{
			static INT32 thunk(INT64 a1, INT64 a2, char dest[272], UINT32 type)
			{
				ModifyFileName(dest);
				return func(a1, a2, dest, type);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
#if defined(SKYRIMAE)
			REL::Relocation<std::uintptr_t> TakeScreenshot_hook	{ REL::ID(36853), 0x69 };
#else
			REL::Relocation<std::uintptr_t> TakeScreenshot_hook	{ REL::ID(35882), 0x73 };
#endif
			stl::write_thunk_call<TakeScreenshot>(TakeScreenshot_hook.address());
		}
	};


private:
	constexpr ScreenshotHandler() noexcept = default;
	ScreenshotHandler(const ScreenshotHandler&) = delete;
	ScreenshotHandler(ScreenshotHandler&&) = delete;

	~ScreenshotHandler() = default;

	ScreenshotHandler& operator=(const ScreenshotHandler&) = delete;
	ScreenshotHandler& operator=(ScreenshotHandler&&) = delete;


};
