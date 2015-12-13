#pragma once
 
#include "ModuleManager.h"

#define IMPORTOBJ_ADDRESS FIPv4Address(0, 0, 0, 0)
#define IMPORTOBJ_PORT 3939

//DEFINE_LOG_CATEGORY(MyPlugin);
//DECLARE_LOG_CATEGORY_EXTERN(MyPlugin, Log, All);

class Fm2uTickObject;
 
class ImportObjImpl : public IModuleInterface
{
public:
	ImportObjImpl();

	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	/* TickObject Delegate */
	void Tick(float DeltaTime);

	/* TcpListener Delegate */
	bool HandleConnectionAccepted(FSocket* ClientSocket, const class FIPv4Endpoint& ClientEndpoint);

	/* TCP messaging functions */
	bool GetMessage(FString& Result);
	void SendResponse(const FString& Message);
	void ResetConnection(uint16 Port);

protected:
	FSocket* Client;
	class FTcpListener* TcpListener;
	Fm2uTickObject* TickObject;
};