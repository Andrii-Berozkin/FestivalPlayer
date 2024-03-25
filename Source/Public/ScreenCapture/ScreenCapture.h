// Copyright Andrii Berozkin, All Rights Reserved.



#pragma once


#include <Windows.h>
#include <wingdi.h>
#include <opencv2/core/core.hpp>



class UScreenCapture
{

public:

	HWND FocusWindow;

	std::function<void(const cv::Mat&)> CaptureCallback;

private:

	std::mutex* ThreadLock;

	std::thread CaptureThread;

	uint Planes;
	uint CompressionBits;

	cv::Range CaptureDimensionsXY;
	cv::Range CaptureInitPosXY;

	HDC HScreen;
	HDC HCompatibleScreen;

	HBITMAP HCaptureBitmap;
	BITMAPINFO bmp;

	cv::Mat OutMat;

	DWORD BltAttributes;
	UINT GetBitsColorUsage;


	//	=======

	bool bInitialized;

	bool bIsPaused;

public:

	//	Constructors & Destructors
	UScreenCapture();

	UScreenCapture(const cv::Range& CaptureDimensionsXY, const cv::Range& CaptureInitPosXY);

	UScreenCapture(const HWND FocusWindow, const cv::Range& CaptureDimensionsXY, const cv::Range& CaptureInitPosXY);

	~UScreenCapture();
	//	=======

	const FORCEINLINE bool IsOnPause() { return bIsPaused; }

	FORCEINLINE void SetCapturingPause(const bool& IsPaused) { bIsPaused = IsPaused; }

	void StartCapturing();


	//	Getters & Setters
	const FORCEINLINE std::mutex* GetThreadMutex() { return ThreadLock; }

	void SetCustomThreadMutex(std::mutex* NewThreadMutex);

	const FORCEINLINE HBITMAP* GetCaptureBitmap() { return &HCaptureBitmap; }

	const FORCEINLINE cv::Range GetCaptureDimensions() { return CaptureDimensionsXY; }

	FORCEINLINE void SetCaptureDimensions(const cv::Range& NewCaptureDimensions) { CaptureDimensionsXY = NewCaptureDimensions; }

	const FORCEINLINE cv::Range GetCaptureInitPos() { return CaptureInitPosXY; }

	FORCEINLINE void SetCaptureInitPos(const cv::Range& NewCaptureInitPos) { CaptureInitPosXY = NewCaptureInitPos; }
	//	=======

private:

	void InitScreen();

	void CaptureScreen();

	void CaptureCycle();
};