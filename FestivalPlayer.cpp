// Copyright Andrii Berozkin, All Rights Reserved.




#include <vector>

#include <iostream>

#include <opencv2/opencv.hpp>


#include <Public/ScreenCapture/ScreenCapture.h>
#include <Public/ImageResolver/ImageResolver.h>
#include <Public/InputController/InputController.h>
#include <Public/ProjectSettings/ProjectSettings.h>



UInputController* InputControllerPtr;
UScreenCapture* SC_Capture;

float MusicNoteSizeXMultiplier;
cv::Point3_<uint8_t> TargetColor;
int ColorTolerance;



void GetFortniteFestivalMusicDimensions(const int& ScreenX, const int& ScreenY, cv::Range& OutMusic, cv::Range& OutScreenStartPos,const FCaptureSettings& CaptureSettings)
{
    uint ActivePlaygroundX = ScreenX * CaptureSettings.SizeMultiplierX;
    uint ActivePlaygroundY = ScreenY * CaptureSettings.SizeMultiplierY;
    
    
    uint MusicDimensionX = (ScreenX * CaptureSettings.ScreenPercentX) - ActivePlaygroundX * 0.5f;
    uint MusicDimensionY = (ScreenY * CaptureSettings.ScreenPercentY) - ActivePlaygroundY * 0.5f;

    OutMusic = cv::Range(ActivePlaygroundX, ActivePlaygroundY);
    OutScreenStartPos = cv::Range(MusicDimensionX, MusicDimensionY);
}

cv::Point3_<uint8_t> NoteColorV4(250, 235, 250);  // Best one

void CapturingCallback(const cv::Mat& OutMat)
{
    cv::Mat CopyOut;
    OutMat.copyTo(CopyOut);


    /** Define columns size for N (5 in this case) equal container  */
    int MusicNoteWidthX = CopyOut.cols * MusicNoteSizeXMultiplier;
    int MusicNoteHeightY = CopyOut.rows;



    //  Order Matters !!!
    std::vector<cv::Rect> NoteRect(5);
    std::vector<cv::Mat> MusicNotesMat(5);
    std::vector<bool> ActiveButtons(5);
    std::vector<WORD> ButtonsSign(5);

    ButtonsSign[0] = 'D';
    ButtonsSign[1] = 'F';
    ButtonsSign[2] = 'J';
    ButtonsSign[3] = 'K';
    ButtonsSign[4] = 'L';

    /** Divide capture image to N (5 in this case) mat container for each music note line   */
    for (int MusicNote = 0; MusicNote < 5; MusicNote++)
    {
        NoteRect[MusicNote] = cv::Rect(MusicNoteWidthX * MusicNote, 0, MusicNoteWidthX, MusicNoteHeightY);
        MusicNotesMat[MusicNote] = CopyOut(NoteRect[MusicNote]);

        //cv::imshow("asd", MusicNotesMat[MusicNote]);
        //cv::waitKey(0);
    }

    /** Filter each section to find a target color with allowed tolerance in color value    */
    for (int i = 0; i < 5; i++)
    {
        ActiveButtons[i] = UImageResolverHelper::FindColorInMat(MusicNotesMat[i], TargetColor, ColorTolerance);
    }

    

    std::vector<INPUT> NewInputArr(5);

    /** Send found keys to controller for executing input   */
    //  Find a way to reduce KEYEVENTF_KEYUP calls, as it`s make it every frame if target colors not found
    for (int i = 0; i < ButtonsSign.size(); i++)
    {
        NewInputArr[i].type = INPUT_KEYBOARD;
        NewInputArr[i].ki.wVk = ButtonsSign[i];
        NewInputArr[i].ki.dwFlags = ActiveButtons[i] ? 0 : KEYEVENTF_KEYUP;
    }

    InputControllerPtr->AddInputToQueue(NewInputArr);

}

int main()
{
    UProjectSettings* ProjectSettings = new UProjectSettings();

    //  Fulfill basic settings from a loaded json
    MusicNoteSizeXMultiplier = ProjectSettings->CaptureSettings.MusicNoteSizeXMultiplier;
    TargetColor = ProjectSettings->FilterSettings.MusicNoteTargetColor;
    ColorTolerance = ProjectSettings->FilterSettings.ColorTolerance;

    /** Create and set settings for input controller (works async)    */
    InputControllerPtr = new UInputController();
    InputControllerPtr->SetEnableDebugLettering(ProjectSettings->InputSettings.bEnableDebug);
    InputControllerPtr->ReactionDelay_ms = ProjectSettings->InputSettings.ReactionDelay;


    HWND FocusWindow = NULL;

    cv::Range CaptureDimesnions;
    cv::Range InitCapturePos;

    GetFortniteFestivalMusicDimensions(ProjectSettings->ScreenResolutionX, ProjectSettings->ScreenResolutionY, 
        CaptureDimesnions, InitCapturePos, ProjectSettings->CaptureSettings);


    SC_Capture = new UScreenCapture(FocusWindow, CaptureDimesnions, InitCapturePos);
    SC_Capture->CaptureCallback = CapturingCallback;
    SC_Capture->StartCapturing();


    while (!SC_Capture->IsOnPause())
    {
        //  Loop while capture working
    }

    //  At this point it`s doing nothing
    /** Propper stopping and destructing InputController    */
    InputControllerPtr->SetFinishLoop(true);
    delete InputControllerPtr;
}