#define _CRT_SECURE_NO_WARNINGS

#include "UnrealSDKClass.h"
#include "Afort.hpp"
#include "Actor.hpp"
#include "main.h"

bool DllMain(void* hModule, int ul_reason_for_call, void* lpReserved) {
	if (ul_reason_for_call == 0x1) {
		Main();
	}

	return true;
}