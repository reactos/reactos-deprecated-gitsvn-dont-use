/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/scut.c
 * PURPOSE:     Manages named shortcuts to ReactOS source directories.
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *                             Daniel Reimer <reimer.daniel@freenet.de>
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _SHORTCUT
{
    char* name;
    char* path;
    struct _SHORTCUT *next;
} SHORTCUT, *PSHORTCUT;

#define LINE_MAX 1024
char* programname;
char shortcutfile[260];

PSHORTCUT addshortcut(PSHORTCUT ptr, char* name, char* path);
void checkfile(void);
int checkname(PSHORTCUT head, char* name);
int checkpath(char* path); // Alters path by fully expanding it.
void defaultshortcut(char* name);
PSHORTCUT deleteshortcut(PSHORTCUT ptr, PSHORTCUT *head);
void editshortcut(PSHORTCUT ptr, char* name, char* path);
void freeshortcuts(PSHORTCUT ptr);
PSHORTCUT previousshortcut(PSHORTCUT current, PSHORTCUT head);
PSHORTCUT readshortcuts(void);
int writeshortcuts(PSHORTCUT head);

int main(int argc, char* argv[])
{
    PSHORTCUT shortcuts = NULL, current = NULL;
    char name[260], path[260];
    int removed = 0;
    programname = argv[0];

    strcpy(shortcutfile, getenv("APPDATA"));
    strcat(shortcutfile, "\\RosBE\\srclist.txt");
    checkfile();

    if (argc > 4)
    {
        fprintf(stderr, "%s: Error too many parameters specified.\n", programname);
        return -1;
    }
    if (argc == 1)
    {
        defaultshortcut(NULL);
    }
    else if ((!strncmp(argv[1], "/?", 2)) ||
             (!_strnicmp(argv[1], "-h", 2)) ||
             (!_strnicmp(argv[1], "--help", 6)))
    {
        printf("Usage: %s [OPTIONS] [SHORTCUT] [PATH]\n", programname);
        printf("Manages named shortcuts to ReactOS source directories. scut started\n");
        printf("with the name of a shortcut sets this shortcut active.\n\n");
        printf("The following details the options:\n\n");
        printf("   list    - Lists all shortcuts and their paths.\n");
        printf("   add     - Adds a shortcut.\n");
        printf("             (Optional: SHORTCUT PATH as the second and third parameters)\n");
        printf("   rem     - Removes a shortcut.\n");
        printf("             (Optional: SHORTCUT as the second parameter)\n");
        printf("   edit    - Edits a shortcut.\n");
        printf("             (Optional: SHORTCUT PATH as the second and third parameters)\n");
        printf("   def     - Sets the default shortcut which is enabled on\n");
        printf("             every start of RosBE. 'Default' is the source\n");
        printf("             directory as you know it from previous versions.\n\n");
        return 0;
    }
    else if (!_strnicmp(argv[1], "list", 4))
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        printf("All available shortcuts:\n\n");
        while(current)
        {
            printf("Shortcut Name: %s\n", current->name);
            printf("      -> Path: %s\n", current->path);
            current = current->next;
        }
        freeshortcuts(shortcuts);
    }
    else if (!_strnicmp(argv[1], "add", 3))
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        if (argc >= 3)
        {
            strcpy(name, argv[2]);
        }
        else
        {
            do
            {
                printf("Please enter a name for the shortcut: ");
                fflush(stdin);
                fgets(name, 260, stdin);
            } while(strlen(name) == 1);
            strcpy(name, strtok(name, "\n"));
        }
        if (!checkname(shortcuts, name))
        {
            fprintf(stderr, "%s: Shortcut '%s' already exists.\n", programname, name);
        }
        else
        {
            if (argc == 4)
            {
                strcpy(path, argv[3]);
            }
            else
            {
                do
                {
                    printf("Please enter the path for the shortcut: ");
                    fflush(stdin);
                    fgets(path, 260, stdin);
                } while(strlen(path) == 1);
                strcpy(path, strtok(path, "\n"));
            }
            if (!checkpath(path))
            {
                fprintf(stderr, "%s: Directory '%s' doesn't exist.\n", programname, path);
            }
            else
            {
                shortcuts = addshortcut(shortcuts, name, path);
                writeshortcuts(shortcuts);
            }
        }
        freeshortcuts(shortcuts);
    }
    else if (!_strnicmp(argv[1], "rem", 3))
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        if (argc >= 3)
        {
            strcpy(name, argv[2]);
        }
        else
        {
            do
            {
                printf("Please enter the name of the shortcut to remove: ");
                fflush(stdin);
                fgets(name, 260, stdin);
            } while(strlen(name) == 1);
            strcpy(name, strtok(name, "\n"));
        }
        if (!_stricmp(name, "Default"))
        {
            fprintf(stderr, "%s: Unable to remove default shortcut.\n", programname);
        }
        else
        {
            while(current)
            {
                if (!_stricmp(current->name, name))
                {
                    current = deleteshortcut(current, &shortcuts);
                    removed = 1;
                    break;
                }
                current = current->next;
            }
            if (removed)
            {
                writeshortcuts(shortcuts);
                printf("Removed shortcut: %s\n", name);
            }
            else
            {
                printf("Unable to find shortcut: %s\n", name);
            }
        }
        freeshortcuts(shortcuts);
    }
    else if (!_strnicmp(argv[1], "edit", 4))
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        if (argc >= 3)
        {
            strcpy(name, argv[2]);
        }
        else
        {
            do
            {
                printf("Please enter the name of the shortcut to edit: ");
                fflush(stdin);
                fgets(name, 260, stdin);
            } while(strlen(name) == 1);
            strcpy(name, strtok(name, "\n"));
        }
        if (!_stricmp(name, "Default") || checkname(shortcuts, name))
        {
            fprintf(stderr, "%s: Shortcut '%s' doesn't exist.\n", programname, name);
        }
        else
        {
            while(current)
            {
                if (!_stricmp(current->name, name))
                {
                    if (argc == 4)
                    {
                        strcpy(path, argv[3]);
                    }
                    else
                    {
                        do
                        {
                            printf("Please enter a new path for the shortcut: ");
                            fflush(stdin);
                            fgets(path, 260, stdin);
                        } while(strlen(path) == 1);
                        strcpy(path, strtok(path, "\n"));
                    }
                    if (!checkpath(path))
                    {
                        fprintf(stderr, "%s: Directory '%s' doesn't exist.\n", programname, path);
                        break;
                    }
                    else
                    {
                        editshortcut(current, name, path);
                        writeshortcuts(shortcuts);
                        printf("Edited shortcut: %s\n", name);
                        break;
                    }
                }
                current = current->next;
            }
        }
        freeshortcuts(shortcuts);
    }
    else if (!_strnicmp(argv[1], "def", 3))
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        if (argc >= 3)
        {
            strcpy(name, argv[2]);
        }
        else
        {
            do
            {
                printf("Please enter the the name of the shortcut to set as default: ");
                fflush(stdin);
                fgets(name, 260, stdin);
            } while(strlen(name) == 1);
            strcpy(name, strtok(name, "\n"));
        }
        if (!_stricmp(name, "Default") || checkname(shortcuts, name))
        {
            fprintf(stderr, "%s: Shortcut '%s' doesn't exist.\n", programname, name);
            freeshortcuts(shortcuts);
        }
        else
        {
            freeshortcuts(shortcuts);
            defaultshortcut(name);
        }
    }
    else
    {
        shortcuts = readshortcuts();
        current = shortcuts;

        strcpy(name, argv[1]);
        if (!_stricmp(name, "Default") || checkname(shortcuts, name))
        {
            fprintf(stderr, "%s: Shortcut '%s' doesn't exist.\n", programname, name);
        }
        else
        {
            while(current)
            {
                if (!_stricmp(current->name, name))
                {
                    printf("%s", current->path);
                    break;
                }
                current = current->next;
            }
        }
        freeshortcuts(shortcuts);
    }

    return 0;
}

