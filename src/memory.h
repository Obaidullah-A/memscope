#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct Region {
    uintptr_t base;
    size_t    size;
    DWORD     state;
    DWORD     protect;
    DWORD     type;
};

std::string protect_str(DWORD protect);
std::string state_str(DWORD state);
std::string type_str(DWORD type);
std::vector<Region> query_regions(HANDLE process);
