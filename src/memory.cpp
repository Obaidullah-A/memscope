#include "memory.h"

std::string protect_str(DWORD p) {
    switch (p & 0xFF) {
        case PAGE_NOACCESS:          return "---";
        case PAGE_READONLY:          return "R--";
        case PAGE_READWRITE:         return "RW-";
        case PAGE_WRITECOPY:         return "RC-";
        case PAGE_EXECUTE:           return "--X";
        case PAGE_EXECUTE_READ:      return "R-X";
        case PAGE_EXECUTE_READWRITE: return "RWX";
        case PAGE_EXECUTE_WRITECOPY: return "RCX";
        default:                     return "???";
    }
}

std::string state_str(DWORD s) {
    switch (s) {
        case MEM_COMMIT:  return "COMMIT ";
        case MEM_RESERVE: return "RESERVE";
        case MEM_FREE:    return "FREE   ";
        default:          return "UNKNOWN";
    }
}

std::string type_str(DWORD t) {
    switch (t) {
        case MEM_IMAGE:   return "IMAGE  ";
        case MEM_MAPPED:  return "MAPPED ";
        case MEM_PRIVATE: return "PRIVATE";
        default:          return "       ";
    }
}

std::vector<Region> query_regions(HANDLE process) {
    std::vector<Region> regions;
    MEMORY_BASIC_INFORMATION mbi{};
    uintptr_t addr = 0;

    while (VirtualQueryEx(process, (LPCVOID)addr, &mbi, sizeof(mbi))) {
        regions.push_back({
            (uintptr_t)mbi.BaseAddress,
            mbi.RegionSize,
            mbi.State,
            mbi.Protect,
            mbi.Type
        });
        addr = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
        if (addr < (uintptr_t)mbi.BaseAddress) break; // overflow guard
    }

    return regions;
}
