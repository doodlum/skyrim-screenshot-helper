#include "ScreenshotHandler.h"
#include "WStringUtil.h"

void ScreenshotHandler::ResetName()
{
	filename = WStringUtil::ConvertStringToWString("");
}


void ScreenshotHandler::Add(const char* a_str)
{
	filename += WStringUtil::ConvertStringToWString(a_str);
}


std::string ScreenshotHandler::FilenameToString()
{
	return WStringUtil::ConvertWStringToString(filename);
}


void ScreenshotHandler::FilenameToCString(char* dest)
{
	strcpy_s(dest, FILENAME_MAX_LENGTH, WStringUtil::ConvertWStringToString(filename).c_str());
}


const char* ScreenshotHandler::GetCurrentLocationName(RE::Actor* a_actor)
{
	for (auto location = a_actor->GetCurrentLocation(); location; location = location->parentLoc) {
		auto locationName = location->GetName();
		if (locationName && strlen(locationName))
			return locationName;
	}

	for (auto worldspace = a_actor->GetWorldspace(); worldspace; worldspace = worldspace->parentWorld) {
		auto worldspaceName = worldspace->GetName();
		if (worldspaceName && strlen(worldspaceName))
			return worldspaceName;
	}

	return nullptr;
}


void ScreenshotHandler::ModifyFileName(char* dest)
{
	auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
	auto Calendar = RE::Calendar::GetSingleton();

	if (PlayerCharacter && Calendar) {
		ResetName();
		Add("Screenshots\\");

		if (PlayerCharacter->GetName() && PlayerCharacter->GetRace() && PlayerCharacter->GetRace()->GetName()) {
			Add(PlayerCharacter->GetName());
			Add(" - ");
			Add(PlayerCharacter->GetRace()->GetName());
			Add("\\");
		} else {
			logger::error("Failed to access player information");
		}

		if (!std::filesystem::exists(filename))
			std::filesystem::create_directories(filename);

		auto currentLocationName = GetCurrentLocationName(PlayerCharacter);
		if (currentLocationName) {
			Add(currentLocationName);
			Add(" - ");
		} else {
			logger::warn("Failed to find a location name");
		}

		char timedate[504];
		Calendar->GetTimeDateString(timedate, 0x200u, 1);
		Add(timedate);


		if (std::filesystem::exists(FilenameToString() + ".png")) {
			int fixName = 1;
			while (std::filesystem::exists(FilenameToString() + " - " + std::to_string(fixName) + ".png"))
				fixName++;
			Add(" - ");
			Add(std::to_string(fixName).c_str());
		}

		Add(".png");

		FilenameToCString(dest);
	}
}


const wchar_t* ScreenshotHandler::FinalFileName()
{
	filename = WStringUtil::ReplaceWCSWithPattern(filename, L":", L"꞉");

	logger::info(FMT_STRING("Attempting to create save file at {}"sv), FilenameToString());

	return filename.c_str();
}
