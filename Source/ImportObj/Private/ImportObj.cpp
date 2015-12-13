#include "ImportObjPCH.h"

#include "ActorEditorUtils.h"
#include "UnrealEd.h"
#include "m2uAssetHelper.h"

ImportObjImpl::ImportObjImpl()
	: Client(NULL)
	, TcpListener(NULL)
{
}
 
void ImportObjImpl::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("ImportObj loaded"));
	if (!GIsEditor)
	{
		return;
	}

	ResetConnection(IMPORTOBJ_PORT);
	TickObject = new Fm2uTickObject(this);

	//IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	//UE_LOG(LogTemp, Warning, TEXT("importing obj..."));

	TArray<FString> sourceFiles;
	sourceFiles.Add(FString(TEXT("D:/Temp/shuttle.obj")));

	//assetTools.ImportAssets(sourceFiles, FString(TEXT("/Game/Test")));
	//assetTools.ImportAssets(FString(TEXT("/Game/Test")));

	//m2uAssetHelper::ImportAssets(sourceFiles, FString(TEXT("/Game/Test")));

	//UE_LOG(LogTemp, Warning, TEXT("done importing"));
}

void ImportObjImpl::ShutdownModule()
{
}

bool ImportObjImpl::GetMessage(FString & Result)
{
	// get all data from client and create one long FString from it
	// return the result when the client is out of pending data.
	//FString Result;
	uint32 DataSize = 0;
	while (Client->HasPendingData(DataSize) && DataSize > 0)
	{
		//UE_LOG(LogM2U, Log, TEXT("pending data size %i"), DataSize);
		// create data array to read from client
		//FArrayReaderPtr Data = MakeShareable(new FArrayReader(true));
		FArrayReader Data;
		Data.SetNumUninitialized(DataSize);

		int32 BytesRead = 0;
		// read pending data into the Data array reader
		if (Client->Recv(Data.GetData(), Data.Num(), BytesRead))
		{
			//UE_LOG(LogM2U, Log, TEXT("DataNum %i, BytesRead: %i"), Data.Num(), BytesRead);

			// the data we receive is supposed to be ansi, but we will work with TCHAR, so we have to convert
			int32 DestLen = TStringConvert<ANSICHAR, TCHAR>::ConvertedLength((char*)(Data.GetData()), Data.Num());
			//UE_LOG(LogM2U, Log, TEXT("DestLen will be %i"), DestLen);
			TCHAR* Dest = new TCHAR[DestLen + 1];
			TStringConvert<ANSICHAR, TCHAR>::Convert(Dest, DestLen, (char*)(Data.GetData()), Data.Num());
			Dest[DestLen] = '\0';

			//FString Text(Dest); // FString from tchar array
			//UE_LOG(LogM2U, Log, TEXT("server received %s"), *Text);
			//UE_LOG(LogM2U, Log, TEXT("Server received: %s"), Dest);

			//FString Result = ExecuteCommand(Dest, this);
			//SendResponse(Result);

			// add all the message parts to the Result
			Result += Dest;

			delete Dest;
		}
	}// while
	if (!Result.IsEmpty())
		return true;
	else
		return false;
}

void ImportObjImpl::SendResponse(const FString & Message)
{
	if (Client != NULL && Client->GetConnectionState() == SCS_Connected)
	{
		//const uint8* Data = *Message;
		//const int32 Count = Message.Len();
		int32 DestLen = TStringConvert<TCHAR, ANSICHAR>::ConvertedLength(*Message, Message.Len());
		//UE_LOG(LogM2U, Log, TEXT("DestLen will be %i"), DestLen);
		uint8* Dest = new uint8[DestLen + 1];
		TStringConvert<TCHAR, ANSICHAR>::Convert((ANSICHAR*)Dest, DestLen, *Message, Message.Len());
		Dest[DestLen] = '\0';

		int32 BytesSent = 0;
		if (!Client->Send(Dest, DestLen, BytesSent))
		{
			UE_LOG(LogTemp, Error, TEXT("TCP Server sending answer failed."));
		}
	}
}

void ImportObjImpl::ResetConnection(uint16 Port)
{
	if (Client != NULL)
	{
		Client->Close();
		Client = NULL;
	}
	if (TcpListener != NULL)
	{
		TcpListener->Stop();
		delete TcpListener;
	}
	UE_LOG(LogTemp, Log, TEXT("Hosting on Port %i"), Port);
	TcpListener = new FTcpListener(FIPv4Endpoint(IMPORTOBJ_ADDRESS, Port));
	TcpListener->OnConnectionAccepted().BindRaw(this, &ImportObjImpl::HandleConnectionAccepted);
}

bool ImportObjImpl::HandleConnectionAccepted(FSocket* ClientSocket, const class FIPv4Endpoint& ClientEndpoint)
{
	if (Client == NULL)
	{
		Client = ClientSocket;
		UE_LOG(LogTemp, Log, TEXT("Connected on Port %i"), Client->GetPortNo());
		return true;
	}
	UE_LOG(LogTemp, Log, TEXT("Connection declined"));
	return false;
}

void ImportObjImpl::Tick(float DeltaTime)
{
	// valid and connected?
	if (Client != NULL && Client->GetConnectionState() == SCS_Connected)
	{
		//UE_LOG(LogM2U, Log, TEXT("Tick time was %f"),DeltaTime);
		// get the message, do stuff, and tell the caller what happened ;)
		FString Message;
		if (GetMessage(Message))
		{
			//FString Result = ExecuteCommand(*Message);
			// TODO: add batch-parse-message and execute multiple, newline-divided
			// operations in one go
			//FString Result = OperationManager->Execute(Message);
			//SendResponse(Result);
			UE_LOG(LogTemp, Log, TEXT("Message received: %s"), *Message);

			TArray<FString> sourceFiles;
			sourceFiles.Add(FString(TEXT("D:/Temp/shuttle.obj")));

			//assetTools.ImportAssets(sourceFiles, FString(TEXT("/Game/Test")));
			//assetTools.ImportAssets(FString(TEXT("/Game/Test")));

			UE_LOG(LogTemp, Log, TEXT("IMPORTING"));
			IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			//UE_LOG(LogTemp, Warning, TEXT("importing obj..."));

			//assetTools.ImportAssets(sourceFiles, FString(TEXT("/Game/Test"), nullptr));
			//assetTools.ImportAssets(FString(TEXT("/Game/Test")));


			m2uAssetHelper::ImportAssets2(sourceFiles, FString(TEXT("/Game/Test")), nullptr);
			UE_LOG(LogTemp, Log, TEXT("DONE IMPORTING!!!!!"));
		}
	}
}
 
IMPLEMENT_MODULE(ImportObjImpl, Module)