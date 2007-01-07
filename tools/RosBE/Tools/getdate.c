#include <windows.h>
#include <stdio.h>

int main(void)
{
    SYSTEMTIME LocalSystemTime;
    char CurrentDate[20];

    GetSystemTime(&LocalSystemTime);
    GetDateFormat(LOCALE_USER_DEFAULT,
                  0,
                  &LocalSystemTime,
                  "MM/dd/yyyy",
                  CurrentDate,
                  20);

    printf("%s", CurrentDate);

    return 0;
}
