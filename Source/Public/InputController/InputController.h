// Copyright Andrii Berozkin, All Rights Reserved.



#pragma once



#include <future>
#include <Windows.h>
#include <queue>



class UInputController
{
public:

	int ReactionDelay_ms;

private:

	std::future<void> AsyncKeyInputHandle;

	std::vector<INPUT> InputQueue;

	bool bFinishLoop;
	bool bEnableDebugLettering;

public:

	//	Constructors & destructor

	UInputController();

	~UInputController();
	//	=======

	void AddInputToQueue(const INPUT& NewInput);
	void AddInputToQueue(const std::vector<INPUT>& NewInput);

	//	Getters & Setters

	const FORCEINLINE bool GetFinishLoop() { return bFinishLoop; }

	FORCEINLINE void SetFinishLoop(const bool& NewFinishLoop) { bFinishLoop = NewFinishLoop; }

	const FORCEINLINE bool GetEnableDebugLettering() { return bEnableDebugLettering; }

	FORCEINLINE void SetEnableDebugLettering(const bool& NewEnableDebugLettering) { bEnableDebugLettering = NewEnableDebugLettering; }

	//	=======

protected:

	void ExecQueue();

private:

};