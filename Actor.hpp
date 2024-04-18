#pragma once 
#include "menu_render.hh"
#include "Hyper_menu.h"

void Hook(UGameViewportClient* Viewport, UCanvas* Canvas) {
    UWorld* GWorld = reinterpret_cast<UWorld*>(*(uintptr_t*)(Gworld));
    UGameInstance* GameInstance = GWorld->GameInstance();
    ULocalPlayer* LocalPlayer = GameInstance->LocalPlayers()->LocalPlayer();
    APlayerController* Controller = LocalPlayer->PlayerController();
    UGameViewportClient* ViewportClient = LocalPlayer->ViewPortClient();
    AFortPawn* AcknowledgedPawn = Controller->AcknowledgedPawn();

    // Initialize FortKismetLibrary
    static UFortKismetLibrary* FortKismetLibrary = UFortKismetLibrary::StaticClass();

    // Draw FOV circle if enabled
    if (config.Fov_cricle) {
        Vector2 Center = Canvas->SZE() / 2;
        float Radius = (config.Fov_cricle_value * Canvas->SZE().x / config.angle) / 2;
        KGUI::DrawCircle(Center, Radius, { 1, 1, 1, 1 }, 128);
    }

    // Toggle menu with Insert key
    if (Controller->WasInputKeyJustPressed(keys::Insert))
        menu_opened = !menu_opened;

    if (menu_opened) {
        // Set up canvas for menu
        KGUI::SetupCanvas(Canvas);
        GUI::Input::Handle();
        lit_menu();
    }

    // Get all FortPawn players
    TArray<AFortPawn*> Players = FortKismetLibrary->GetFortPawn((UObject*)GWorld);
   	for (int i = 0; i < Players.Num(); i++) {
        AFortPawn* Player = Players[i];

        if (!GetBoneID(Controller, Player) || Player == AcknowledgedPawn || !Player->RootComponent() || !Player->Mesh())
            continue;

        // Determine ESP color based on visibility
        FLinearColor esp_color = config.visble_check ? (Controller->LineOfSightTo(Player) ? FLinearColor(1.f, 1.f, 1.f, 1.f) : FLinearColor(1.f, 0.f, 0.f, 1.f)) : FLinearColor(1.f, 1.f, 1.f, 1.f);

        // Draw skeleton lines if enabled
        if (config.SkeletonESP) {
            // Define skeleton lines
            std::vector<std::pair<Vector2, Vector2>> skeletonLines = {
                { s_neck, s_chest },
                { s_chest, s_pelvis },
                { s_chest, s_rshoulder },
                { s_rshoulder, s_relbow },
                { s_relbow, s_rhand },
                { s_rthigh, s_rknee }
            };

            // Draw each skeleton line
            for (const auto& line : skeletonLines) {
                Canvas->DrawLine(line.first, line.second, 1.f, esp_color);
            }
        }

        if (config.Box) {
            // Calculate box dimensions
            float BoxHeight = static_cast<float>(s_bottom_2.y - s_head_2.y);
            float BoxWidth = BoxHeight * 0.50f;

            // Define box corners
            Vector2 topLeft(s_head_2.x - BoxWidth / 2, s_head_2.y);
            Vector2 topRight(s_head_2.x + BoxWidth / 2, s_head_2.y);
            Vector2 bottomLeft(s_head_2.x - BoxWidth / 2, s_bottom_2.y);
            Vector2 bottomRight(s_head_2.x + BoxWidth / 2, s_bottom_2.y);

            // Draw box lines
            Canvas->DrawLine(topLeft, topRight, 1.f, esp_color);
            Canvas->DrawLine(topRight, bottomRight, 1.f, esp_color);
            Canvas->DrawLine(bottomRight, bottomLeft, 1.f, esp_color);
            Canvas->DrawLine(bottomLeft, topLeft, 1.f, esp_color);
        }
    }
}