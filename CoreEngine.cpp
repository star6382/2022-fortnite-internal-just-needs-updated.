#include "CoreEngine.h"
#include "PatternScanner.h"
#include <iostream>

// Global base address
uint64_t GBase = *(uint64_t*)(__readgsqword(0x60) + 0x10);

// Get object name
std::string UObject::GetName()
{
    std::string name(Name.GetName());
    if (Name.Number > 0)
    {
        name += '_' + std::to_string(Name.Number);
    }
    auto pos = name.rfind('/');
    if (pos == std::string::npos)
    {
        return name;
    }
    return name.substr(pos + 1);
}

// Get full object name (including outer objects)
std::string UObject::GetFullName()
{
    std::string name;
    if (Class != nullptr)
    {
        std::string temp;
        for (auto out = Outer; out; out = out->Outer)
        {
            temp = out->GetName() + "." + temp;
        }
        name = Class->GetName();
        name += " ";
        name += temp;
        name += GetName();
    }
    return name;
}

// Process an event for the object
void UObject::ProcessEvent(uintptr_t address, void* fnobject, void* parms)
{
    if (!address) return;
    auto index = *reinterpret_cast<void***>(address);
    if (!index) return;
    auto fProcessEvent = static_cast<void(*)(void* address, void* fnobject, void* parms)>(index[0x4C]);
    if (!fProcessEvent) return;
    fProcessEvent((void*)address, (void*)fnobject, (void*)parms);
}

// Find an object by its full name
uintptr_t UObject::FindObject(const std::string& name) {
    static FUObjectArray* GObjects = NULL;
    if (!GObjects) {
        GObjects = (FUObjectArray*)ResolveRelativeAddress(PScan(("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1")), 7);
    }

    auto GlobalObjects = GObjects->ObjObjects;
    for (int i = 0; i < GlobalObjects.Num(); ++i)
    {
        auto object = GlobalObjects.GetByIndex(i);
        if (object)
        {
            if (object->GetFullName() == name)
            {
                std::cout << name.c_str() << " : 0x" << std::hex << object - GBase << std::endl;
                return reinterpret_cast<uintptr_t>(object);
            }
        }
    }

    return 0;
}