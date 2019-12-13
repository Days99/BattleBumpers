// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClient.h"
#include "MenuLevelScript.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPv4Address.h"
#include "IPAddress.h"
#include "RunnableThread.h"

TCPClient::TCPClient(AMenuLevelScript* Level)
{
	Thread = FRunnableThread::Create(this, TEXT("TCP Client Thread"), 0, TPri_Normal);
	GameLevel = Level;

}

TCPClient::~TCPClient()
{
	running = false;
	delete Thread;
}

bool TCPClient::Init()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Default"), false);
	int newSize = 0;
	Socket->SetReceiveBufferSize(1024, newSize);
	FIPv4Address MathmackingServerIP(127, 0, 0, 1);
	TSharedRef<FInternetAddr> MatchmackingServer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	MatchmackingServer->SetIp(MathmackingServerIP.Value);
	MatchmackingServer->SetPort(8856);

	connected = Socket->Connect(*MatchmackingServer);
	if (connected) {
		UE_LOG(LogTemp, Log, TEXT("Connected"));
	}
	return true;
}

uint32 TCPClient::Run()
{
	while (running)
	{

	} 
	return uint32();
}

void TCPClient::Stop()
{
	running = false;
}
