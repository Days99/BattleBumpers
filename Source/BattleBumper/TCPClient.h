// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SocketSubSystem.h"
#include "CoreMinimal.h"
#include "Sockets.h"
#include "Runnable.h"
#include "IPv4Address.h"

/**
 * 
 */
class BATTLEBUMPER_API TCPClient : public FRunnable
{
public:
	TCPClient(class AMyLevelScriptActor *gLevel);
	~TCPClient();
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	bool IsConnected();
	void HostNewGame(FString sname, FString sport);
	void RequestSessionList();

private:
	FRunnableThread *Thread;
	FSocket *Socket;
	FSocket *ListenerSocket;
	bool running;
	bool connected;
	class AMyLevelScriptActor *GameLevel;



};
