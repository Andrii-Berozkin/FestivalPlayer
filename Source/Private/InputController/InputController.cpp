// Copyright Andrii Berozkin, All Rights Reserved.



#include <Public/InputController/InputController.h>
#include <mutex>



std::mutex InputMutex;

#define INPUT_CYCLEEMPTY_SLEEP_MS 0



UInputController::UInputController()
{
	InputQueue.clear();

	bFinishLoop = false;
	bEnableDebugLettering = false;

	//ReactionDelay_ms = 615;
}

UInputController::~UInputController()
{
	bFinishLoop = true;


	if(AsyncKeyInputHandle.valid()) 
	{ 
		InputMutex.lock();
		bFinishLoop = true;
		InputMutex.unlock();
		AsyncKeyInputHandle.wait();

		printf("Async input cycle : stopped\n");
	}
}

void UInputController::AddInputToQueue(const INPUT& NewInput)
{
	InputMutex.lock();

	InputQueue.push_back(NewInput);


	InputMutex.unlock();
}

void UInputController::AddInputToQueue(const std::vector<INPUT>& NewInput)
{
	InputMutex.lock();
	for (int i = 0; i < NewInput.size(); i++)
	{
		InputQueue.push_back(NewInput[i]);

	}
	InputMutex.unlock();

	ExecQueue();
}

void UInputController::ExecQueue()
{

	auto SendInputLambda([](UInputController* InputController, const std::vector<INPUT> InputArr)
		{
			InputMutex.lock();
			std::vector<INPUT> CopyInput(InputArr);
			InputMutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(InputController->ReactionDelay_ms));

			SendInput(CopyInput.size(), CopyInput.data(), sizeof(INPUT));


			if (InputController->GetEnableDebugLettering())
			{
				for (int i = 0; i < CopyInput.size(); i++)
				{
					if (CopyInput[i].ki.dwFlags != KEYEVENTF_KEYUP)
					{
						printf("%c", char(CopyInput[i].ki.wVk));
					}
				}
				printf("\n");
			}
		});

	std::thread t1(SendInputLambda, this, InputQueue);
	t1.detach();

	
	InputMutex.lock();

	InputQueue.clear();

	InputMutex.unlock();
}