PSHORTCUT addshortcut(PSHORTCUT ptr, char* name, char* path)
{
    if (!ptr)
    {
        ptr = (PSHORTCUT)malloc(sizeof(SHORTCUT));
        ptr->name = (char*)malloc(strlen(name) + 1);
        strcpy(ptr->name, name);
        ptr->path = (char*)malloc(strlen(path) + 1);
        strcpy(ptr->path, path);
        ptr->next = NULL;
    }
    else
    {
        ptr->next = addshortcut(ptr->next, name, path);
    }

    return ptr;
}

void checkfile(void)
{
    FILE *FILE;

    FILE = fopen(shortcutfile, "r");
    if (!FILE)
    {
        FILE = fopen(shortcutfile, "w");
        if (!FILE)
        {
            fprintf(stderr, "%s: Error creating file.\n", programname);
        }
        else
        {
            fprintf(FILE, "Default,Default\n");
            if (fclose(FILE))
            {
                fprintf(stderr, "%s: Error closing file.\n", programname);
            }
        }
    }
    else
    {
        if (fclose(FILE))
        {
            fprintf(stderr, "%s: Error closing file.\n", programname);
        }
    }
}

int checkname(PSHORTCUT head, char* name)
{
    PSHORTCUT current = head;

    while(current)
    {
        if (!_stricmp(current->name, name))
        {
            return 0;
        }
        current = current->next;
    }

    return 1;
}

