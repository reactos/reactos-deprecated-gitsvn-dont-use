/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         BSD - See COPYING.ARM in the top level directory
 * FILE:            ntoskrnl/config/cmsysini.c
 * PURPOSE:         Configuration Manager - System Initialization Code
 * PROGRAMMERS:     ReactOS Portable Systems Group
 *                  Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES *******************************************************************/

#include "ntoskrnl.h"
#define NDEBUG
#include "debug.h"

POBJECT_TYPE CmpKeyObjectType;
PCMHIVE CmiVolatileHive;
LIST_ENTRY CmpHiveListHead;
ERESOURCE CmpRegistryLock;
KGUARDED_MUTEX CmpSelfHealQueueLock;
LIST_ENTRY CmpSelfHealQueueListHead;
KEVENT CmpLoadWorkerEvent;
LONG CmpLoadWorkerIncrement;
PEPROCESS CmpSystemProcess;
BOOLEAN HvShutdownComplete;
PVOID CmpRegistryLockCallerCaller, CmpRegistryLockCaller;
BOOLEAN CmpFlushOnLockRelease;
BOOLEAN CmpSpecialBootCondition;
BOOLEAN CmpNoWrite;
BOOLEAN CmpWasSetupBoot;
BOOLEAN CmpProfileLoaded;
BOOLEAN CmpNoVolatileCreates;
ULONG CmpTraceLevel = 0;

extern LONG CmpFlushStarveWriters;
extern BOOLEAN CmFirstTime;

/* FUNCTIONS ******************************************************************/

BOOLEAN
NTAPI
CmpLinkKeyToHive(
    _In_z_ PWSTR LinkKeyName,
    _In_z_ PWSTR TargetKeyName)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING LinkKeyName_U;
    HANDLE TargetKeyHandle;
    ULONG Disposition;
    NTSTATUS Status;
    PAGED_CODE();

    /* Initialize the object attributes */
    RtlInitUnicodeString(&LinkKeyName_U, LinkKeyName);
    InitializeObjectAttributes(&ObjectAttributes,
                               &LinkKeyName_U,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               NULL,
                               NULL);

    /* Create the link key */
    Status = ZwCreateKey(&TargetKeyHandle,
                         KEY_CREATE_LINK,
                         &ObjectAttributes,
                         0,
                         NULL,
                         REG_OPTION_VOLATILE | REG_OPTION_CREATE_LINK,
                         &Disposition);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("CM: CmpLinkKeyToHive: couldn't create %S Status = 0x%lx\n",
                LinkKeyName, Status);
        return FALSE;
    }

    /* Check if the new key was actually created */
    if (Disposition != REG_CREATED_NEW_KEY)
    {
        DPRINT1("CM: CmpLinkKeyToHive: %S already exists!\n", LinkKeyName);
        ZwClose(TargetKeyHandle);
        return FALSE;
    }

    /* Set the target key name as link target */
    Status = ZwSetValueKey(TargetKeyHandle,
                           &CmSymbolicLinkValueName,
                           0,
                           REG_LINK,
                           TargetKeyName,
                           (ULONG)wcslen(TargetKeyName) * sizeof(WCHAR));

    /* Close the link key handle */
    ObCloseHandle(TargetKeyHandle, KernelMode);

    if (!NT_SUCCESS(Status))
    {
        DPRINT1("CM: CmpLinkKeyToHive: couldn't create symbolic link for %S\n",
                TargetKeyName);
        return FALSE;
    }

    return TRUE;
}

VOID
NTAPI
CmpDeleteKeyObject(PVOID DeletedObject)
{
    PCM_KEY_BODY KeyBody = (PCM_KEY_BODY)DeletedObject;
    PCM_KEY_CONTROL_BLOCK Kcb;
    REG_KEY_HANDLE_CLOSE_INFORMATION KeyHandleCloseInfo;
    REG_POST_OPERATION_INFORMATION PostOperationInfo;
    NTSTATUS Status;
    PAGED_CODE();

    /* First off, prepare the handle close information callback */
    PostOperationInfo.Object = KeyBody;
    KeyHandleCloseInfo.Object = KeyBody;
    Status = CmiCallRegisteredCallbacks(RegNtPreKeyHandleClose,
                                        &KeyHandleCloseInfo);
    if (!NT_SUCCESS(Status))
    {
        /* If we failed, notify the post routine */
        PostOperationInfo.Status = Status;
        CmiCallRegisteredCallbacks(RegNtPostKeyHandleClose, &PostOperationInfo);
        return;
    }

    /* Acquire hive lock */
    CmpLockRegistry();

    /* Make sure this is a valid key body */
    if (KeyBody->Type == CM_KEY_BODY_TYPE)
    {
        /* Get the KCB */
        Kcb = KeyBody->KeyControlBlock;
        if (Kcb)
        {
            /* Delist the key */
            DelistKeyBodyFromKCB(KeyBody, FALSE);

            /* Dereference the KCB */
            CmpDelayDerefKeyControlBlock(Kcb);
        }
    }

    /* Release the registry lock */
    CmpUnlockRegistry();

    /* Do the post callback */
    PostOperationInfo.Status = STATUS_SUCCESS;
    CmiCallRegisteredCallbacks(RegNtPostKeyHandleClose, &PostOperationInfo);
}

VOID
NTAPI
CmpCloseKeyObject(IN PEPROCESS Process OPTIONAL,
                  IN PVOID Object,
                  IN ACCESS_MASK GrantedAccess,
                  IN ULONG ProcessHandleCount,
                  IN ULONG SystemHandleCount)
{
    PCM_KEY_BODY KeyBody = (PCM_KEY_BODY)Object;
    PAGED_CODE();

    /* Don't do anything if we're not the last handle */
    if (SystemHandleCount > 1) return;

    /* Make sure we're a valid key body */
    if (KeyBody->Type == CM_KEY_BODY_TYPE)
    {
        /* Don't do anything if we don't have a notify block */
        if (!KeyBody->NotifyBlock) return;

        /* This shouldn't happen yet */
        ASSERT(FALSE);
    }
}

