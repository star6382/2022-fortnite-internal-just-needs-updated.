#pragma once

#include <windows.h>
#include <vector>
#include <math.h>
#include <string>

// Forward declaration
std::string GetById(int id);

// Represents a name in the Unreal Engine
struct FName {
	union {
		struct {
			int32_t ComparisonIndex;
			int32_t Number;
		};
		uint64_t CompositeComparisonValue;
	};

	FName() : ComparisonIndex(0), Number(0) {}
	FName(int32_t i) : ComparisonIndex(i), Number(0) {}

	std::string GetName() const {
		return GetById(ComparisonIndex);
	}

	bool operator==(const FName& other) const {
		return ComparisonIndex == other.ComparisonIndex;
	}
};

// Base class for Unreal Engine objects
class UObject {
public:
	void** Vtable;
	int32_t ObjectFlags;
	int32_t InternalIndex;
	class UClass* Class;
	FName Name;
	class UObject* Outer;

	std::string GetName();
	std::string GetFullName();

	static void ProcessEvent(uintptr_t address, void* fnobject, void* parms);
	static uintptr_t FindObject(const std::string& name);
	static UClass* FindClass(const std::string& name) {
		return reinterpret_cast<UClass*>(FindObject(name));
	}
};

// Represents a field in Unreal Engine
class UField : public UObject {
public:
	class UField* Next;
};

// Represents a struct in Unreal Engine
class UStruct : public UField {
public:
	char pad_0030[0x10];
	class UStruct* SuperField;
	class UField* Children;
	void* ChildProperties;
	int32_t PropertySize;
	int32_t MinAlignment;
	char pad_0060[0x50];
};

// Represents a class in Unreal Engine
class UClass : public UStruct {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

// Represents an item in the Unreal Engine object array
class FUObjectItem {
public:
	UObject* Object;
	int32_t Flags;
	int32_t ClusterIndex;
	int32_t SerialNumber;
	char padding[0x4];

	enum class ObjectFlags : int32_t {
		None = 0,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		NoStrongReference = 1 << 31
	};

	bool IsUnreachable() const {
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::Unreachable));
	}

	bool IsPendingKill() const {
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::PendingKill));
	}
};

// Represents a chunk of FUObjectItem pointers
class PreFUObjectItem {
public:
    FUObjectItem* FUObject[10];
};

// Represents the Unreal Engine object array
class TUObjectArray {
public:
    void NumChunks(int* start, int* end) const {
        int cStart = 0, cEnd = 0;
        if (!cEnd) {
            while (Objects->FUObject[cStart] == 0) {
                cStart++;
            }
            cEnd = cStart;
            while (Objects->FUObject[cEnd] != 0) {
                cEnd++;
            }
        }
        *start = cStart;
        *end = cEnd;
    }

    UObject* GetByIndex(int32_t index) const {
        int cStart = 0, cEnd = 0;
        int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
        FUObjectItem* Object = nullptr;
        NumChunks(&cStart, &cEnd);
        chunkIndex = index / chunkSize;
        if (chunkSize * chunkIndex != 0 && chunkSize * chunkIndex == index) {
            chunkIndex--;
        }
        chunkPos = cStart + chunkIndex;
        if (chunkPos < cEnd) {
            Object = Objects->FUObject[chunkPos] + (index - chunkSize * chunkIndex);
            return Object ? Object->Object : nullptr;
        }

        return nullptr;
    }

    FUObjectItem* GetItemByIndex(int32_t index) const {
        int cStart = 0, cEnd = 0;
        int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
		FUObjectItem* Object = nullptr;
        NumChunks(&cStart, &cEnd);
        chunkIndex = index / chunkSize;
        if (chunkSize * chunkIndex != 0 && chunkSize * chunkIndex == index) {
            chunkIndex--;
        }
        chunkPos = cStart + chunkIndex;
        if (chunkPos < cEnd) {
            return Objects->FUObject[chunkPos] + (index - chunkSize * chunkIndex);
        }
        return nullptr;
    }

    int32_t Num() const {
        return NumElements;
    }

private:
    PreFUObjectItem* Objects;
    char padding[8];
    int32_t MaxElements;
    int32_t NumElements;
};

// Represents the Unreal Engine global object array
class FUObjectArray {
public:
    TUObjectArray ObjObjects;
};