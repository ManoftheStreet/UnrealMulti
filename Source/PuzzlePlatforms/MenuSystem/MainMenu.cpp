// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/Blueprint/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(HostIPButton != nullptr)) return false;
	HostIPButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancleHostMenuButton != nullptr)) return false;
	CancleHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::CancleJoinMenu);

	if (!ensure(CancleJoinMenuButton != nullptr)) return false;
	CancleJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::CancleJoinMenu);

	if (!ensure(JoinIPButton != nullptr)) return false;
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	
	return true;
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		UE_LOG(LogTemp, Warning, TEXT("Current Players: %d"), ServerData.CurrentPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		i++;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row)
		{
			Row->Selected = (SelectedIndex && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selecte index %d"),SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selecte index not set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::CancleJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
