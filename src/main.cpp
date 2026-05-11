#include <windows.h>
#include <cstdio>
#include "memory.h"

static HANDLE open_process(DWORD pid) {
    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!h) {
        fprintf(stderr, "error: cannot open process %lu (code %lu)\n", pid, GetLastError());
    }
    return h;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: memscope <pid>\n");
        return 1;
    }

    DWORD pid = (DWORD)strtoul(argv[1], nullptr, 10);
    HANDLE process = open_process(pid);
    if (!process) return 1;

    auto regions = query_regions(process);
    CloseHandle(process);

    printf("%-18s %-12s %-9s %-9s %s\n", "BASE", "SIZE", "STATE", "TYPE", "PROTECT");
    printf("%-18s %-12s %-9s %-9s %s\n",
        "------------------", "------------", "---------", "---------", "-------");

    for (auto& r : regions) {
        printf("0x%016llX %-12llu %-9s %-9s %s\n",
            (unsigned long long)r.base,
            (unsigned long long)r.size,
            state_str(r.state).c_str(),
            type_str(r.type).c_str(),
            protect_str(r.protect).c_str()
        );
    }

    printf("\n%zu regions\n", regions.size());
    return 0;
}