NTSTATUS
NTAPI
CmpQueryKeyName(IN PVOID ObjectBody,
                IN BOOLEAN HasName,
                IN OUT POBJECT_NAME_INFORMATION ObjectNameInfo,
                IN ULONG Length,
                OUT PULONG ReturnLength,
                IN KPROCESSOR_MODE PreviousMode)
{
    PUNICODE_STRING KeyName;
    ULONG BytesToCopy;
    NTSTATUS Status = STATUS_SUCCESS;
    PCM_KEY_BODY KeyBody = (PCM_KEY_BODY)ObjectBody;
    PCM_KEY_CONTROL_BLOCK Kcb = KeyBody->KeyControlBlock;

    /* Acquire hive lock */
    CmpLockRegistry();

    /* Lock KCB shared */
    CmpAcquireKcbLockShared(Kcb);

    /* Check if it's a deleted block */
    if (Kcb->Delete)
    {
        /* Release the locks */
        CmpReleaseKcbLock(Kcb);
        CmpUnlockRegistry();

        /* Let the caller know it's deleted */
        return STATUS_KEY_DELETED;
    }

    /* Get the name */
    KeyName = CmpConstructName(Kcb);

    /* Release the locks */
    CmpReleaseKcbLock(Kcb);
    CmpUnlockRegistry();

    /* Check if we got the name */
    if (!KeyName) return STATUS_INSUFFICIENT_RESOURCES;

    /* Set the returned length */
    *ReturnLength = KeyName->Length + sizeof(OBJECT_NAME_INFORMATION) + sizeof(WCHAR);

    /* Calculate amount of bytes to copy into the buffer */
    BytesToCopy = KeyName->Length + sizeof(WCHAR);

    /* Check if the provided buffer is too small to fit even anything */
    if ((Length <= sizeof(OBJECT_NAME_INFORMATION)) ||
        ((Length < (*ReturnLength)) && (BytesToCopy < sizeof(WCHAR))))
    {
        /* Free the buffer allocated by CmpConstructName */
        ExFreePoolWithTag(KeyName, TAG_CM);

        /* Return buffer length failure without writing anything there because nothing fits */
        return STATUS_INFO_LENGTH_MISMATCH;
    }

    /* Check if the provided buffer can be partially written */
    if (Length < (*ReturnLength))
    {
        /* Yes, indicate so in the return status */
        Status = STATUS_INFO_LENGTH_MISMATCH;

        /* Calculate amount of bytes which the provided buffer could handle */
        BytesToCopy = Length - sizeof(OBJECT_NAME_INFORMATION);
    }

    /* Remove the null termination character from the size */
    BytesToCopy -= sizeof(WCHAR);

    /* Fill in the result */
    _SEH2_TRY
    {
        /* Return data to user */
        ObjectNameInfo->Name.Buffer = (PWCHAR)(ObjectNameInfo + 1);
        ObjectNameInfo->Name.MaximumLength = KeyName->Length;
        ObjectNameInfo->Name.Length = KeyName->Length;

        /* Copy string content*/
        RtlCopyMemory(ObjectNameInfo->Name.Buffer,
                      KeyName->Buffer,
                      BytesToCopy);

        /* Null terminate it */
        ObjectNameInfo->Name.Buffer[BytesToCopy / sizeof(WCHAR)] = 0;
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        /* Get the status */
        Status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;

    /* Free the buffer allocated by CmpConstructName */
    ExFreePoolWithTag(KeyName, TAG_CM);

    /* Return status */
    return Status;
}

NTSTATUS
NTAPI
CmpInitHiveFromFile(IN PCUNICODE_STRING HiveName,
                    IN ULONG HiveFlags,
                    OUT PCMHIVE *Hive,
                    IN OUT PBOOLEAN New,
                    IN ULONG CheckFlags)
{
    ULONG HiveDisposition, LogDisposition;
    HANDLE FileHandle = NULL, LogHandle = NULL;
    NTSTATUS Status;
    ULONG Operation, FileType;
    PCMHIVE NewHive;
    PAGED_CODE();

    /* Assume failure */
    *Hive = NULL;

    /* Open or create the hive files */
    Status = CmpOpenHiveFiles(HiveName,
                              L".LOG",
                              &FileHandle,
                              &LogHandle,
                              &HiveDisposition,
                              &LogDisposition,
                              *New,
                              FALSE,
                              TRUE,
                              NULL);
    if (!NT_SUCCESS(Status)) return Status;

    /* Check if we have a log handle */
    FileType = (LogHandle) ? HFILE_TYPE_LOG : HFILE_TYPE_PRIMARY;

    /* Check if we created or opened the hive */
    if (HiveDisposition == FILE_CREATED)
    {
        /* Do a create operation */
        Operation = HINIT_CREATE;
        *New = TRUE;
    }
    else
    {
        /* Open it as a file */
        Operation = HINIT_FILE;
        *New = FALSE;
    }

    /* Check if we're sharing hives */
    if (CmpShareSystemHives)
    {
        /* Then force using the primary hive */
        FileType = HFILE_TYPE_PRIMARY;
        if (LogHandle)
        {
            /* Get rid of the log handle */
            ZwClose(LogHandle);
            LogHandle = NULL;
        }
    }

    /* Check if we're too late */
    if (HvShutdownComplete)
    {
        /* Fail */
        ZwClose(FileHandle);
        if (LogHandle) ZwClose(LogHandle);
        return STATUS_TOO_LATE;
    }

    /* Initialize the hive */
    Status = CmpInitializeHive(&NewHive,
                               Operation,
                               HiveFlags,
                               FileType,
                               NULL,
                               FileHandle,
                               LogHandle,
                               NULL,
                               HiveName,
                               CheckFlags);
    if (!NT_SUCCESS(Status))
    {
        /* Fail */
        ZwClose(FileHandle);
        if (LogHandle) ZwClose(LogHandle);
        return Status;
    }

    /* Success, return hive */
    *Hive = NewHive;

    /* Duplicate the hive name */
    NewHive->FileFullPath.Buffer = ExAllocatePoolWithTag(PagedPool,
                                                         HiveName->Length,
                                                         TAG_CM);
    if (NewHive->FileFullPath.Buffer)
    {
        /* Copy the string */
        RtlCopyMemory(NewHive->FileFullPath.Buffer,
                      HiveName->Buffer,
                      HiveName->Length);
        NewHive->FileFullPath.Length = HiveName->Length;
        NewHive->FileFullPath.MaximumLength = HiveName->MaximumLength;
    }

    /* Return success */
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
INIT_FUNCTION
CmpSetSystemValues(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING KeyName, ValueName = { 0, 0, NULL };
    HANDLE KeyHandle = NULL;
    NTSTATUS Status;
    ASSERT(LoaderBlock != NULL);

    /* Setup attributes for loader options */
    RtlInitUnicodeString(&KeyName,
                         L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\"
                         L"Control");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(&KeyHandle, KEY_WRITE, &ObjectAttributes);
    if (!NT_SUCCESS(Status)) goto Quickie;

    /* Key opened, now write to the key */
    RtlInitUnicodeString(&KeyName, L"SystemStartOptions");
    Status = NtSetValueKey(KeyHandle,
                           &KeyName,
                           0,
                           REG_SZ,
                           CmpLoadOptions.Buffer,
                           CmpLoadOptions.Length);
    if (!NT_SUCCESS(Status)) goto Quickie;

    /* Setup value name for system boot device in ARC format */
    RtlInitUnicodeString(&KeyName, L"SystemBootDevice");
    RtlCreateUnicodeStringFromAsciiz(&ValueName, LoaderBlock->ArcBootDeviceName);
    Status = NtSetValueKey(KeyHandle,
                           &KeyName,
                           0,
                           REG_SZ,
                           ValueName.Buffer,
                           ValueName.Length);

Quickie:
    /* Free the buffers */
    RtlFreeUnicodeString(&ValueName);

    /* Close the key and return */
    if (KeyHandle) NtClose(KeyHandle);

    /* Return the status */
    return (ExpInTextModeSetup ? STATUS_SUCCESS : Status);
}

static
NTSTATUS
INIT_FUNCTION
CmpCreateHardwareProfile(HANDLE ControlSetHandle)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING KeyName;
    HANDLE ProfilesHandle = NULL;
    HANDLE ProfileHandle = NULL;
    ULONG Disposition;
    NTSTATUS Status;

    DPRINT("CmpCreateHardwareProfile()\n");

    /* Create the Hardware Profiles key */
    RtlInitUnicodeString(&KeyName, L"Hardware Profiles");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               ControlSetHandle,
                               NULL);
    Status = NtCreateKey(&ProfilesHandle,
                         KEY_ALL_ACCESS,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         &Disposition);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Creating the Hardware Profile key failed\n");
        goto done;
    }

    /* Sanity check */
    ASSERT(Disposition == REG_CREATED_NEW_KEY);

    /* Create the 0000 key */
    RtlInitUnicodeString(&KeyName, L"0000");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               ProfilesHandle,
                               NULL);
    Status = NtCreateKey(&ProfileHandle,
                         KEY_ALL_ACCESS,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         &Disposition);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Creating the Hardware Profile\\0000 key failed\n");
        goto done;
    }

    /* Sanity check */
    ASSERT(Disposition == REG_CREATED_NEW_KEY);

done:
    if (ProfilesHandle)
        NtClose(ProfilesHandle);

    if (ProfileHandle)
        NtClose(ProfileHandle);

    DPRINT1("CmpCreateHardwareProfile() done\n");

    return Status;
}

