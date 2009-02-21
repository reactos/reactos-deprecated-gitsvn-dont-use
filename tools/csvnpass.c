/* csvnpass - Convert SVN passwd files to Apache htpasswd format
   by Colin Finck (2009-02-21)
   Licensed under GNU General Public License, version 2 or (at your option) any later version

   loosely based on the original htpasswd.c by Rob McCool
*/

#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

const char Base64Characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./";

void AddEntry(char* User, char* Password, FILE* fp)
{
    char* CryptedPassword;
    char Salt[3];

    srand((unsigned int)time(NULL));
    Salt[0] = Base64Characters[rand() % 64];
    Salt[1] = Base64Characters[rand() % 64];
    Salt[2] = 0;
    CryptedPassword = crypt(Password, Salt);

    fprintf(fp, "%s:%s\n", User, CryptedPassword);
}

void ProcessInput(char* pInput, FILE* fp)
{
    char InUsers = 0;
    char* pLineBreak;
    char* User;
    size_t Length;

    for(;;)
    {
        /* Find the line break character (if any) and set it to 0, so we can treat pInput as a whole line */
        pLineBreak = strchr(pInput, '\n');

        if(!pLineBreak)
            break;

        *pLineBreak = 0;

        if(InUsers)
        {
            while(isspace(*pInput))
                ++pInput;

            /* Break if we're at the beginning of a new section */
            if(*pInput == '[')
                break;

            /* The line may not begin with a comment character either */
            if(*pInput != '#')
            {
                /* The username ends at the equal sign or the first space */
                Length = strcspn(pInput, "= ");

                /* Store it */
                User = malloc(Length + 1);
                memcpy(User, pInput, Length);
                User[Length] = 0;

                /* Move to the password */
                pInput += Length;
                pInput += strspn(pInput, "= ");

                /* Clear trailing password spaces (if any) */
                Length = strcspn(pInput, " ");
                pInput[Length] = 0;

                AddEntry(User, pInput, fp);

                free(User);
            }
        }
        else if(!strcmp(pInput, "[users]"))
        {
            InUsers = 1;
        }

        /* Move to the next line */
        pInput = pLineBreak + 1;
    }
}

void PrintUsage()
{
    printf("csvnpass - Convert SVN passwd files to Apache htpasswd format\n");
    printf("by Colin Finck (2009-02-21)\n");
    printf("Usage: csvnpass input_file output_file\n");
}

int main(int argc, char *argv[])
{
    char InUsers = 0;
    char* InputFile = NULL;
    char* pEqual;
    char* pInput;
    char* pLineBreak;
    char* User = NULL;
    int ReturnValue = 1;
    FILE* fp = NULL;
    struct stat StatBuffer;

    if(argc != 3)
    {
        PrintUsage();
        goto Cleanup;
    }

    /* Read the entire input file */
    fp = fopen(argv[1], "r");

    if(!fp)
    {
        fprintf(stderr, "Could not open input file!\n");
        goto Cleanup;
    }

    if(fstat(fileno(fp), &StatBuffer))
    {
        fprintf(stderr, "fstat failed for the input file!\n");
        goto Cleanup;
    }

    InputFile = (char*)malloc(StatBuffer.st_size + 1);
    fread(InputFile, StatBuffer.st_size, 1, fp);
    InputFile[StatBuffer.st_size] = 0;
    fclose(fp);

    /* Open the output file */
    fp = fopen(argv[2], "w");

    if(!fp)
    {
        fprintf(stderr, "Could not open output file!\n");
        goto Cleanup;
    }

    ProcessInput(InputFile, fp);
    ReturnValue = 0;

Cleanup:
    if(fp)
        fclose(fp);

    if(InputFile)
        free(InputFile);

    return ReturnValue;
}
