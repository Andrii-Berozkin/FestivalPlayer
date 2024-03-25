// Copyright Andrii Berozkin, All Rights Reserved.



#include <Public/ScreenCapture/ScreenCapture.h>
#include <future>



#define ONPAUSE_WAIT_TIME_MS 250



UScreenCapture::UScreenCapture()
{
	HWND hwnd = NULL;

	cv::Range CaptureXY(256, 256);
	cv::Range InitPosXY(0, 0);

	UScreenCapture(hwnd, CaptureXY, InitPosXY);
}

UScreenCapture::UScreenCapture(const cv::Range& CaptureDimensionsXY, const cv::Range& CaptureInitPosXY)
{
	HWND hwnd = NULL;

	UScreenCapture(hwnd, CaptureDimensionsXY, CaptureInitPosXY);
}

void TestToPtr(const cv::Mat& OutMat)
{

}

UScreenCapture::UScreenCapture(const HWND FocusWindow, const cv::Range& CaptureDimensionsXY, const cv::Range& CaptureInitPosXY)
	:FocusWindow(FocusWindow), CaptureDimensionsXY(CaptureDimensionsXY), CaptureInitPosXY(CaptureInitPosXY)
{
	ThreadLock = new std::mutex;
	CaptureCallback;

	OutMat = cv::Mat(CaptureDimensionsXY.end, CaptureDimensionsXY.start, CV_8UC4);

	Planes = 1;
	CompressionBits = 32;

	BltAttributes = SRCCOPY;
	GetBitsColorUsage = DIB_RGB_COLORS;

	bmp = { 0 };
	bmp.bmiHeader.biSize = sizeof(bmp.bmiHeader);
	bmp.bmiHeader.biWidth = CaptureDimensionsXY.start;
	bmp.bmiHeader.biHeight = CaptureDimensionsXY.end;
	bmp.bmiHeader.biPlanes = Planes;
	bmp.bmiHeader.biBitCount = CompressionBits;
	bmp.bmiHeader.biCompression = BI_RGB;

	HScreen = NULL;
	HCompatibleScreen = NULL;
	HCaptureBitmap = NULL;

	bInitialized = false;
	bIsPaused = false;
}

UScreenCapture::~UScreenCapture()
{
	CaptureThread.join();

	ReleaseDC(FocusWindow, HScreen);
	DeleteDC(HCompatibleScreen);
	DeleteObject(HCaptureBitmap);
}

void UScreenCapture::StartCapturing()
{
	//CaptureCycle();
	
	auto CaptureLambda([](UScreenCapture* SC) 
		{
			SC->CaptureCycle();
		});

	CaptureThread = std::thread(CaptureLambda, this);
	CaptureThread.detach();
}

void UScreenCapture::InitScreen()
{
	bInitialized = false;

	HScreen = GetDC(FocusWindow);
	HCompatibleScreen = CreateCompatibleDC(HScreen);


	HCaptureBitmap = CreateCompatibleBitmap(HScreen, CaptureDimensionsXY.start, CaptureDimensionsXY.end);
	SelectObject(HCompatibleScreen, HCaptureBitmap);

	if (HCaptureBitmap && HCompatibleScreen) { bInitialized = true; }
}

void UScreenCapture::CaptureScreen()
{
	//	Actual capture
	BitBlt(HCompatibleScreen, 0, 0, CaptureDimensionsXY.start, CaptureDimensionsXY.end, HScreen,
		CaptureInitPosXY.start, CaptureInitPosXY.end, BltAttributes);


	if (ThreadLock) { ThreadLock->lock(); }

	GetDIBits(HCompatibleScreen, HCaptureBitmap, 0, CaptureDimensionsXY.end, OutMat.data, &bmp, GetBitsColorUsage);

	if (ThreadLock) { ThreadLock->unlock(); }

	if (CaptureCallback) { CaptureCallback(OutMat); }
}

void UScreenCapture::CaptureCycle()
{
	InitScreen();

	if (!bInitialized) { printf("Can`t start capturing cycle : Initialization FAILED\n"); return; }

	while (true)
	{
		if (!bIsPaused) { CaptureScreen(); }
		else { std::this_thread::sleep_for(std::chrono::milliseconds(ONPAUSE_WAIT_TIME_MS)); }
	}
}

void UScreenCapture::SetCustomThreadMutex(std::mutex* NewThreadMutex)
{
	if (ThreadLock) { delete ThreadLock; }

	ThreadLock = NewThreadMutex;
}