NTSTATUS
NTAPI
INIT_FUNCTION
CmpCreateControlSet(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    UNICODE_STRING ConfigName = RTL_CONSTANT_STRING(L"Control\\IDConfigDB");
    UNICODE_STRING SelectName =
        RTL_CONSTANT_STRING(L"\\Registry\\Machine\\System\\Select");
    UNICODE_STRING KeyName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    CHAR ValueInfoBuffer[128];
    PKEY_VALUE_FULL_INFORMATION ValueInfo;
    CHAR Buffer[128];
    WCHAR UnicodeBuffer[128];
    HANDLE SelectHandle, KeyHandle, ConfigHandle = NULL, ProfileHandle = NULL;
    HANDLE ParentHandle = NULL;
    ULONG ControlSet, HwProfile;
    ANSI_STRING TempString;
    NTSTATUS Status;
    ULONG ResultLength, Disposition;
    PLOADER_PARAMETER_EXTENSION LoaderExtension;
    PAGED_CODE();

    /* Open the select key */
    InitializeObjectAttributes(&ObjectAttributes,
                               &SelectName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(&SelectHandle, KEY_READ, &ObjectAttributes);
    if (!NT_SUCCESS(Status))
    {
        /* ReactOS Hack: Hard-code current to 001 for SetupLdr */
        if (!LoaderBlock->RegistryBase)
        {
            /* Build the ControlSet001 key */
            RtlInitUnicodeString(&KeyName,
                                 L"\\Registry\\Machine\\System\\ControlSet001");
            InitializeObjectAttributes(&ObjectAttributes,
                                       &KeyName,
                                       OBJ_CASE_INSENSITIVE,
                                       NULL,
                                       NULL);
            Status = NtCreateKey(&KeyHandle,
                                 KEY_ALL_ACCESS,
                                 &ObjectAttributes,
                                 0,
                                 NULL,
                                 0,
                                 &Disposition);
            if (!NT_SUCCESS(Status)) return Status;

            /* Create the Hardware Profile keys */
            Status = CmpCreateHardwareProfile(KeyHandle);
            if (!NT_SUCCESS(Status))
                return Status;

            /* Don't need the handle */
            ZwClose(KeyHandle);

            /* Use hard-coded setting */
            ControlSet = 1;
            goto UseSet;
        }

        /* Fail for real boots */
        return Status;
    }

    /* Open the current value */
    RtlInitUnicodeString(&KeyName, L"Current");
    Status = NtQueryValueKey(SelectHandle,
                             &KeyName,
                             KeyValueFullInformation,
                             ValueInfoBuffer,
                             sizeof(ValueInfoBuffer),
                             &ResultLength);
    NtClose(SelectHandle);
    if (!NT_SUCCESS(Status)) return Status;

    /* Get the actual value pointer, and get the control set ID */
    ValueInfo = (PKEY_VALUE_FULL_INFORMATION)ValueInfoBuffer;
    ControlSet = *(PULONG)((PUCHAR)ValueInfo + ValueInfo->DataOffset);

    /* Create the current control set key */
UseSet:
    RtlInitUnicodeString(&KeyName,
                         L"\\Registry\\Machine\\System\\CurrentControlSet");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtCreateKey(&KeyHandle,
                         KEY_CREATE_LINK,
                         &ObjectAttributes,
                         0,
                         NULL,
                         REG_OPTION_VOLATILE | REG_OPTION_CREATE_LINK,
                         &Disposition);
    if (!NT_SUCCESS(Status)) return Status;

    /* Sanity check */
    ASSERT(Disposition == REG_CREATED_NEW_KEY);

    /* Initialize the symbolic link name */
    sprintf(Buffer,
            "\\Registry\\Machine\\System\\ControlSet%03ld",
            ControlSet);
    RtlInitAnsiString(&TempString, Buffer);

    /* Create a Unicode string out of it */
    KeyName.MaximumLength = sizeof(UnicodeBuffer);
    KeyName.Buffer = UnicodeBuffer;
    Status = RtlAnsiStringToUnicodeString(&KeyName, &TempString, FALSE);

    /* Set the value */
    Status = NtSetValueKey(KeyHandle,
                           &CmSymbolicLinkValueName,
                           0,
                           REG_LINK,
                           KeyName.Buffer,
                           KeyName.Length);
    if (!NT_SUCCESS(Status)) return Status;

    /* Get the configuration database key */
    InitializeObjectAttributes(&ObjectAttributes,
                               &ConfigName,
                               OBJ_CASE_INSENSITIVE,
                               KeyHandle,
                               NULL);
    Status = NtOpenKey(&ConfigHandle, KEY_READ, &ObjectAttributes);
    NtClose(KeyHandle);

    /* Check if we don't have one */
    if (!NT_SUCCESS(Status))
    {
        /* Cleanup and exit */
        ConfigHandle = 0;
        goto Cleanup;
    }

    /* ReactOS Hack: Hard-code current to 001 for SetupLdr */
    if (!LoaderBlock->RegistryBase)
    {
        HwProfile = 0;
    }
    else
    {
        /* Now get the current config */
        RtlInitUnicodeString(&KeyName, L"CurrentConfig");
        Status = NtQueryValueKey(ConfigHandle,
                                 &KeyName,
                                 KeyValueFullInformation,
                                 ValueInfoBuffer,
                                 sizeof(ValueInfoBuffer),
                                 &ResultLength);

        /* Set pointer to buffer */
        ValueInfo = (PKEY_VALUE_FULL_INFORMATION)ValueInfoBuffer;

        /* Check if we failed or got a non DWORD-value */
        if (!(NT_SUCCESS(Status)) || (ValueInfo->Type != REG_DWORD)) goto Cleanup;

        /* Get the hadware profile */
        HwProfile = *(PULONG)((PUCHAR)ValueInfo + ValueInfo->DataOffset);
    }

    /* Open the hardware profile key */
    RtlInitUnicodeString(&KeyName,
                         L"\\Registry\\Machine\\System\\CurrentControlSet"
                         L"\\Hardware Profiles");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(&ParentHandle, KEY_READ, &ObjectAttributes);
    if (!NT_SUCCESS(Status))
    {
        /* Exit and clean up */
        ParentHandle = 0;
        goto Cleanup;
    }

    /* Build the profile name */
    sprintf(Buffer, "%04ld", HwProfile);
    RtlInitAnsiString(&TempString, Buffer);

    /* Convert it to Unicode */
    KeyName.MaximumLength = sizeof(UnicodeBuffer);
    KeyName.Buffer = UnicodeBuffer;
    Status = RtlAnsiStringToUnicodeString(&KeyName,
                                          &TempString,
                                          FALSE);
    ASSERT(Status == STATUS_SUCCESS);

    /* Open the associated key */
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               ParentHandle,
                               NULL);
    Status = NtOpenKey(&ProfileHandle,
                       KEY_READ | KEY_WRITE,
                       &ObjectAttributes);
    if (!NT_SUCCESS (Status))
    {
        /* Cleanup and exit */
        ProfileHandle = 0;
        goto Cleanup;
    }

    /* Check if we have a loader block extension */
    LoaderExtension = LoaderBlock->Extension;
    if (LoaderExtension)
    {
        DPRINT("ReactOS doesn't support NTLDR Profiles yet!\n");
    }

    /* Create the current hardware profile key */
    RtlInitUnicodeString(&KeyName,
                         L"\\Registry\\Machine\\System\\CurrentControlSet\\"
                         L"Hardware Profiles\\Current");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtCreateKey(&KeyHandle,
                         KEY_CREATE_LINK,
                         &ObjectAttributes,
                         0,
                         NULL,
                         REG_OPTION_VOLATILE | REG_OPTION_CREATE_LINK,
                         &Disposition);
    if (NT_SUCCESS(Status))
    {
        /* Sanity check */
        ASSERT(Disposition == REG_CREATED_NEW_KEY);

        /* Create the profile name */
        sprintf(Buffer,
                "\\Registry\\Machine\\System\\CurrentControlSet\\"
                "Hardware Profiles\\%04ld",
                HwProfile);
        RtlInitAnsiString(&TempString, Buffer);

        /* Convert it to Unicode */
        KeyName.MaximumLength = sizeof(UnicodeBuffer);
        KeyName.Buffer = UnicodeBuffer;
        Status = RtlAnsiStringToUnicodeString(&KeyName,
                                              &TempString,
                                              FALSE);
        ASSERT(STATUS_SUCCESS == Status);

        /* Set it */
        Status = NtSetValueKey(KeyHandle,
                               &CmSymbolicLinkValueName,
                               0,
                               REG_LINK,
                               KeyName.Buffer,
                               KeyName.Length);
        NtClose(KeyHandle);
    }

    /* Close every opened handle */
Cleanup:
    if (ConfigHandle) NtClose(ConfigHandle);
    if (ProfileHandle) NtClose(ProfileHandle);
    if (ParentHandle) NtClose(ParentHandle);

    DPRINT("CmpCreateControlSet() done\n");

    /* Return success */
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
CmpLinkHiveToMaster(IN PUNICODE_STRING LinkName,
                    IN HANDLE RootDirectory,
                    IN PCMHIVE RegistryHive,
                    IN BOOLEAN Allocate,
                    IN PSECURITY_DESCRIPTOR SecurityDescriptor)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    NTSTATUS Status;
    CM_PARSE_CONTEXT ParseContext = {0};
    HANDLE KeyHandle;
    PCM_KEY_BODY KeyBody;
    PAGED_CODE();

    /* Setup the object attributes */
    InitializeObjectAttributes(&ObjectAttributes,
                               LinkName,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               RootDirectory,
                               SecurityDescriptor);

    /* Setup the parse context */
    ParseContext.CreateLink = TRUE;
    ParseContext.CreateOperation = TRUE;
    ParseContext.ChildHive.KeyHive = &RegistryHive->Hive;

    /* Check if we have a root keycell or if we need to create it */
    if (Allocate)
    {
        /* Create it */
        ParseContext.ChildHive.KeyCell = HCELL_NIL;
    }
    else
    {
        /* We have one */
        ParseContext.ChildHive.KeyCell = RegistryHive->Hive.BaseBlock->RootCell;
    }

    /* Create the link node */
    Status = ObOpenObjectByName(&ObjectAttributes,
                                CmpKeyObjectType,
                                KernelMode,
                                NULL,
                                KEY_READ | KEY_WRITE,
                                (PVOID)&ParseContext,
                                &KeyHandle);
    if (!NT_SUCCESS(Status)) return Status;

    /* Mark the hive as clean */
    RegistryHive->Hive.DirtyFlag = FALSE;

    /* ReactOS Hack: Keep alive */
    Status = ObReferenceObjectByHandle(KeyHandle,
                                       0,
                                       CmpKeyObjectType,
                                       KernelMode,
                                       (PVOID*)&KeyBody,
                                       NULL);
    ASSERT(NT_SUCCESS(Status));

    /* Close the extra handle */
    ZwClose(KeyHandle);
    return STATUS_SUCCESS;
}

