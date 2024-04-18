#pragma once 

bool menu_opened = true;

void lit_menu() {
    static Vector2 pos = Vector2(700, 700);
    if (KGUI::Window("[Hyper - lite]", &pos, Vector2{ 300.f, 490.f }, menu_opened)) {
        static int tab = 1;
        if (KGUI::ButtonTab("Aimbot", Vector2{ 90, 25 }, tab == 0)) tab = 0;
        KGUI::NextColumn(100);
        if (KGUI::ButtonTab("Esp", Vector2{ 90, 25 }, tab == 1)) tab = 1;
        KGUI::NextColumn(200.0f);
        if (KGUI::ButtonTab("Misc", Vector2{ 90, 25 }, tab == 2)) tab = 2;

        // Draw background rectangle
        KGUI::drawRect(Vector2(pos.x + 10, pos.y + 88), 280, 390, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

        if (tab == 0) {
            // Aimbot settings
            KGUI::PushElementX(5);
            KGUI::PushElementY(55);
            KGUI::FakeElement();
            KGUI::Text(" ");
            KGUI::Checkbox("Aimbot", &config.AIm);
            KGUI::Checkbox("Fov Circle", &config.Fov_cricle);
            KGUI::SliderFloat("Fov Value", &config.Fov_cricle_value, 10, 100);
        }

        else if (tab == 1) {
            // Visuals settings
            KGUI::PushElementX(5);
            KGUI::PushElementY(55);
            KGUI::FakeElement();
            KGUI::Text(" ");
            KGUI::TextLeft("Visuals", Vector2(pos.x + 15, pos.y + 65), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false, false);
            KGUI::Checkbox("Player Box", &config.Box);
            KGUI::Checkbox("Player Skeleton", &config.SkeletonESP);

            KGUI::PushElementX(150);
            KGUI::PushElementY(55);
            KGUI::FakeElement();
            KGUI::Text(" ");
            KGUI::TextLeft("World", Vector2(pos.x + 170, pos.y + 65), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false, false);
           
        }

        else if (tab == 2) {
            // Misc settings
            KGUI::PushElementX(5);
            KGUI::PushElementY(55);
            KGUI::FakeElement();
            KGUI::Text(" ");
            KGUI::Checkbox("Visible Check", &config.visble_check);
        }
    }
}