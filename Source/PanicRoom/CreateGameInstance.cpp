
#include "CreateGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

void UCreateGameInstance::Init()
{
	//플레이 시작할때 
	Super::Init();
	//server
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCreateGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCreateGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCreateGameInstance::OnJoinSessionComplete);
		}
	}
}

void UCreateGameInstance::CreateServer()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = false;///수정함
	SessionSettings.bUseLobbiesIfAvailable = true;//수정함
	SessionSettings.bIsDedicated = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSettings.bIsLANMatch = false;
	else
		SessionSettings.bIsLANMatch = true;

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 8;
	SessionSettings.Set(FName("SESSION_NAME"),
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState->GetPlayerName(),
		EOnlineDataAdvertisementType::ViaOnlineService);

	//SessionSettings.Settings.Add(FName("SESSION_NAME"),
	//	UGameplayStatics::GetPlayerController(GetWorld(),0)->PlayerState->GetPlayerName());

	SessionInterface->CreateSession(0,
		FName(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState->GetPlayerName()), SessionSettings);
}


void UCreateGameInstance::JoinServer(FServerInfo Info)
{
	SessionInterface->JoinSession(0, FName(Info.ServerName), SearchResults[Info.Index]);
}

void UCreateGameInstance::FindServer()
{
	//서버 결과 초기화
	SearchResults.Empty();

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSearch->bIsLanQuery = false;
	else
		SessionSearch->bIsLanQuery = true;//isLan
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}


void UCreateGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	UE_LOG(LogTemp, Display, L"OnCreateSessionComplete, Succeeded %d", Succeeded);
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Level/TestMap?listen");
	}
}

void UCreateGameInstance::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Display, L"OnFindSessionComplete, Succeeded %d", Succeeded);
	if (Succeeded)
	{
		SearchResults = SessionSearch->SearchResults;
		int32 index = 0;
		for (FOnlineSessionSearchResult Result : SearchResults)
		{
			if (!Result.IsValid())
				continue;
			FServerInfo Info;
			Info.ServerName = Result.Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString();//서버 이름
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections; //최대인원
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;//현재인원
			Info.Index = index;
			//delegate 호출
			if (FServerListDel.IsBound())
				FServerListDel.Broadcast(Info);
			index++;
		}
	}
}

void UCreateGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, L"OnJoinSessionComplete , sessionName %s", *SessionName.ToString());
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
			PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}
}