BOOLEAN
NTAPI
INIT_FUNCTION
CmpInitializeSystemHive(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    PVOID HiveBase;
    ANSI_STRING LoadString;
    PVOID Buffer;
    ULONG Length;
    NTSTATUS Status;
    BOOLEAN Allocate;
    UNICODE_STRING KeyName;
    PCMHIVE SystemHive = NULL;
    UNICODE_STRING HiveName = RTL_CONSTANT_STRING(L"SYSTEM");
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PAGED_CODE();

    /* Setup the ansi string */
    RtlInitAnsiString(&LoadString, LoaderBlock->LoadOptions);

    /* Allocate the unicode buffer */
    Length = LoadString.Length * sizeof(WCHAR) + sizeof(UNICODE_NULL);
    Buffer = ExAllocatePoolWithTag(PagedPool, Length, TAG_CM);
    if (!Buffer)
    {
        /* Fail */
        KeBugCheckEx(BAD_SYSTEM_CONFIG_INFO, 3, 1, (ULONG_PTR)LoaderBlock, 0);
    }

    /* Setup the unicode string */
    RtlInitEmptyUnicodeString(&CmpLoadOptions, Buffer, (USHORT)Length);

    /* Add the load options and null-terminate */
    RtlAnsiStringToUnicodeString(&CmpLoadOptions, &LoadString, FALSE);
    CmpLoadOptions.Buffer[LoadString.Length] = UNICODE_NULL;
    CmpLoadOptions.Length += sizeof(WCHAR);

    /* Get the System Hive base address */
    HiveBase = LoaderBlock->RegistryBase;
    if (HiveBase)
    {
        /* Import it */
        Status = CmpInitializeHive(&SystemHive,
                                   HINIT_MEMORY,
                                   HIVE_NOLAZYFLUSH,
                                   HFILE_TYPE_LOG,
                                   HiveBase,
                                   NULL,
                                   NULL,
                                   NULL,
                                   &HiveName,
                                   2);
        if (!NT_SUCCESS(Status)) return FALSE;

        /* Set the hive filename */
        RtlCreateUnicodeString(&SystemHive->FileFullPath,
                               L"\\SystemRoot\\System32\\Config\\SYSTEM");

        /* We imported, no need to create a new hive */
        Allocate = FALSE;

        /* Manually set the hive as volatile, if in Live CD mode */
        if (CmpShareSystemHives) SystemHive->Hive.HiveFlags = HIVE_VOLATILE;
    }
    else
    {
        /* Create it */
        Status = CmpInitializeHive(&SystemHive,
                                   HINIT_CREATE,
                                   HIVE_NOLAZYFLUSH,
                                   HFILE_TYPE_LOG,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   &HiveName,
                                   0);
        if (!NT_SUCCESS(Status)) return FALSE;

        /* Set the hive filename */
        RtlCreateUnicodeString(&SystemHive->FileFullPath,
                               L"\\SystemRoot\\System32\\Config\\SYSTEM");

        /* Tell CmpLinkHiveToMaster to allocate a hive */
        Allocate = TRUE;
    }

    /* Save the boot type */
    CmpBootType = SystemHive->Hive.BaseBlock->BootType;

    /* Are we in self-healing mode? */
    if (!CmSelfHeal)
    {
        /* Disable self-healing internally and check if boot type wanted it */
        CmpSelfHeal = FALSE;
        if (CmpBootType & 4)
        {
            /* We're disabled, so bugcheck */
            KeBugCheckEx(BAD_SYSTEM_CONFIG_INFO,
                         3,
                         3,
                         (ULONG_PTR)SystemHive,
                         0);
        }
    }

    /* Create the default security descriptor */
    SecurityDescriptor = CmpHiveRootSecurityDescriptor();

    /* Attach it to the system key */
    RtlInitUnicodeString(&KeyName, L"\\Registry\\Machine\\SYSTEM");
    Status = CmpLinkHiveToMaster(&KeyName,
                                 NULL,
                                 SystemHive,
                                 Allocate,
                                 SecurityDescriptor);

    /* Free the security descriptor */
    ExFreePoolWithTag(SecurityDescriptor, TAG_CMSD);
    if (!NT_SUCCESS(Status)) return FALSE;

    /* Add the hive to the hive list */
    CmpMachineHiveList[3].CmHive = SystemHive;

    /* Success! */
    return TRUE;
}

NTSTATUS
NTAPI
INIT_FUNCTION
CmpCreateObjectTypes(VOID)
{
    OBJECT_TYPE_INITIALIZER ObjectTypeInitializer;
    UNICODE_STRING Name;
    GENERIC_MAPPING CmpKeyMapping = {KEY_READ,
                                     KEY_WRITE,
                                     KEY_EXECUTE,
                                     KEY_ALL_ACCESS};
    PAGED_CODE();

    /* Initialize the Key object type */
    RtlZeroMemory(&ObjectTypeInitializer, sizeof(ObjectTypeInitializer));
    RtlInitUnicodeString(&Name, L"Key");
    ObjectTypeInitializer.Length = sizeof(ObjectTypeInitializer);
    ObjectTypeInitializer.DefaultPagedPoolCharge = sizeof(CM_KEY_BODY);
    ObjectTypeInitializer.GenericMapping = CmpKeyMapping;
    ObjectTypeInitializer.PoolType = PagedPool;
    ObjectTypeInitializer.ValidAccessMask = KEY_ALL_ACCESS;
    ObjectTypeInitializer.UseDefaultObject = TRUE;
    ObjectTypeInitializer.DeleteProcedure = CmpDeleteKeyObject;
    ObjectTypeInitializer.ParseProcedure = CmpParseKey;
    ObjectTypeInitializer.SecurityProcedure = CmpSecurityMethod;
    ObjectTypeInitializer.QueryNameProcedure = CmpQueryKeyName;
    ObjectTypeInitializer.CloseProcedure = CmpCloseKeyObject;
    ObjectTypeInitializer.SecurityRequired = TRUE;
    ObjectTypeInitializer.InvalidAttributes = OBJ_EXCLUSIVE | OBJ_PERMANENT;

    /* Create it */
    return ObCreateObjectType(&Name, &ObjectTypeInitializer, NULL, &CmpKeyObjectType);
}

BOOLEAN
NTAPI
INIT_FUNCTION
CmpCreateRootNode(IN PHHIVE Hive,
                  IN PCWSTR Name,
                  OUT PHCELL_INDEX Index)
{
    UNICODE_STRING KeyName;
    PCM_KEY_NODE KeyCell;
    PAGED_CODE();

    /* Initialize the node name and allocate it */
    RtlInitUnicodeString(&KeyName, Name);
    *Index = HvAllocateCell(Hive,
                            FIELD_OFFSET(CM_KEY_NODE, Name) +
                            CmpNameSize(Hive, &KeyName),
                            Stable,
                            HCELL_NIL);
    if (*Index == HCELL_NIL) return FALSE;

    /* Set the cell index and get the data */
    Hive->BaseBlock->RootCell = *Index;
    KeyCell = (PCM_KEY_NODE)HvGetCell(Hive, *Index);
    if (!KeyCell) return FALSE;

    /* Setup the cell */
    KeyCell->Signature = CM_KEY_NODE_SIGNATURE;
    KeyCell->Flags = KEY_HIVE_ENTRY | KEY_NO_DELETE;
    KeQuerySystemTime(&KeyCell->LastWriteTime);
    KeyCell->Parent = HCELL_NIL;
    KeyCell->SubKeyCounts[Stable] = 0;
    KeyCell->SubKeyCounts[Volatile] = 0;
    KeyCell->SubKeyLists[Stable] = HCELL_NIL;
    KeyCell->SubKeyLists[Volatile] = HCELL_NIL;
    KeyCell->ValueList.Count = 0;
    KeyCell->ValueList.List = HCELL_NIL;
    KeyCell->Security = HCELL_NIL;
    KeyCell->Class = HCELL_NIL;
    KeyCell->ClassLength = 0;
    KeyCell->MaxNameLen = 0;
    KeyCell->MaxClassLen = 0;
    KeyCell->MaxValueNameLen = 0;
    KeyCell->MaxValueDataLen = 0;

    /* Copy the name (this will also set the length) */
    KeyCell->NameLength = CmpCopyName(Hive, KeyCell->Name, &KeyName);

    /* Check if the name was compressed and set the flag if so */
    if (KeyCell->NameLength < KeyName.Length)
        KeyCell->Flags |= KEY_COMP_NAME;

    /* Return success */
    HvReleaseCell(Hive, *Index);
    return TRUE;
}

BOOLEAN
NTAPI
INIT_FUNCTION
CmpCreateRegistryRoot(VOID)
{
    UNICODE_STRING KeyName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    PCM_KEY_BODY RootKey;
    HCELL_INDEX RootIndex;
    NTSTATUS Status;
    PCM_KEY_NODE KeyCell;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PCM_KEY_CONTROL_BLOCK Kcb;
    PAGED_CODE();

    /* Setup the root node */
    if (!CmpCreateRootNode(&CmiVolatileHive->Hive, L"REGISTRY", &RootIndex))
    {
        /* We failed */
        return FALSE;
    }

    /* Create '\Registry' key. */
    RtlInitUnicodeString(&KeyName, L"\\REGISTRY");
    SecurityDescriptor = CmpHiveRootSecurityDescriptor();
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               SecurityDescriptor);
    Status = ObCreateObject(KernelMode,
                            CmpKeyObjectType,
                            &ObjectAttributes,
                            KernelMode,
                            NULL,
                            sizeof(CM_KEY_BODY),
                            0,
                            0,
                            (PVOID*)&RootKey);
    ExFreePoolWithTag(SecurityDescriptor, TAG_CMSD);
    if (!NT_SUCCESS(Status)) return FALSE;

    /* Sanity check, and get the key cell */
    ASSERT((&CmiVolatileHive->Hive)->ReleaseCellRoutine == NULL);
    KeyCell = (PCM_KEY_NODE)HvGetCell(&CmiVolatileHive->Hive, RootIndex);
    if (!KeyCell) return FALSE;

    /* Create the KCB */
    RtlInitUnicodeString(&KeyName, L"\\REGISTRY");
    Kcb = CmpCreateKeyControlBlock(&CmiVolatileHive->Hive,
                                   RootIndex,
                                   KeyCell,
                                   NULL,
                                   0,
                                   &KeyName);
    if (!Kcb)
    {
        ObDereferenceObject(RootKey);
        return FALSE;
    }

    /* Initialize the object */
    RootKey->KeyControlBlock = Kcb;
    RootKey->Type = CM_KEY_BODY_TYPE;
    RootKey->NotifyBlock = NULL;
    RootKey->ProcessID = PsGetCurrentProcessId();

    /* Link with KCB */
    EnlistKeyBodyWithKCB(RootKey, 0);

    /* Insert the key into the namespace */
    Status = ObInsertObject(RootKey,
                            NULL,
                            KEY_ALL_ACCESS,
                            0,
                            NULL,
                            &CmpRegistryRootHandle);
    if (!NT_SUCCESS(Status))
    {
        ObDereferenceObject(RootKey);
        return FALSE;
    }

    /* Reference the key again so that we never lose it */
    Status = ObReferenceObjectByHandle(CmpRegistryRootHandle,
                                       KEY_READ,
                                       NULL,
                                       KernelMode,
                                       (PVOID*)&RootKey,
                                       NULL);
    if (!NT_SUCCESS(Status))
    {
        ObDereferenceObject(RootKey);
        return FALSE;
    }

    /* Completely sucessful */
    return TRUE;
}

