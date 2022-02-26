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

	static const char*  GetCurrentLocationName(RE::Actor* a_actor);

	#define FILENAME_MAX_LENGTH 257

	std::wstring   filename;

	std::string FilenameToString();
	void        FilenameToCString(char* a_str);

	void           ResetName();
	void           Add(const char* a_str);
	void           ModifyFileName(char* dest);
	const wchar_t* FinalFileName();


protected:

	struct Hooks
	{
		struct TakeScreenshot
		{
			static INT32 thunk(INT64 a1, INT64 a2, char* dest, UINT32 type)
			{
				GetSingleton()->ModifyFileName(dest);
				return func(a1, a2, dest, type);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct WriteScreenshot
		{
			static INT64 thunk(INT64 a1, UINT32 a2, INT64 a3, const wchar_t* dest)
			{
				return func(a1, a2, a3, dest ? GetSingleton()->FinalFileName() : dest);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};


		static void Install()
		{
#if defined(SKYRIMAE)
			REL::Relocation<std::uintptr_t> TakeScreenshot_hook	{ REL::ID(36853), 0x69 };
			REL::Relocation<std::uintptr_t> WriteScreenshot_hook{ REL::ID(77406), 0x143 };
#else
			REL::Relocation<std::uintptr_t> TakeScreenshot_hook	{ REL::ID(35882), 0x73 };
			REL::Relocation<std::uintptr_t> WriteScreenshot_hook{ REL::ID(75598), 0x13B };
#endif
			stl::write_thunk_call<TakeScreenshot>(TakeScreenshot_hook.address());
			stl::write_thunk_call<WriteScreenshot>(WriteScreenshot_hook.address());

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
