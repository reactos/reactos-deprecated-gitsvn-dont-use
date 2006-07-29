#include <windows.h>

int main() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  printf("%ld\n", sysinfo.dwNumberOfProcessors);
}