NTSTATUS
NTAPI
CmpGetRegistryPath(IN PWCHAR ConfigPath)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    NTSTATUS Status;
    HANDLE KeyHandle;
    PKEY_VALUE_PARTIAL_INFORMATION ValueInfo;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\HARDWARE");
    UNICODE_STRING ValueName = RTL_CONSTANT_STRING(L"InstallPath");
    ULONG BufferSize, ResultSize;

    /* Check if we are booted in setup */
    if (ExpInTextModeSetup)
    {
        /* Setup the object attributes */
        InitializeObjectAttributes(&ObjectAttributes,
                                   &KeyName,
                                   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                   NULL,
                                   NULL);
        /* Open the key */
        Status =  ZwOpenKey(&KeyHandle,
                            KEY_ALL_ACCESS,
                            &ObjectAttributes);
        if (!NT_SUCCESS(Status)) return Status;

        /* Allocate the buffer */
        BufferSize = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + 4096;
        ValueInfo = ExAllocatePoolWithTag(PagedPool, BufferSize, TAG_CM);
        if (!ValueInfo)
        {
            /* Fail */
            ZwClose(KeyHandle);
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        /* Query the value */
        Status = ZwQueryValueKey(KeyHandle,
                                 &ValueName,
                                 KeyValuePartialInformation,
                                 ValueInfo,
                                 BufferSize,
                                 &ResultSize);
        ZwClose(KeyHandle);
        if (!NT_SUCCESS(Status))
        {
            /* Fail */
            ExFreePoolWithTag(ValueInfo, TAG_CM);
            return Status;
        }

        /* Copy the config path and null-terminate it */
        RtlCopyMemory(ConfigPath,
                      ValueInfo->Data,
                      ValueInfo->DataLength);
        ConfigPath[ValueInfo->DataLength / sizeof(WCHAR)] = UNICODE_NULL;
        ExFreePoolWithTag(ValueInfo, TAG_CM);
    }
    else
    {
        /* Just use default path */
        wcscpy(ConfigPath, L"\\SystemRoot");
    }

    /* Add registry path */
    wcscat(ConfigPath, L"\\System32\\Config\\");

    /* Done */
    return STATUS_SUCCESS;
}

VOID
NTAPI
CmpLoadHiveThread(IN PVOID StartContext)
{
    WCHAR FileBuffer[MAX_PATH], RegBuffer[MAX_PATH], ConfigPath[MAX_PATH];
    UNICODE_STRING TempName, FileName, RegName;
    ULONG i, ErrorResponse, WorkerCount, Length;
    USHORT FileStart;
    //ULONG RegStart;
    ULONG PrimaryDisposition, SecondaryDisposition, ClusterSize;
    PCMHIVE CmHive;
    HANDLE PrimaryHandle = NULL, LogHandle = NULL;
    NTSTATUS Status = STATUS_SUCCESS;
    PVOID ErrorParameters;
    PAGED_CODE();

    /* Get the hive index, make sure it makes sense */
    i = PtrToUlong(StartContext);
    ASSERT(CmpMachineHiveList[i].Name != NULL);

    /* We were started */
    CmpMachineHiveList[i].ThreadStarted = TRUE;

    /* Build the file name and registry name strings */
    RtlInitEmptyUnicodeString(&FileName, FileBuffer, MAX_PATH);
    RtlInitEmptyUnicodeString(&RegName, RegBuffer, MAX_PATH);

    /* Now build the system root path */
    CmpGetRegistryPath(ConfigPath);
    RtlInitUnicodeString(&TempName, ConfigPath);
    RtlAppendStringToString((PSTRING)&FileName, (PSTRING)&TempName);
    FileStart = FileName.Length;

    /* And build the registry root path */
    RtlInitUnicodeString(&TempName, L"\\REGISTRY\\");
    RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);
    //RegStart = RegName.Length;

    /* Build the base name */
    RtlInitUnicodeString(&TempName, CmpMachineHiveList[i].BaseName);
    RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);

    /* Check if this is a child of the root */
    if (RegName.Buffer[RegName.Length / sizeof(WCHAR) - 1] == '\\')
    {
        /* Then setup the whole name */
        RtlInitUnicodeString(&TempName, CmpMachineHiveList[i].Name);
        RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);
    }

    /* Now add the rest of the file name */
    RtlInitUnicodeString(&TempName, CmpMachineHiveList[i].Name);
    FileName.Length = FileStart;
    RtlAppendStringToString((PSTRING)&FileName, (PSTRING)&TempName);
    if (!CmpMachineHiveList[i].CmHive)
    {
        /* We need to allocate a new hive structure */
        CmpMachineHiveList[i].Allocate = TRUE;

        /* Load the hive file */
        Status = CmpInitHiveFromFile(&FileName,
                                     CmpMachineHiveList[i].HHiveFlags,
                                     &CmHive,
                                     &CmpMachineHiveList[i].Allocate,
                                     0);
        if (!(NT_SUCCESS(Status)) ||
            (!(CmHive->FileHandles[HFILE_TYPE_LOG]) && !(CmpMiniNTBoot))) // HACK
        {
            /* We failed or couldn't get a log file, raise a hard error */
            ErrorParameters = &FileName;
            NtRaiseHardError(STATUS_CANNOT_LOAD_REGISTRY_FILE,
                             1,
                             1,
                             (PULONG_PTR)&ErrorParameters,
                             OptionOk,
                             &ErrorResponse);
        }

        /* Set the hive flags and newly allocated hive pointer */
        CmHive->Flags = CmpMachineHiveList[i].CmHiveFlags;
        CmpMachineHiveList[i].CmHive2 = CmHive;
    }
    else
    {
        /* We already have a hive, is it volatile? */
        CmHive = CmpMachineHiveList[i].CmHive;
        if (!(CmHive->Hive.HiveFlags & HIVE_VOLATILE))
        {
            /* It's now, open the hive file and log */
            Status = CmpOpenHiveFiles(&FileName,
                                      L".LOG",
                                      &PrimaryHandle,
                                      &LogHandle,
                                      &PrimaryDisposition,
                                      &SecondaryDisposition,
                                      TRUE,
                                      TRUE,
                                      FALSE,
                                      &ClusterSize);
            if (!(NT_SUCCESS(Status)) || !(LogHandle))
            {
                /* Couldn't open the hive or its log file, raise a hard error */
                ErrorParameters = &FileName;
                NtRaiseHardError(STATUS_CANNOT_LOAD_REGISTRY_FILE,
                                 1,
                                 1,
                                 (PULONG_PTR)&ErrorParameters,
                                 OptionOk,
                                 &ErrorResponse);

                /* And bugcheck for posterity's sake */
                KeBugCheckEx(BAD_SYSTEM_CONFIG_INFO, 9, 0, i, Status);
            }

            /* Save the file handles. This should remove our sync hacks */
            CmHive->FileHandles[HFILE_TYPE_LOG] = LogHandle;
            CmHive->FileHandles[HFILE_TYPE_PRIMARY] = PrimaryHandle;

            /* Allow lazy flushing since the handles are there -- remove sync hacks */
            //ASSERT(CmHive->Hive.HiveFlags & HIVE_NOLAZYFLUSH);
            CmHive->Hive.HiveFlags &= ~HIVE_NOLAZYFLUSH;

            /* Get the real size of the hive */
            Length = CmHive->Hive.Storage[Stable].Length + HBLOCK_SIZE;

            /* Check if the cluster size doesn't match */
            if (CmHive->Hive.Cluster != ClusterSize) ASSERT(FALSE);

            /* Set the file size */
            DPRINT("FIXME: Should set file size: %lx\n", Length);
            //if (!CmpFileSetSize((PHHIVE)CmHive, HFILE_TYPE_PRIMARY, Length, Length))
            {
                /* This shouldn't fail */
                //ASSERT(FALSE);
            }

            /* Another thing we don't support is NTLDR-recovery */
            if (CmHive->Hive.BaseBlock->BootRecover) ASSERT(FALSE);

            /* Finally, set our allocated hive to the same hive we've had */
            CmpMachineHiveList[i].CmHive2 = CmHive;
            ASSERT(CmpMachineHiveList[i].CmHive == CmpMachineHiveList[i].CmHive2);
        }
    }

    /* We're done */
    CmpMachineHiveList[i].ThreadFinished = TRUE;

    /* Check if we're the last worker */
    WorkerCount = InterlockedIncrement(&CmpLoadWorkerIncrement);
    if (WorkerCount == CM_NUMBER_OF_MACHINE_HIVES)
    {
        /* Signal the event */
        KeSetEvent(&CmpLoadWorkerEvent, 0, FALSE);
    }

    /* Kill the thread */
    PsTerminateSystemThread(Status);
}

