#pragma once

#include "CoreMinimal.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "CreateGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString ServerName;
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 MaxPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 Index;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, FServerInfoDel);

UCLASS()
class PANICROOM_API UCreateGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	//server
	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer(FServerInfo Info);
	UFUNCTION(BlueprintCallable)
	void FindServer();
	//game exit
	UFUNCTION(BlueprintCallable)
	void GameExit();


	//µ®∏Æ∞‘¿Ã∆Æ
	UPROPERTY(BlueprintAssignable)
	FServerDel FServerListDel;

protected:
	virtual void Init() override;
	//server
	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	TArray<FOnlineSessionSearchResult> SearchResults;

};
