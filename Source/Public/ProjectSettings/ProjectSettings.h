// Copyright Andrii Berozkin, All Rights Reserved.


#pragma once

#include <opencv2/core/core.hpp>



#define SETTINGS_RELATIVE_PATH "FestivalPlayer_Settings.json"
#define CAPTURE_SETTINGS "CaptureSettings"
#define CAPTURE_SIZE_FROM_TOTAL "CaptureSizeFromTotal"
#define SIZE_MULTIPLIER_X "SizeMultiplierX"
#define SIZE_MULTIPLIER_Y "SizeMultiplierY"
#define CAPTURE_POS "CapturePos"
#define SCREEN_PERCENT_X "ScreenPercentX"
#define SCREEN_PERCENT_Y "ScreenPercentY"
#define MUSIC_NOTE_SIZE_X_MULTIPLIER "MusicNoteSizeXMultiplier"
#define FILTER_SETTINGS "FilterSettings"
#define COLOR_TOLERANCE "ColorTolerance"
#define MUSIC_NOTE_TARGET_COLOR "MusicNoteTargetColor"
#define INPUT_SETTINGS "InputSettings"
#define BENABLE_DEBUG "bEnableDebug"
#define REACTION_DELAY_MS "ReactionDelayMS"



struct FCaptureSettings
{
	//	CaptureSizeFromTotal
	double SizeMultiplierX;
	double SizeMultiplierY;

	//	CapturePos
	double ScreenPercentX;
	double ScreenPercentY;

	double MusicNoteSizeXMultiplier;

	FCaptureSettings()
		: SizeMultiplierX(0.230), SizeMultiplierY(0.0150),
		ScreenPercentX(0.503), ScreenPercentY(0.5), MusicNoteSizeXMultiplier(0.2)
	{

	};
};

struct FFilterSettings
{
	int ColorTolerance;

	cv::Point3_<uint8_t> MusicNoteTargetColor;

	FFilterSettings()
		: ColorTolerance(11), MusicNoteTargetColor(250, 235, 250)
	{
	};
};

struct FInputSettings
{
	bool bEnableDebug;
	int ReactionDelay;

	FInputSettings()
		: bEnableDebug(true), ReactionDelay(615)
	{

	};
};

class UProjectSettings
{
public:

	FCaptureSettings CaptureSettings;
	FFilterSettings FilterSettings;
	FInputSettings InputSettings;

	uint ScreenResolutionX;
	uint ScreenResolutionY;

private:

public:


	// Constructors & Destructors
	UProjectSettings();

	~UProjectSettings();

private:

	void GetScreenResolution(uint& ScreenX, uint& ScreenY);

	void InitSettings();

	bool CreateDefaultJson();
};