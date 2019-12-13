// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sockets.h"
#include "Runnable.h"
#include "CoreMinimal.h"

/**
 * 
 */
class BATTLEBUMPER_API TCPClient : public FRunnable
{
public:
	TCPClient(class AMenuLevelScript* Level);
	~TCPClient();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

private:

	FRunnableThread* Thread;
	FSocket* Socket;
	FSocket* ListenerSocket;
	bool running;
	bool connected;

	class AMenuLevelScript* GameLevel;

};