VOID
NTAPI
CmpInitializeHiveList(IN USHORT Flag)
{
    WCHAR FileBuffer[MAX_PATH], RegBuffer[MAX_PATH], ConfigPath[MAX_PATH];
    UNICODE_STRING TempName, FileName, RegName;
    HANDLE Thread;
    NTSTATUS Status;
    ULONG i;
    USHORT RegStart;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PAGED_CODE();

    /* Allow writing for now */
    CmpNoWrite = FALSE;

    /* Build the file name and registry name strings */
    RtlInitEmptyUnicodeString(&FileName, FileBuffer, MAX_PATH);
    RtlInitEmptyUnicodeString(&RegName, RegBuffer, MAX_PATH);

    /* Now build the system root path */
    CmpGetRegistryPath(ConfigPath);
    RtlInitUnicodeString(&TempName, ConfigPath);
    RtlAppendStringToString((PSTRING)&FileName, (PSTRING)&TempName);

    /* And build the registry root path */
    RtlInitUnicodeString(&TempName, L"\\REGISTRY\\");
    RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);
    RegStart = RegName.Length;

    /* Setup the event to synchronize workers */
    KeInitializeEvent(&CmpLoadWorkerEvent, SynchronizationEvent, FALSE);

    /* Enter special boot condition */
    CmpSpecialBootCondition = TRUE;

    /* Create the SD for the root hives */
    SecurityDescriptor = CmpHiveRootSecurityDescriptor();

    /* Loop every hive we care about */
    for (i = 0; i < CM_NUMBER_OF_MACHINE_HIVES; i++)
    {
        /* Make sure the list is setup */
        ASSERT(CmpMachineHiveList[i].Name != NULL);

        /* Create a thread to handle this hive */
        Status = PsCreateSystemThread(&Thread,
                                      THREAD_ALL_ACCESS,
                                      NULL,
                                      0,
                                      NULL,
                                      CmpLoadHiveThread,
                                      UlongToPtr(i));
        if (NT_SUCCESS(Status))
        {
            /* We don't care about the handle -- the thread self-terminates */
            ZwClose(Thread);
        }
        else
        {
            /* Can't imagine this happening */
            KeBugCheckEx(BAD_SYSTEM_CONFIG_INFO, 9, 3, i, Status);
        }
    }

    /* Make sure we've reached the end of the list */
    ASSERT(CmpMachineHiveList[i].Name == NULL);

    /* Wait for hive loading to finish */
    KeWaitForSingleObject(&CmpLoadWorkerEvent,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    /* Exit the special boot condition and make sure all workers completed */
    CmpSpecialBootCondition = FALSE;
    ASSERT(CmpLoadWorkerIncrement == CM_NUMBER_OF_MACHINE_HIVES);

    /* Loop hives again */
    for (i = 0; i < CM_NUMBER_OF_MACHINE_HIVES; i++)
    {
        /* Make sure the thread ran and finished */
        ASSERT(CmpMachineHiveList[i].ThreadFinished == TRUE);
        ASSERT(CmpMachineHiveList[i].ThreadStarted == TRUE);

        /* Check if this was a new hive */
        if (!CmpMachineHiveList[i].CmHive)
        {
            /* Make sure we allocated something */
            ASSERT(CmpMachineHiveList[i].CmHive2 != NULL);

            /* Build the base name */
            RegName.Length = RegStart;
            RtlInitUnicodeString(&TempName, CmpMachineHiveList[i].BaseName);
            RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);

            /* Check if this is a child of the root */
            if (RegName.Buffer[RegName.Length / sizeof(WCHAR) - 1] == '\\')
            {
                /* Then setup the whole name */
                RtlInitUnicodeString(&TempName, CmpMachineHiveList[i].Name);
                RtlAppendStringToString((PSTRING)&RegName, (PSTRING)&TempName);
            }

            /* Now link the hive to its master */
            Status = CmpLinkHiveToMaster(&RegName,
                                         NULL,
                                         CmpMachineHiveList[i].CmHive2,
                                         CmpMachineHiveList[i].Allocate,
                                         SecurityDescriptor);
            if (Status != STATUS_SUCCESS)
            {
                /* Linking needs to work */
                KeBugCheckEx(CONFIG_LIST_FAILED, 11, Status, i, (ULONG_PTR)&RegName);
            }

            /* Check if we had to allocate a new hive */
            if (CmpMachineHiveList[i].Allocate)
            {
                /* Sync the new hive */
                //HvSyncHive((PHHIVE)(CmpMachineHiveList[i].CmHive2));
            }
        }

        /* Check if we created a new hive */
        if (CmpMachineHiveList[i].CmHive2)
        {
            /* Add to HiveList key */
            CmpAddToHiveFileList(CmpMachineHiveList[i].CmHive2);
        }
    }

    /* Get rid of the SD */
    ExFreePoolWithTag(SecurityDescriptor, TAG_CMSD);

    /* Link SECURITY to SAM */
    CmpLinkKeyToHive(L"\\Registry\\Machine\\Security\\SAM",
                     L"\\Registry\\Machine\\SAM\\SAM");

    /* Link S-1-5-18 to .Default */
    CmpNoVolatileCreates = FALSE;
    CmpLinkKeyToHive(L"\\Registry\\User\\S-1-5-18",
                     L"\\Registry\\User\\.Default");
    CmpNoVolatileCreates = TRUE;
}

BOOLEAN
NTAPI
INIT_FUNCTION
CmInitSystem1(VOID)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING KeyName;
    HANDLE KeyHandle;
    NTSTATUS Status;
    PCMHIVE HardwareHive;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PAGED_CODE();

    /* Check if this is PE-boot */
    if (InitIsWinPEMode)
    {
        /* Set registry to PE mode */
        CmpMiniNTBoot = TRUE;
        CmpShareSystemHives = TRUE;
    }

    /* Initialize the hive list and lock */
    InitializeListHead(&CmpHiveListHead);
    ExInitializePushLock(&CmpHiveListHeadLock);
    ExInitializePushLock(&CmpLoadHiveLock);

    /* Initialize registry lock */
    ExInitializeResourceLite(&CmpRegistryLock);

    /* Initialize the cache */
    CmpInitializeCache();

    /* Initialize allocation and delayed dereferencing */
    CmpInitCmPrivateAlloc();
    CmpInitCmPrivateDelayAlloc();
    CmpInitDelayDerefKCBEngine();

    /* Initialize callbacks */
    CmpInitCallback();

    /* Initialize self healing */
    KeInitializeGuardedMutex(&CmpSelfHealQueueLock);
    InitializeListHead(&CmpSelfHealQueueListHead);

    /* Save the current process and lock the registry */
    CmpSystemProcess = PsGetCurrentProcess();

    /* Create the key object types */
    Status = CmpCreateObjectTypes();
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 1, Status, 0);
    }

    /* Build the master hive */
    Status = CmpInitializeHive(&CmiVolatileHive,
                               HINIT_CREATE,
                               HIVE_VOLATILE,
                               HFILE_TYPE_PRIMARY,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               0);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 2, Status, 0);
    }

    /* Create the \REGISTRY key node */
    if (!CmpCreateRegistryRoot())
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 3, 0, 0);
    }

    /* Create the default security descriptor */
    SecurityDescriptor = CmpHiveRootSecurityDescriptor();

    /* Create '\Registry\Machine' key. */
    RtlInitUnicodeString(&KeyName, L"\\REGISTRY\\MACHINE");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               SecurityDescriptor);
    Status = NtCreateKey(&KeyHandle,
                         KEY_READ | KEY_WRITE,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 5, Status, 0);
    }

    /* Close the handle */
    NtClose(KeyHandle);

    /* Create '\Registry\User' key. */
    RtlInitUnicodeString(&KeyName, L"\\REGISTRY\\USER");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               SecurityDescriptor);
    Status = NtCreateKey(&KeyHandle,
                         KEY_READ | KEY_WRITE,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 6, Status, 0);
    }

    /* Close the handle */
    NtClose(KeyHandle);

    /* After this point, do not allow creating keys in the master hive */
    CmpNoVolatileCreates = TRUE;

    /* Initialize the system hive */
    if (!CmpInitializeSystemHive(KeLoaderBlock))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 7, 0, 0);
    }

    /* Create the 'CurrentControlSet' link. */
    Status = CmpCreateControlSet(KeLoaderBlock);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 8, Status, 0);
    }

    /* Create the hardware hive */
    Status = CmpInitializeHive(&HardwareHive,
                               HINIT_CREATE,
                               HIVE_VOLATILE,
                               HFILE_TYPE_PRIMARY,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               0);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 11, Status, 0);
    }

    /* Add the hive to the hive list */
    CmpMachineHiveList[0].CmHive = HardwareHive;

    /* Attach it to the machine key */
    RtlInitUnicodeString(&KeyName, L"\\Registry\\Machine\\HARDWARE");
    Status = CmpLinkHiveToMaster(&KeyName,
                                 NULL,
                                 HardwareHive,
                                 TRUE,
                                 SecurityDescriptor);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 12, Status, 0);
    }

    /* Add to HiveList key */
    CmpAddToHiveFileList(HardwareHive);

    /* Free the security descriptor */
    ExFreePoolWithTag(SecurityDescriptor, TAG_CMSD);

    /* Fill out the Hardware key with the ARC Data from the Loader */
    Status = CmpInitializeHardwareConfiguration(KeLoaderBlock);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 13, Status, 0);
    }

    /* Initialize machine-dependent information into the registry */
    Status = CmpInitializeMachineDependentConfiguration(KeLoaderBlock);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 14, Status, 0);
    }

    /* Initialize volatile registry settings */
    Status = CmpSetSystemValues(KeLoaderBlock);
    if (!NT_SUCCESS(Status))
    {
        /* Bugcheck */
        KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 1, 15, Status, 0);
    }

    /* Free the load options */
    ExFreePoolWithTag(CmpLoadOptions.Buffer, TAG_CM);

    /* If we got here, all went well */
    return TRUE;
}

