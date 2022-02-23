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

void ScreenshotHandler::ModifyFileName(char dest[257])
{
	auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
	auto Calendar = RE::Calendar::GetSingleton();

	if (PlayerCharacter && Calendar) {
		std::string filename = "Screenshots\\";

		filename += PlayerCharacter->GetName();
		filename += "\\";

		std::filesystem::create_directories(filename);

		auto location = PlayerCharacter->GetCurrentLocation();

		filename += location ? location->GetName() : PlayerCharacter->GetWorldspace()->GetName();
		filename += " - ";

		filename += NumberToOrdinal(static_cast<int>(Calendar->GetDay()));
		filename += " of " + Calendar->GetMonthName() + ", ";

		float time = Calendar->GetHour();
		int   hour = static_cast<int>(time);
		int   minute = static_cast<int>((time - hour) * 60);

		std::stringstream ss;
		ss << hour << '.' << std::setw(2) << minute;
		filename += ss.str();
		filename += hour < 12 ? " AM" : " PM";

		filename += ".png";
		logger::info(FMT_STRING("filename {}"sv), filename);

		strcpy_s(dest, 257, filename.c_str());
	}
}
