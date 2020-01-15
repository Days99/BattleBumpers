// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClient.h"
#include "MyLevelScriptActor.h"
#include "Sockets.h"
#include "Interfaces/IPv4/IPv4Address.h"

TCPClient::TCPClient(AMyLevelScriptActor *gLevel)
{
	Thread = FRunnableThread::Create(this, TEXT("TCPClientThread"),
		0, TPri_Normal);
	GameLevel = gLevel;
}

TCPClient::~TCPClient()
{
	running = false;
	delete Thread;
}

bool TCPClient::Init()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->
		CreateSocket(NAME_Stream, TEXT("default"), false);
	int NewSize = 0;
	Socket->SetReceiveBufferSize(1024, NewSize);
	FIPv4Address MatchmakingServerIP(40,113,114,149);
	TSharedRef<FInternetAddr>MatchmakingServer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	MatchmakingServer->SetIp(MatchmakingServerIP.Value);
	MatchmakingServer->SetPort(8856);

	connected = Socket->Connect(*MatchmakingServer);

	if (connected)
	{
		UE_LOG(LogTemp, Log, TEXT("CONNECTED!!!!"));
		FString message = TEXT("g|#");
		TCHAR* convertedMessage = message.GetCharArray().GetData();
		int32 size = FCString::Strlen(convertedMessage);
		int32 sent = 0;
		bool success = Socket->Send((uint8*)TCHAR_TO_UTF8(convertedMessage), 
			size, sent);
		if (success)
		{
			UE_LOG(LogTemp, Log, TEXT("MESSAGE SENT!!!!"));
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("MESSAGE NOT SENT!!!!"));
			return false;
		}
	}
	return false;
}

uint32 TCPClient::Run()
{
	running = true;
	TArray<uint8> ReceivedData;
	GameLevel->UpdateSessionList("s|null|");
	while (running)
	{
		uint32 size = 0;
		if (Socket->HasPendingData(size))
		{
			ReceivedData.Init(0, size);
			int32 readsize = 0;
			Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), readsize);
			FString ServerMessage = FString(UTF8_TO_TCHAR(ReceivedData.GetData()));
			UE_LOG(LogTemp, Log, TEXT("RECEIVED: %s"), *ServerMessage);
			if (ServerMessage[0] == 's')
			{
				//UPDATE SESSION LIST
				GameLevel->UpdateSessionList(ServerMessage);
			}
			else if (ServerMessage[0] == 'o')
			{
				//HOST SESSION
				running = false;
				TArray<FString> Out;
				ServerMessage.ParseIntoArray(Out, TEXT("|"), true);
				GameLevel->StartGameHost(FCString::Atoi(*Out[1]));				
			}

		}
	}
	UE_LOG(LogTemp, Log, TEXT("THREAD ENDED!!!!!!!!"));
	return 0;
}

void TCPClient::Stop()
{
	running = false;
}

bool TCPClient::IsConnected()
{
	return connected;
}

void TCPClient::HostNewGame(FString sname, FString sport)
{
	//h|SESSION_NAME|SESSION_IP|SESSION_PORT|#
	bool canBind = false;

		FString message = TEXT("h|" + sname + "|" + "40.113.114.149" + "|" + sport+ "|#");
		TCHAR* convertedMessage = message.GetCharArray().GetData();
		int32 size = FCString::Strlen(convertedMessage);
		int32 sent = 0;
		bool success = Socket->Send((uint8*)TCHAR_TO_UTF8(convertedMessage), size, sent);
		if (success)
		{
			UE_LOG(LogTemp, Log, TEXT("MESSAGE SENT!!!!"));
		}

}

void TCPClient::RequestSessionList()
{
	FString message = TEXT("g|#");
	TCHAR* convertedMessage = message.GetCharArray().GetData();
	int32 size = FCString::Strlen(convertedMessage);
	int32 sent = 0;
	bool success = Socket->Send((uint8*)TCHAR_TO_UTF8(convertedMessage),
		size, sent);
	if (success)
	{
		UE_LOG(LogTemp, Log, TEXT("MESSAGE SENT!!!!"));
	}
}
