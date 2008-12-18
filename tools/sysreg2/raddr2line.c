#include "sysreg.h"

bool GetPackagePath(char* Buffer, int BuffSize, char* Module)
{
    char* ptr;
    FILE* fp;
    int i;

    fp = fopen("boot/bootdata/packages/reactos.dff", "r");
    if (fp)
    {
        while(fgets(Buffer, BuffSize,fp))
        {
            if (strstr(Buffer, Module))
            {
                ptr = strchr(Buffer, ' ');
                if (ptr)
                    *ptr = '\0';

                for (i = 0; i < strlen(Buffer); i++)
                {
                    if (Buffer[i] == '\\')
                        Buffer[i] = '/';
                }
                fclose(fp);
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

bool ResolveAddressFromFile(char* Buffer, int BuffSize, char* Data)
{
    int Resolved = 0;
    char* sep;
    char* sep2;
    char Module[32];
    char Addr[20];
    char Command[256];
    char PkgData[80];
    char emptystr[] = ""; 

    if ((strstr(Data, ":>")) && (!strchr(Data, ')')))
    {
        sep = strchr(Data, ':');
        if (sep)
        {
            strncpy(Module, Data + 1, (int) (sep - Data) - 1);
            Module[(int)(sep-Data)-2] = '\0';
            sep2 = strchr(Data, '>');
            strncpy(Addr, Data + (sep - Data + 1), (int) (sep2 - sep) -1);
            Addr[(int)(sep2-sep)-1] = '\0'; 
            Resolved = 1;
        }
    }

    sep = strchr(Data, '+');
    if ((sep) && strstr(Data, ":\\"))
    {
        sep2 = strchr(Data + (sep - Data), ' ');
        strncpy(Addr, Data + (sep - Data + 1), (int) (sep2 - sep) - 1);
        Addr[(int)(sep2-sep) - 1] = '\0';
        sep = strrchr(Data, '\\');
        strcpy(Module, sep + 1);
        Module[strlen(Module) - 2] = '\0';
        Resolved = 2;
    }

    if (Resolved > 0)
    {
        if (GetPackagePath(PkgData, sizeof(PkgData), Module))
        {
            const char* outdir = getenv("ROS_OUTPUT");
            char* ptr;
            if (!outdir)
                outdir = emptystr;
            sprintf(Command, "%s/tools/raddr2line %s/%s %s 2>&1", 
                    outdir, outdir, PkgData, Addr);
            FILE* p = popen(Command, "r");
            char buf[100] = {'\0'};
            while(feof(p)==0)
            {
                fgets(buf,100,p);
                if (strstr(buf, "error"))
                    strcpy(buf, "(no debug info)");
                ptr = strchr(buf, '\n');
                if (ptr)
                    *ptr = '\0'; 
                ptr = strchr(Data, '>');
                if (ptr)
                    *ptr = '\0';
                if (Resolved == 2)
                {
                    ptr = strchr(Data, '\n');
                    if (ptr)
                        *ptr = '\0';
                }
                strcpy(Buffer, Data);
                strcat(Buffer, " ");
                if (Resolved == 2)
                    strcat(Buffer, "<");
                strcat(Buffer, buf);
                strcat(Buffer, ">\n");
                return true;     
            }
        }
    }
    return false;
}