VOID
NTAPI
INIT_FUNCTION
CmpFreeDriverList(IN PHHIVE Hive,
                  IN PLIST_ENTRY DriverList)
{
    PLIST_ENTRY NextEntry, OldEntry;
    PBOOT_DRIVER_NODE DriverNode;
    PAGED_CODE();

    /* Parse the current list */
    NextEntry = DriverList->Flink;
    while (NextEntry != DriverList)
    {
        /* Get the driver node */
        DriverNode = CONTAINING_RECORD(NextEntry, BOOT_DRIVER_NODE, ListEntry.Link);

        /* Get the next entry now, since we're going to free it later */
        OldEntry = NextEntry;
        NextEntry = NextEntry->Flink;

        /* Was there a name? */
        if (DriverNode->Name.Buffer)
        {
            /* Free it */
            CmpFree(DriverNode->Name.Buffer, DriverNode->Name.Length);
        }

        /* Was there a registry path? */
        if (DriverNode->ListEntry.RegistryPath.Buffer)
        {
            /* Free it */
            CmpFree(DriverNode->ListEntry.RegistryPath.Buffer,
                    DriverNode->ListEntry.RegistryPath.MaximumLength);
        }

        /* Was there a file path? */
        if (DriverNode->ListEntry.FilePath.Buffer)
        {
            /* Free it */
            CmpFree(DriverNode->ListEntry.FilePath.Buffer,
                    DriverNode->ListEntry.FilePath.MaximumLength);
        }

        /* Now free the node, and move on */
        CmpFree(OldEntry, sizeof(BOOT_DRIVER_NODE));
    }
}

PUNICODE_STRING*
NTAPI
INIT_FUNCTION
CmGetSystemDriverList(VOID)
{
    LIST_ENTRY DriverList;
    OBJECT_ATTRIBUTES ObjectAttributes;
    NTSTATUS Status;
    PCM_KEY_BODY KeyBody;
    PHHIVE Hive;
    HCELL_INDEX RootCell, ControlCell;
    HANDLE KeyHandle;
    UNICODE_STRING KeyName;
    PLIST_ENTRY NextEntry;
    ULONG i;
    PUNICODE_STRING* ServicePath = NULL;
    BOOLEAN Success, AutoSelect;
    PBOOT_DRIVER_LIST_ENTRY DriverEntry;
    PAGED_CODE();

    /* Initialize the driver list */
    InitializeListHead(&DriverList);

    /* Open the system hive key */
    RtlInitUnicodeString(&KeyName, L"\\Registry\\Machine\\System");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(&KeyHandle, KEY_READ, &ObjectAttributes);
    if (!NT_SUCCESS(Status)) return NULL;

    /* Reference the key object to get the root hive/cell to access directly */
    Status = ObReferenceObjectByHandle(KeyHandle,
                                       KEY_QUERY_VALUE,
                                       CmpKeyObjectType,
                                       KernelMode,
                                       (PVOID*)&KeyBody,
                                       NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Fail */
        NtClose(KeyHandle);
        return NULL;
    }

    /* Do all this under the registry lock */
    CmpLockRegistryExclusive();

    /* Get the hive and key cell */
    Hive = KeyBody->KeyControlBlock->KeyHive;
    RootCell = KeyBody->KeyControlBlock->KeyCell;

    /* Open the current control set key */
    RtlInitUnicodeString(&KeyName, L"Current");
    ControlCell = CmpFindControlSet(Hive, RootCell, &KeyName, &AutoSelect);
    if (ControlCell == HCELL_NIL) goto EndPath;

    /* Find all system drivers */
    Success = CmpFindDrivers(Hive, ControlCell, SystemLoad, NULL, &DriverList);
    if (!Success) goto EndPath;

    /* Sort by group/tag */
    if (!CmpSortDriverList(Hive, ControlCell, &DriverList)) goto EndPath;

    /* Remove circular dependencies (cycles) and sort */
    if (!CmpResolveDriverDependencies(&DriverList)) goto EndPath;

    /* Loop the list to count drivers */
    for (i = 0, NextEntry = DriverList.Flink;
         NextEntry != &DriverList;
         i++, NextEntry = NextEntry->Flink);

    /* Allocate the array */
    ServicePath = ExAllocatePool(NonPagedPool, (i + 1) * sizeof(PUNICODE_STRING));
    if (!ServicePath) KeBugCheckEx(CONFIG_INITIALIZATION_FAILED, 2, 1, 0, 0);

    /* Loop the driver list */
    for (i = 0, NextEntry = DriverList.Flink;
         NextEntry != &DriverList;
         i++, NextEntry = NextEntry->Flink)
    {
        /* Get the entry */
        DriverEntry = CONTAINING_RECORD(NextEntry, BOOT_DRIVER_LIST_ENTRY, Link);

        /* Allocate the path for the caller and duplicate the registry path */
        ServicePath[i] = ExAllocatePool(NonPagedPool, sizeof(UNICODE_STRING));
        RtlDuplicateUnicodeString(RTL_DUPLICATE_UNICODE_STRING_NULL_TERMINATE,
                                  &DriverEntry->RegistryPath,
                                  ServicePath[i]);
    }

    /* Terminate the list */
    ServicePath[i] = NULL;

EndPath:
    /* Free the driver list if we had one */
    if (!IsListEmpty(&DriverList)) CmpFreeDriverList(Hive, &DriverList);

    /* Unlock the registry */
    CmpUnlockRegistry();

    /* Close the key handle and dereference the object, then return the path */
    ObDereferenceObject(KeyBody);
    NtClose(KeyHandle);
    return ServicePath;
}

VOID
NTAPI
CmpLockRegistryExclusive(VOID)
{
    /* Enter a critical region and lock the registry */
    KeEnterCriticalRegion();
    ExAcquireResourceExclusiveLite(&CmpRegistryLock, TRUE);

    /* Sanity check */
    ASSERT(CmpFlushStarveWriters == 0);
    RtlGetCallersAddress(&CmpRegistryLockCaller, &CmpRegistryLockCallerCaller);
}

VOID
NTAPI
CmpLockRegistry(VOID)
{
    /* Enter a critical region */
    KeEnterCriticalRegion();

    /* Check if we have to starve writers */
    if (CmpFlushStarveWriters)
    {
        /* Starve exlusive waiters */
        ExAcquireSharedStarveExclusive(&CmpRegistryLock, TRUE);
    }
    else
    {
        /* Just grab the lock */
        ExAcquireResourceSharedLite(&CmpRegistryLock, TRUE);
    }
}

BOOLEAN
NTAPI
CmpTestRegistryLock(VOID)
{
    /* Test the lock */
    return !ExIsResourceAcquiredSharedLite(&CmpRegistryLock) ? FALSE : TRUE;
}

BOOLEAN
NTAPI
CmpTestRegistryLockExclusive(VOID)
{
    /* Test the lock */
    return !ExIsResourceAcquiredExclusiveLite(&CmpRegistryLock) ? FALSE : TRUE;
}

VOID
NTAPI
CmpLockHiveFlusherExclusive(IN PCMHIVE Hive)
{
    /* Lock the flusher. We should already be in a critical section */
    CMP_ASSERT_REGISTRY_LOCK_OR_LOADING(Hive);
    ASSERT((ExIsResourceAcquiredShared(Hive->FlusherLock) == 0) &&
           (ExIsResourceAcquiredExclusiveLite(Hive->FlusherLock) == 0));
    ExAcquireResourceExclusiveLite(Hive->FlusherLock, TRUE);
}

VOID
NTAPI
CmpLockHiveFlusherShared(IN PCMHIVE Hive)
{
    /* Lock the flusher. We should already be in a critical section */
    CMP_ASSERT_REGISTRY_LOCK_OR_LOADING(Hive);
    ASSERT((ExIsResourceAcquiredShared(Hive->FlusherLock) == 0) &&
           (ExIsResourceAcquiredExclusiveLite(Hive->FlusherLock) == 0));
    ExAcquireResourceSharedLite(Hive->FlusherLock, TRUE);
}

VOID
NTAPI
CmpUnlockHiveFlusher(IN PCMHIVE Hive)
{
    /* Sanity check */
    CMP_ASSERT_REGISTRY_LOCK_OR_LOADING(Hive);
    CMP_ASSERT_FLUSH_LOCK(Hive);

    /* Release the lock */
    ExReleaseResourceLite(Hive->FlusherLock);
}

BOOLEAN
NTAPI
CmpTestHiveFlusherLockShared(IN PCMHIVE Hive)
{
    /* Test the lock */
    return !ExIsResourceAcquiredSharedLite(Hive->FlusherLock) ? FALSE : TRUE;
}

BOOLEAN
NTAPI
CmpTestHiveFlusherLockExclusive(IN PCMHIVE Hive)
{
    /* Test the lock */
    return !ExIsResourceAcquiredExclusiveLite(Hive->FlusherLock) ? FALSE : TRUE;
}

VOID
NTAPI
CmpUnlockRegistry(VOID)
{
    /* Sanity check */
    CMP_ASSERT_REGISTRY_LOCK();

    /* Check if we should flush the registry */
    if (CmpFlushOnLockRelease)
    {
        /* The registry should be exclusively locked for this */
        CMP_ASSERT_EXCLUSIVE_REGISTRY_LOCK();

        /* Flush the registry */
        CmpDoFlushAll(TRUE);
        CmpFlushOnLockRelease = FALSE;
    }
    else
    {
        /* Lazy flush the registry */
        CmpLazyFlush();
    }

    /* Release the lock and leave the critical region */
    ExReleaseResourceLite(&CmpRegistryLock);
    KeLeaveCriticalRegion();
}