int checkpath(char* path)
{
    char currentdir[260];
    getcwd(currentdir, 260);
    if (!chdir(path))
    {
        getcwd(path, 260);
        chdir(currentdir);
        return 1;
    }

    return 0;
}

void defaultshortcut(char* name)
{
    PSHORTCUT shortcuts = NULL, current = NULL;
    char path[260];

    shortcuts = readshortcuts();
    current = shortcuts;

    if (!name)
    {
        while(current)
        {
            if (!_stricmp(current->name, "Default"))
            {
                printf("%s", current->path);
                break;
            }
            current = current->next;
        }
    }
    else
    {
        while(current)
        {
            if (!_stricmp(current->name, name))
            {
                strcpy(path, current->path);
                current = shortcuts;
                while(current)
                {
                    if (!_stricmp(current->name, "Default"))
                    {
                        editshortcut(current, "Default", path);
                        writeshortcuts(shortcuts);
                        printf("Default shortcut set to: %s\n", path);
                        break;
                    }
                    current = current->next;
                }
                break;
            }
            current = current->next;
        }
    }

    freeshortcuts(shortcuts);
}

PSHORTCUT deleteshortcut(PSHORTCUT current, PSHORTCUT *head)
{
    PSHORTCUT temp = NULL;
    PSHORTCUT previous = NULL;

    if (current)
    {
        temp = current;
        if (current != *head)
        {
            previous = previousshortcut(current, *head);
            current = current->next;
            previous->next = current;
        }
        else
        {
            *head = current->next;
        }
        if (temp->name) free(temp->name);
        if (temp->path) free(temp->path);
        free(temp);
    }

    return current;
}

void editshortcut(PSHORTCUT current, char* name, char* path)
{
    if (current)
    {
        if (name)
        {
            current->name = (char*)realloc(current->name, strlen(name) + 1);
            strcpy(current->name, name);
        }
        if (path)
        {
            current->path = (char*)realloc(current->path, strlen(path) + 1);
            strcpy(current->path, path);
        }
    }
}

void freeshortcuts(PSHORTCUT head)
{
    PSHORTCUT temp = NULL;

    while (head)
    {
        temp = head;
        head = head->next;
        if (temp->name) free(temp->name);
        if (temp->path) free(temp->path);
        free(temp);
    }
}

PSHORTCUT previousshortcut(PSHORTCUT current, PSHORTCUT head)
{
    PSHORTCUT temp = head;

    if(current == head)
    {
        return head;
    }

    while(temp->next != current)
    {
        temp = temp->next;
    }

    return temp;
}

PSHORTCUT readshortcuts(void)
{
    FILE *FILE;
    PSHORTCUT head = NULL;
    char strbuff[LINE_MAX];
    char *name = NULL, *path = NULL;

    FILE = fopen(shortcutfile, "r");
    if (!FILE)
    {
        fprintf(stderr, "%s: Error file doesn't seem to exist.\n", programname);
        return NULL;
    }
    else
    {
        while(!feof(FILE))
        {
            fgets(strbuff, LINE_MAX, FILE);
            name = strtok(strbuff, ",");
            path = strtok(NULL, "\n");
            if (name && path)
            {
                head = addshortcut(head, name, path);
            }
        }
        if (fclose(FILE))
        {
            fprintf(stderr, "%s: Error closing file.\n", programname);
            freeshortcuts(head);
            return NULL;
        }
    }

    return head;
}

int writeshortcuts(PSHORTCUT head)
{
    FILE *FILE;

    FILE = fopen(shortcutfile, "w");
    if (!FILE)
    {
        fprintf(stderr, "%s: Error file doesn't seem to exist.\n", programname);
        return -1;
    }
    else
    {
        while(head)
        {
            fprintf(FILE, "%s,%s\n", head->name, head->path);
            head = head->next;
        }
        if (fclose(FILE))
        {
            fprintf(stderr, "%s: Error closing file.\n", programname);
            return -1;
        }
    }

    return 0;
}
