/*
 * PROJECT:     ReactOS System Regression Testing Utility
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Integrated raddr2line tool for getting source references from addresses
 * COPYRIGHT:   Copyright 2008-2009 Christoph von Wittich <christoph_vw@reactos.org>
 *              Copyright 2009 Colin Finck <colin@reactos.org>
 */

#include "sysreg.h"

static void RecurseModuleDirectory(const char* Directory, ModuleListEntry** LastElement)
{
    char* EntryPath;
    char* Period;
    DIR* dir;
    struct dirent* dp;
    struct stat statbuf;

    dir = opendir(Directory);
    if(!dir)
        return;

    while(dp = readdir(dir))
    {
        if(*dp->d_name == '.')
            continue;

        EntryPath = (char*)malloc(strlen(Directory) + strlen(dp->d_name) + 2);
        strcpy(EntryPath, Directory);
        strcat(EntryPath, "/");
        strcat(EntryPath, dp->d_name);

        if(stat(EntryPath, &statbuf) == -1)
        {
            free(EntryPath);
            continue;
        }

        if(statbuf.st_mode & S_IFDIR)
        {
            RecurseModuleDirectory(EntryPath, LastElement);
            free(EntryPath);
        }
        else
        {
            Period = strchr(dp->d_name, '.');

            /* A file needs to have one of the following extensions to be a valid module */
            if(!Period || (strcasecmp(Period, ".exe") && strcasecmp(Period, ".dll") && strcasecmp(Period, ".sys")))
            {
                free(EntryPath);
                continue;
            }

            (*LastElement)->Next = (ModuleListEntry*)malloc(sizeof(ModuleListEntry));
            *LastElement = (*LastElement)->Next;
            (*LastElement)->Next = NULL;

            (*LastElement)->Module = (char*)malloc(strlen(dp->d_name) + 1);
            strcpy((*LastElement)->Module, dp->d_name);
            (*LastElement)->Path = EntryPath;
        }
    }

    closedir(dir);
}

void InitializeModuleList()
{
    ModuleListEntry* LastElement;

    ModuleList = (ModuleListEntry*)malloc(sizeof(ModuleListEntry));
    ModuleList->Next = NULL;
    LastElement = ModuleList;

    RecurseModuleDirectory(OutputPath, &LastElement);
}

void CleanModuleList()
{
    ModuleListEntry* CurrentElement;
    ModuleListEntry* PreviousElement;

    CurrentElement = ModuleList->Next;

    while(CurrentElement)
    {
        free(CurrentElement->Module);
        free(CurrentElement->Path);

        PreviousElement = CurrentElement;
        CurrentElement = CurrentElement->Next;
        free(PreviousElement);
    }

    free(ModuleList);
}

static ModuleListEntry* FindModule(const char* Module)
{
    ModuleListEntry* CurrentElement;

    CurrentElement = ModuleList->Next;

    while(CurrentElement)
    {
        if(!strcmp(CurrentElement->Module, Module))
            return CurrentElement;

        CurrentElement = CurrentElement->Next;
    }

    return NULL;
}

bool ResolveAddressFromFile(char* Buffer, size_t BufferSize, const char* Data)
{
    bool ReturnValue = false;
    char* Address = NULL;
    char* AddressStart;
    char Command[256];
    char* Module = NULL;
    char* pBuffer;
    FILE* Process;
    ModuleListEntry* ModuleEntry;
    size_t AddressLength;

    /* A resolvable backtrace line has to look like this:
       <abcdefg.dll:123a>
    */
    if(*Data != '<')
        return false;

    AddressStart = strchr(Data, ':');

    if(!AddressStart)
        return false;

    ++AddressStart;
    AddressLength = strspn(AddressStart, "1234567890abcdefABCDEF");

    if(!AddressLength || AddressStart[AddressLength] != '>')
        return false;

    /* Ok, this looks like a backtrace line we can resolve */
    Module = (char*)malloc(AddressStart - Data - 1);
    strncpy(Module, Data + 1, AddressStart - Data - 2);
    Module[AddressStart - Data - 2] = 0;

    Address = (char*)malloc(AddressLength + 1);
    strncpy(Address, AddressStart, AddressLength);
    Address[AddressLength] = 0;

    /* Try to find the path to this module */
    if(ModuleEntry = FindModule(Module))
    {
        /* Run raddr2line */
        sprintf(Command, "%s/tools/rsym/raddr2line %s %s 2>/dev/null", OutputPath, ModuleEntry->Path, Address);
        Process = popen(Command, "r");

        if(!feof(Process))
        {
            pBuffer = &Buffer[AddressStart - Data + AddressLength];
            strncpy(Buffer, Data, pBuffer - Buffer);

            *pBuffer++ = ' ';
            *pBuffer++ = '(';

            if(fgets(pBuffer, BufferSize - (pBuffer - Buffer), Process))
            {
                pBuffer += strlen(pBuffer) - 1;

                *pBuffer++ = ')';
                *pBuffer++ = '>';
                *pBuffer++ = '\n';
                *pBuffer = 0;

                ReturnValue = true;
            }
        }

        pclose(Process);
    }

    free(Module);
    free(Address);

    return ReturnValue;
}