VOID
NTAPI
CmpAcquireTwoKcbLocksExclusiveByKey(IN ULONG ConvKey1,
                                    IN ULONG ConvKey2)
{
    ULONG Index1, Index2;

    /* Sanity check */
    CMP_ASSERT_REGISTRY_LOCK();

    /* Get hash indexes */
    Index1 = GET_HASH_INDEX(ConvKey1);
    Index2 = GET_HASH_INDEX(ConvKey2);

    /* See which one is highest */
    if (Index1 < Index2)
    {
        /* Grab them in the proper order */
        CmpAcquireKcbLockExclusiveByKey(ConvKey1);
        CmpAcquireKcbLockExclusiveByKey(ConvKey2);
    }
    else
    {
        /* Grab the second one first, then the first */
        CmpAcquireKcbLockExclusiveByKey(ConvKey2);
        if (Index1 != Index2) CmpAcquireKcbLockExclusiveByKey(ConvKey1);
    }
}

VOID
NTAPI
CmpReleaseTwoKcbLockByKey(IN ULONG ConvKey1,
                          IN ULONG ConvKey2)
{
    ULONG Index1, Index2;

    /* Sanity check */
    CMP_ASSERT_REGISTRY_LOCK();

    /* Get hash indexes */
    Index1 = GET_HASH_INDEX(ConvKey1);
    Index2 = GET_HASH_INDEX(ConvKey2);
    ASSERT((GET_HASH_ENTRY(CmpCacheTable, ConvKey2).Owner == KeGetCurrentThread()) ||
           (CmpTestRegistryLockExclusive()));

    /* See which one is highest */
    if (Index1 < Index2)
    {
        /* Grab them in the proper order */
        ASSERT((GET_HASH_ENTRY(CmpCacheTable, ConvKey1).Owner == KeGetCurrentThread()) ||
               (CmpTestRegistryLockExclusive()));
        CmpReleaseKcbLockByKey(ConvKey2);
        CmpReleaseKcbLockByKey(ConvKey1);
    }
    else
    {
        /* Release the first one first, then the second */
        if (Index1 != Index2)
        {
            ASSERT((GET_HASH_ENTRY(CmpCacheTable, ConvKey1).Owner == KeGetCurrentThread()) ||
                   (CmpTestRegistryLockExclusive()));
            CmpReleaseKcbLockByKey(ConvKey1);
        }
        CmpReleaseKcbLockByKey(ConvKey2);
    }
}

VOID
NTAPI
CmShutdownSystem(VOID)
{
    PLIST_ENTRY ListEntry;
    PCMHIVE Hive;

    /* Kill the workers */
    if (!CmFirstTime) CmpShutdownWorkers();

    /* Flush all hives */
    CmpLockRegistryExclusive();
    CmpDoFlushAll(TRUE);

    /* Close all hive files */
    ListEntry = CmpHiveListHead.Flink;
    while (ListEntry != &CmpHiveListHead)
    {
        Hive = CONTAINING_RECORD(ListEntry, CMHIVE, HiveList);

        CmpCloseHiveFiles(Hive);

        ListEntry = ListEntry->Flink;
    }

    CmpUnlockRegistry();
}

VOID
NTAPI
CmpSetVersionData(VOID)
{
    NTSTATUS Status;
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING KeyName;
    UNICODE_STRING ValueName;
    UNICODE_STRING ValueData;
    ANSI_STRING TempString;
    HANDLE SoftwareKeyHandle = NULL;
    HANDLE MicrosoftKeyHandle = NULL;
    HANDLE WindowsNtKeyHandle = NULL;
    HANDLE CurrentVersionKeyHandle = NULL;
    WCHAR Buffer[128]; // Buffer large enough to contain a full ULONG in decimal representation,
                       // and the full 'CurrentType' string.

    /*
     * Open the 'HKLM\Software\Microsoft\Windows NT\CurrentVersion' key
     * (create the intermediate subkeys if needed).
     */

    RtlInitUnicodeString(&KeyName, L"\\REGISTRY\\MACHINE\\SOFTWARE");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               NULL,
                               NULL);
    Status = NtCreateKey(&SoftwareKeyHandle,
                         KEY_CREATE_SUB_KEY,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create key %wZ (Status: %08lx)\n", &KeyName, Status);
        return;
    }

    RtlInitUnicodeString(&KeyName, L"Microsoft");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               SoftwareKeyHandle,
                               NULL);
    Status = NtCreateKey(&MicrosoftKeyHandle,
                         KEY_CREATE_SUB_KEY,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create key %wZ (Status: %08lx)\n", &KeyName, Status);
        goto Quit;
    }

    RtlInitUnicodeString(&KeyName, L"Windows NT");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               MicrosoftKeyHandle,
                               NULL);
    Status = NtCreateKey(&WindowsNtKeyHandle,
                         KEY_CREATE_SUB_KEY,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create key %wZ (Status: %08lx)\n", &KeyName, Status);
        goto Quit;
    }

    RtlInitUnicodeString(&KeyName, L"CurrentVersion");
    InitializeObjectAttributes(&ObjectAttributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               WindowsNtKeyHandle,
                               NULL);
    Status = NtCreateKey(&CurrentVersionKeyHandle,
                         KEY_CREATE_SUB_KEY | KEY_SET_VALUE,
                         &ObjectAttributes,
                         0,
                         NULL,
                         0,
                         NULL);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create key %wZ (Status: %08lx)\n", &KeyName, Status);
        goto Quit;
    }

    /* Set the 'CurrentVersion' value */
    RtlInitUnicodeString(&ValueName, L"CurrentVersion");
    NtSetValueKey(CurrentVersionKeyHandle,
                  &ValueName,
                  0,
                  REG_SZ,
                  CmVersionString.Buffer,
                  CmVersionString.Length + sizeof(WCHAR));

    /* Set the 'CurrentBuildNumber' value */
    RtlInitUnicodeString(&ValueName, L"CurrentBuildNumber");
    RtlInitEmptyUnicodeString(&ValueData, Buffer, sizeof(Buffer));
    RtlIntegerToUnicodeString(NtBuildNumber & 0xFFFF, 10, &ValueData);
    NtSetValueKey(CurrentVersionKeyHandle,
                  &ValueName,
                  0,
                  REG_SZ,
                  ValueData.Buffer,
                  ValueData.Length + sizeof(WCHAR));

    /* Set the 'BuildLab' value */
    RtlInitUnicodeString(&ValueName, L"BuildLab");
    RtlInitAnsiString(&TempString, NtBuildLab);
    Status = RtlAnsiStringToUnicodeString(&ValueData, &TempString, FALSE);
    if (NT_SUCCESS(Status))
    {
        NtSetValueKey(CurrentVersionKeyHandle,
                      &ValueName,
                      0,
                      REG_SZ,
                      ValueData.Buffer,
                      ValueData.Length + sizeof(WCHAR));
    }

    /* Set the 'CurrentType' value */
    RtlInitUnicodeString(&ValueName, L"CurrentType");

    swprintf(Buffer, L"%s %s",
#ifdef CONFIG_SMP
             L"Multiprocessor"
#else
             L"Uniprocessor"
#endif
             ,
#if (DBG == 1)
             L"Checked"
#else
             L"Free"
#endif
             );
    RtlInitUnicodeString(&ValueData, Buffer);
    NtSetValueKey(CurrentVersionKeyHandle,
                  &ValueName,
                  0,
                  REG_SZ,
                  ValueData.Buffer,
                  ValueData.Length + sizeof(WCHAR));

    /* Set the 'CSDVersion' value */
    RtlInitUnicodeString(&ValueName, L"CSDVersion");
    if (CmCSDVersionString.Length != 0)
    {
        NtSetValueKey(CurrentVersionKeyHandle,
                      &ValueName,
                      0,
                      REG_SZ,
                      CmCSDVersionString.Buffer,
                      CmCSDVersionString.Length + sizeof(WCHAR));
    }
    else
    {
        NtDeleteValueKey(CurrentVersionKeyHandle, &ValueName);
    }

    /* Set the 'CSDBuildNumber' value */
    RtlInitUnicodeString(&ValueName, L"CSDBuildNumber");
    if (CmNtSpBuildNumber != 0)
    {
        RtlInitEmptyUnicodeString(&ValueData, Buffer, sizeof(Buffer));
        RtlIntegerToUnicodeString(CmNtSpBuildNumber, 10, &ValueData);
        NtSetValueKey(CurrentVersionKeyHandle,
                      &ValueName,
                      0,
                      REG_SZ,
                      ValueData.Buffer,
                      ValueData.Length + sizeof(WCHAR));
    }
    else
    {
        NtDeleteValueKey(CurrentVersionKeyHandle, &ValueName);
    }

    /* Set the 'SystemRoot' value */
    RtlInitUnicodeString(&ValueName, L"SystemRoot");
    NtSetValueKey(CurrentVersionKeyHandle,
                  &ValueName,
                  0,
                  REG_SZ,
                  NtSystemRoot.Buffer,
                  NtSystemRoot.Length + sizeof(WCHAR));

Quit:
    /* Close the keys */
    if (CurrentVersionKeyHandle != NULL)
        NtClose(CurrentVersionKeyHandle);

    if (WindowsNtKeyHandle != NULL)
        NtClose(WindowsNtKeyHandle);

    if (MicrosoftKeyHandle != NULL)
        NtClose(MicrosoftKeyHandle);

    if (SoftwareKeyHandle != NULL)
        NtClose(SoftwareKeyHandle);
}

/* EOF */
