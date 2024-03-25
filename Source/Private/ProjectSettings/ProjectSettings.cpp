// Copyright Andrii Berozkin, All Rights Reserved.



#include <Public/ProjectSettings/ProjectSettings.h>

#include <Windows.h>
#include <fstream>
#include <nlohmann/json.hpp>



UProjectSettings::UProjectSettings() 
	: CaptureSettings(), FilterSettings(), InputSettings(), ScreenResolutionX(1920), ScreenResolutionY(1080)
{

	InitSettings();

	GetScreenResolution(ScreenResolutionX, ScreenResolutionY);
}

UProjectSettings::~UProjectSettings()
{

}

void UProjectSettings::GetScreenResolution(uint& ScreenX, uint& ScreenY)
{
	DEVMODE DevMode;
	DevMode.dmSize = sizeof(DEVMODE);

	EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &DevMode);

	ScreenX = DevMode.dmPelsWidth;
	ScreenY = DevMode.dmPelsHeight;
}

void UProjectSettings::InitSettings()
{
	std::ifstream JsonSettingsStream(SETTINGS_RELATIVE_PATH);

	//	If settings doesn`t exists, create a default one
	if (!JsonSettingsStream.is_open())
	{
		printf("Failed to find existed settings file\n");
		CreateDefaultJson();
		return;
	}

	//	Make sure to start from the beginning
	JsonSettingsStream.seekg(0, std::ios::beg);

	nlohmann::json JsonObj = nlohmann::json::parse(JsonSettingsStream);



	//	Fulfill structures with settings

	//	Capture settings struct
	CaptureSettings.SizeMultiplierX = JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SIZE_MULTIPLIER_X];
	CaptureSettings.SizeMultiplierY = JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SIZE_MULTIPLIER_Y];

	CaptureSettings.ScreenPercentX = JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SCREEN_PERCENT_X];
	CaptureSettings.ScreenPercentY = JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SCREEN_PERCENT_Y];

	CaptureSettings.MusicNoteSizeXMultiplier = JsonObj[CAPTURE_SETTINGS][MUSIC_NOTE_SIZE_X_MULTIPLIER];

	//	Filter settings struct
	FilterSettings.ColorTolerance = JsonObj[FILTER_SETTINGS][COLOR_TOLERANCE];

	cv::Point3_<uint8_t> TempRGB(
		JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["R"],
		JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["G"],
		JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["B"]);

	FilterSettings.MusicNoteTargetColor = TempRGB;

	//	Input settings struct
	InputSettings.bEnableDebug = JsonObj[INPUT_SETTINGS][BENABLE_DEBUG];
	InputSettings.ReactionDelay = JsonObj[INPUT_SETTINGS][REACTION_DELAY_MS];

	JsonSettingsStream.close();

	printf("Settings successfully loaded from a file\n");
}

bool UProjectSettings::CreateDefaultJson()
{
	std::ofstream JsonSettingsOStream(SETTINGS_RELATIVE_PATH);

	nlohmann::json JsonObj;


	//	Fulfill json object with default values
	JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SIZE_MULTIPLIER_X] = CaptureSettings.SizeMultiplierX;
	JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SIZE_MULTIPLIER_Y] = CaptureSettings.SizeMultiplierY;

	JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SCREEN_PERCENT_X] = CaptureSettings.ScreenPercentX;
	JsonObj[CAPTURE_SETTINGS][CAPTURE_SIZE_FROM_TOTAL][SCREEN_PERCENT_Y] = CaptureSettings.ScreenPercentY;

	JsonObj[CAPTURE_SETTINGS][MUSIC_NOTE_SIZE_X_MULTIPLIER] = CaptureSettings.MusicNoteSizeXMultiplier;


	JsonObj[FILTER_SETTINGS][COLOR_TOLERANCE] = FilterSettings.ColorTolerance;
	JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["R"] = FilterSettings.MusicNoteTargetColor.x;
	JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["G"] = FilterSettings.MusicNoteTargetColor.y;
	JsonObj[FILTER_SETTINGS][MUSIC_NOTE_TARGET_COLOR]["B"] = FilterSettings.MusicNoteTargetColor.z;

	JsonObj[INPUT_SETTINGS][BENABLE_DEBUG] = InputSettings.bEnableDebug;
	JsonObj[INPUT_SETTINGS][REACTION_DELAY_MS] = InputSettings.ReactionDelay;

	JsonSettingsOStream.seekp(0, std::ios::beg);
	JsonSettingsOStream << JsonObj;
	JsonSettingsOStream.close();

	printf("Created default settings at : %s\n", SETTINGS_RELATIVE_PATH);

	return true;
}