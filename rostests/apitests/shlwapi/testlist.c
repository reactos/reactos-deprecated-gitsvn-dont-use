#define STANDALONE
#include <apitest.h>

extern void func_PathFindOnPath(void);
extern void func_isuncpath(void);
extern void func_isuncpathserver(void);
extern void func_isuncpathservershare(void);
extern void func_PathUnExpandEnvStrings(void);
extern void func_StrFormatByteSizeW(void);

const struct test winetest_testlist[] =
{
    { "PathFindOnPath", func_PathFindOnPath },
    { "PathIsUNC", func_isuncpath },
    { "PathIsUNCServer", func_isuncpathserver },
    { "PathIsUNCServerShare", func_isuncpathservershare },
    { "PathUnExpandEnvStrings", func_PathUnExpandEnvStrings },
    { "StrFormatByteSizeW", func_StrFormatByteSizeW },
    { 0, 0 }
};
