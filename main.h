bool __forceinline Main() {
    // Get base address
    CachedData::Base = *(uint64_t*)(__readgsqword(0x60) + 0x10);

    // Scan for UWorld
    auto scanned_uworld = ResolveRelativeAddress(PScan("48 8B 05 ? ? ? ? 48 3B C3 49 0F 44 C7"), 7);

    // Get function pointers
    GetNameByIndex = decltype(GetNameByIndex)(PScan("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B 19 48 8B F2 0F B7 FB"));
    LineSightOfTo = decltype(LineSightOfTo)(PScan("48 8B C4 48 89 58 20 55 56 57 41 54 41 56 48 8D 68 B8 48 81 EC ? ? ? ? ? ? ? ?"));
    GetBoneMatrix = PScan("E8 ? ? ? ? 0F 10 40 68");
    GetBoneMatrix = RVA(GetBoneMatrix, 5);

    // Set GWorld
    Gworld = (void*)scanned_uworld;
    auto GWorld = *(uintptr_t*)(scanned_uworld);
    auto World = (UWorld*)GWorld;

    // Get game instance and local player
    auto Game = World->GameInstance();
    auto LocalPlayers = Game->LocalPlayers();
    auto LocalPlayer = LocalPlayers->LocalPlayer();

    // Get controller and viewport
    auto Controller = LocalPlayer->PlayerController();
    auto Viewport = LocalPlayer->ViewPortClient();

    // Setup keys
    UKismetStringLibrary::SetupKeys();

    // Hook VMT
    auto index = 109;
    vmt((void*)(std::uintptr_t)Viewport, Hook, index, (void**)&Orig);

    return true;
}