#include "ScreenshotHandler.h"

std::string NumberToOrdinal(size_t number)
{
	std::string suffix = "th";
	if (number % 100 < 11 || number % 100 > 13) {
		switch (number % 10) {
		case 1:
			suffix = "st";
			break;
		case 2:
			suffix = "nd";
			break;
		case 3:
			suffix = "rd";
			break;
		}
	}
	return std::to_string(number) + suffix;
}

const char* GetCurrentLocationName(RE::PlayerCharacter* PlayerCharacter) 
{
	auto location = PlayerCharacter->GetCurrentLocation();
	while (location) {
		auto locationName = location->GetName(); 
		if (locationName && strlen(locationName) != 0) {
			return locationName;
		} else {
			location = location->parentLoc;
		}	
	}

	auto worldspace = PlayerCharacter->GetWorldspace();
	while (worldspace) {
		auto worldspaceName = location->GetName();
		if (worldspaceName && strlen(worldspaceName) != 0) {
			return worldspaceName;
		} else {
			worldspace = worldspace->parentWorld;
		}
	}

	return nullptr;
}

void ScreenshotHandler::ModifyFileName(char dest[257])
{
	auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
	auto Calendar = RE::Calendar::GetSingleton();

	if (PlayerCharacter && Calendar) {
		std::string filename = "Screenshots\\";

		if (PlayerCharacter->GetName() && PlayerCharacter->GetRace() && PlayerCharacter->GetRace()->GetName()) {
			filename += PlayerCharacter->GetName();
			filename += " - ";
			filename += PlayerCharacter->GetRace()->GetName();
			filename += "\\";
		} else {
			logger::error("Failed to access player information");
		}

		if (!std::filesystem::exists(filename))
			std::filesystem::create_directories(filename);

		auto currentLocationName = GetCurrentLocationName(PlayerCharacter);
		if (currentLocationName) {
			filename += currentLocationName;
			filename += " - ";
		} else {
			logger::warn("Failed to find a location name");
		}

		if (Calendar->GetDay() && Calendar->GetMonthName() != "Bad Month" && Calendar->GetHour()) {
			filename += NumberToOrdinal(static_cast<int>(Calendar->GetDay()));
			filename += " of " + Calendar->GetMonthName() + ", ";

			float time = Calendar->GetHour();
			int   hour = static_cast<int>(time);
			int   minute = static_cast<int>((time - hour) * 60);

			std::stringstream ss;
			ss << (hour > 12 ? hour - 12 : (hour < 1 ? hour + 12 : hour)) << '.' << std::setw(2) << std::setfill('0') << minute;
			filename += ss.str();
			filename += hour < 12 ? "AM" : "PM";
		} else {
			logger::error("Failed to access calendar");
		}

		if (std::filesystem::exists(filename + ".png")) {
			int fixName = 1;
			while (std::filesystem::exists(filename + " - " + std::to_string(fixName) + ".png"))
				fixName++;
			filename += " - " + std::to_string(fixName);
		}

		filename += ".png";
		logger::info(FMT_STRING("Save file created at {}"sv), filename);

		strcpy_s(dest, 257, filename.c_str());
	}
}
