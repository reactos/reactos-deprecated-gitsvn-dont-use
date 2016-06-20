/*
 * PROJECT:        ReactOS Kernel
 * LICENSE:        GNU GPLv2 only as published by the Free Software Foundation
 * PURPOSE:        To Implement AHCI Miniport driver targeting storport NT 5.2
 * PROGRAMMERS:    Aman Priyadarshi (aman.eureka@gmail.com)
 */

#include "storahci.h"

/**
 * @name AhciPortInitialize
 * @implemented
 *
 * Initialize port by setting up PxCLB & PxFB Registers
 *
 * @param PortExtension
 *
 * @return
 * Return true if intialization was successful
 */
BOOLEAN
AhciPortInitialize (
    __in PAHCI_PORT_EXTENSION PortExtension
    )
{
    ULONG mappedLength, portNumber;
    PAHCI_MEMORY_REGISTERS abar;
    PAHCI_ADAPTER_EXTENSION adapterExtension;
    STOR_PHYSICAL_ADDRESS commandListPhysical, receivedFISPhysical;

    DebugPrint("AhciPortInitialize()\n");

    adapterExtension = PortExtension->AdapterExtension;
    abar = adapterExtension->ABAR_Address;
    portNumber = PortExtension->PortNumber;

    NT_ASSERT(abar != NULL);
    NT_ASSERT(portNumber < adapterExtension->PortCount);

    PortExtension->Port = &abar->PortList[portNumber];

    commandListPhysical = StorPortGetPhysicalAddress(adapterExtension,
                                                     NULL,
                                                     PortExtension->CommandList,
                                                     &mappedLength);

    if ((mappedLength == 0) || ((commandListPhysical.LowPart % 1024) != 0))
    {
        DebugPrint("\tcommandListPhysical mappedLength:%d\n", mappedLength);
        return FALSE;
    }

    receivedFISPhysical = StorPortGetPhysicalAddress(adapterExtension,
                                                     NULL,
                                                     PortExtension->ReceivedFIS,
                                                     &mappedLength);

    if ((mappedLength == 0) || ((receivedFISPhysical.LowPart % 256) != 0))
    {
        DebugPrint("\treceivedFISPhysical mappedLength:%d\n", mappedLength);
        return FALSE;
    }

    // 10.1.2 For each implemented port, system software shall allocate memory for and program:
    //  PxCLB and PxCLBU (if CAP.S64A is set to ‘1’)
    //  PxFB and PxFBU (if CAP.S64A is set to ‘1’)
    // Note: Assuming 32bit support only
    StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->CLB, commandListPhysical.LowPart);
    if (IsAdapterCAPS64(adapterExtension->CAP))
    {
        StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->CLBU, commandListPhysical.HighPart);
    }

    StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->FB, receivedFISPhysical.LowPart);
    if (IsAdapterCAPS64(adapterExtension->CAP))
    {
        StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->FBU, receivedFISPhysical.HighPart);
    }

    // set device power state flag to D0
    PortExtension->DevicePowerState = StorPowerDeviceD0;

    // clear pending interrupts
    StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->SERR, (ULONG)-1);
    StorPortWriteRegisterUlong(adapterExtension, &PortExtension->Port->IS, (ULONG)-1);
    StorPortWriteRegisterUlong(adapterExtension, PortExtension->AdapterExtension->IS, (1 << PortExtension->PortNumber));

    return TRUE;
}// -- AhciPortInitialize();

/**
 * @name AhciAllocateResourceForAdapter
 * @implemented
 *
 * Allocate memory from poll for required pointers
 *
 * @param AdapterExtension
 * @param ConfigInfo
 *
 * @return
 * return TRUE if allocation was successful
 */
BOOLEAN
AhciAllocateResourceForAdapter (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PPORT_CONFIGURATION_INFORMATION ConfigInfo
    )
{
    PVOID portsExtension = NULL;
    PCHAR nonCachedExtension;
    ULONG status, index, NCS, AlignedNCS;
    ULONG portCount, portImplemented, nonCachedExtensionSize;

    DebugPrint("AhciAllocateResourceForAdapter()\n");

    NCS = AHCI_Global_Port_CAP_NCS(AdapterExtension->CAP);
    AlignedNCS = ROUND_UP(NCS, 8);

    // get port count -- Number of set bits in `AdapterExtension->PortImplemented`
    portCount = 0;
    portImplemented = AdapterExtension->PortImplemented;

    NT_ASSERT(portImplemented != 0);
    for (index = MAXIMUM_AHCI_PORT_COUNT - 1; index > 0; index--)
        if ((portImplemented & (1 << index)) != 0)
            break;

    portCount = index + 1;
    DebugPrint("\tPort Count: %d\n", portCount);

    AdapterExtension->PortCount = portCount;
    nonCachedExtensionSize =    sizeof(AHCI_COMMAND_HEADER) * AlignedNCS + //should be 1K aligned
                                sizeof(AHCI_RECEIVED_FIS);

    // align nonCachedExtensionSize to 1024
    nonCachedExtensionSize = ROUND_UP(nonCachedExtensionSize, 1024);

    AdapterExtension->NonCachedExtension = StorPortGetUncachedExtension(AdapterExtension,
                                                                        ConfigInfo,
                                                                        nonCachedExtensionSize * portCount);

    if (AdapterExtension->NonCachedExtension == NULL)
    {
        DebugPrint("\tadapterExtension->NonCachedExtension == NULL\n");
        return FALSE;
    }

    nonCachedExtension = AdapterExtension->NonCachedExtension;
    AhciZeroMemory(nonCachedExtension, nonCachedExtensionSize * portCount);

    for (index = 0; index < portCount; index++)
    {
        AdapterExtension->PortExtension[index].IsActive = FALSE;
        if ((AdapterExtension->PortImplemented & (1 << index)) != 0)
        {
            AdapterExtension->PortExtension[index].PortNumber = index;
            AdapterExtension->PortExtension[index].IsActive = TRUE;
            AdapterExtension->PortExtension[index].AdapterExtension = AdapterExtension;
            AdapterExtension->PortExtension[index].CommandList = nonCachedExtension;
            AdapterExtension->PortExtension[index].ReceivedFIS = (PAHCI_RECEIVED_FIS)(nonCachedExtension + sizeof(AHCI_COMMAND_HEADER) * AlignedNCS);
            nonCachedExtension += nonCachedExtensionSize;
        }
    }

    return TRUE;
}// -- AhciAllocateResourceForAdapter();

/**
 * @name AhciHwInitialize
 * @implemented
 *
 * initializes the HBA and finds all devices that are of interest to the miniport driver.
 *
 * @param adapterExtension
 *
 * @return
 * return TRUE if intialization was successful
 */
BOOLEAN
AhciHwInitialize (
    __in PVOID AdapterExtension
    )
{
    ULONG ghc, messageCount, status;
    PAHCI_ADAPTER_EXTENSION adapterExtension;

    DebugPrint("AhciHwInitialize()\n");

    adapterExtension = AdapterExtension;
    adapterExtension->StateFlags.MessagePerPort = FALSE;

    // First check what type of interrupt/synchronization device is using
    ghc = StorPortReadRegisterUlong(adapterExtension, &adapterExtension->ABAR_Address->GHC);

    // When set to ‘1’ by hardware, indicates that the HBA requested more than one MSI vector
    // but has reverted to using the first vector only.  When this bit is cleared to ‘0’,
    // the HBA has not reverted to single MSI mode (i.e. hardware is already in single MSI mode,
    // software has allocated the number of messages requested
    if ((ghc & AHCI_Global_HBA_CONTROL_MRSM) == 0)
    {
        adapterExtension->StateFlags.MessagePerPort = TRUE;
        DebugPrint("\tMultiple MSI based message not supported\n");
    }

    return TRUE;
}// -- AhciHwInitialize();

/**
 * @name AhciInterruptHandler
 * @not_implemented
 *
 * Interrupt Handler for PortExtension
 *
 * @param PortExtension
 *
 */
VOID
AhciInterruptHandler (
    __in PAHCI_PORT_EXTENSION PortExtension
    )
{
    ULONG IS;
    AHCI_INTERRUPT_STATUS PxIS;
    AHCI_INTERRUPT_STATUS PxISMasked;
    PAHCI_ADAPTER_EXTENSION AdapterExtension;

    DebugPrint("AhciInterruptHandler()\n");
    DebugPrint("\tPort Number: %d\n", PortExtension->PortNumber);

    AdapterExtension = PortExtension->AdapterExtension;
    NT_ASSERT(IsPortValid(AdapterExtension, PortExtension->PortNumber));

    // 5.5.3
    // 1. Software determines the cause of the interrupt by reading the PxIS register.
    //    It is possible for multiple bits to be set
    // 2. Software clears appropriate bits in the PxIS register corresponding to the cause of the interrupt.
    // 3. Software clears the interrupt bit in IS.IPS corresponding to the port.
    // 4. If executing non-queued commands, software reads the PxCI register, and compares the current value to
    //    the list of commands previously issued by software that are still outstanding.
    //    If executing native queued commands, software reads the PxSACT register and compares the current
    //    value to the list of commands previously issued by software.
    //    Software completes with success any outstanding command whose corresponding bit has been cleared in
    //    the respective register. PxCI and PxSACT are volatile registers; software should only use their values
    //    to determine commands that have completed, not to determine which commands have previously been issued.
    // 5. If there were errors, noted in the PxIS register, software performs error recovery actions (see section 6.2.2).
    PxISMasked.Status = 0;
    PxIS.Status = StorPortReadRegisterUlong(AdapterExtension, &PortExtension->Port->IS);

    // 6.2.2
    // Fatal Error
    // signified by the setting of PxIS.HBFS, PxIS.HBDS, PxIS.IFS, or PxIS.TFES
    if (PxIS.HBFS || PxIS.HBDS || PxIS.IFS || PxIS.TFES)
    {
        // In this state, the HBA shall not issue any new commands nor acknowledge DMA Setup FISes to process
        // any native command queuing commands. To recover, the port must be restarted
        // To detect an error that requires software recovery actions to be performed,
        // software should check whether any of the following status bits are set on an interrupt:
        // PxIS.HBFS, PxIS.HBDS, PxIS.IFS, and PxIS.TFES.  If any of these bits are set,
        // software should perform the appropriate error recovery actions based on whether
        // non-queued commands were being issued or native command queuing commands were being issued.

        DebugPrint("\tFata Error: %x\n", PxIS.Status);
    }

    // Normal Command Completion
    // 3.3.5
    // A D2H Register FIS has been received with the ‘I’ bit set, and has been copied into system memory.
    PxISMasked.DHRS = PxIS.DHRS;
    // A PIO Setup FIS has been received with the ‘I’ bit set, it has been copied into system memory.
    PxISMasked.PSS = PxIS.PSS;
    // A DMA Setup FIS has been received with the ‘I’ bit set and has been copied into system memory.
    PxISMasked.DSS = PxIS.DSS;
    // A Set Device Bits FIS has been received with the ‘I’ bit set and has been copied into system memory/
    PxISMasked.SDBS = PxIS.SDBS;
    // A PRD with the ‘I’ bit set has transferred all of its data.
    PxISMasked.DPS = PxIS.DPS;

    if (PxISMasked.Status != 0)
    {
        StorPortWriteRegisterUlong(AdapterExtension, &PortExtension->Port->IS, PxISMasked.Status);
    }

    // 10.7.1.1
    // Clear port interrupt
    // It is set by the level of the virtual interrupt line being a set, and cleared by a write of ‘1’ from the software.
    IS = (1 << PortExtension->PortNumber);
    StorPortWriteRegisterUlong(AdapterExtension, AdapterExtension->IS, IS);

    return;
}// -- AhciInterruptHandler();

/**
 * @name AhciHwInterrupt
 * @implemented
 *
 * The Storport driver calls the HwStorInterrupt routine after the HBA generates an interrupt request.
 *
 * @param AdapterExtension
 *
 * @return
 * return TRUE Indicates that an interrupt was pending on adapter.
 * return FALSE Indicates the interrupt was not ours.
 */
BOOLEAN
AhciHwInterrupt(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    ULONG portPending, nextPort, i, portCount;

    DebugPrint("AhciHwInterrupt()\n");

    if (AdapterExtension->StateFlags.Removed)
    {
        return FALSE;
    }

    portPending = StorPortReadRegisterUlong(AdapterExtension, AdapterExtension->IS);
    // we process interrupt for implemented ports only
    portCount = AdapterExtension->PortCount;
    portPending = portPending & AdapterExtension->PortImplemented;

    if (portPending == 0)
    {
        return FALSE;
    }

    for (i = 1; i <= portCount; i++)
    {
        nextPort = (AdapterExtension->LastInterruptPort + i) % portCount;

        if ((portPending & (0x1 << nextPort)) == 0)
            continue;

        NT_ASSERT(IsPortValid(AdapterExtension, nextPort));

        if ((nextPort == AdapterExtension->LastInterruptPort) ||
            (AdapterExtension->PortExtension[nextPort].IsActive == FALSE))
        {
            return FALSE;
        }

        // we can assign this interrupt to this port
        AdapterExtension->LastInterruptPort = nextPort;
        AhciInterruptHandler(&AdapterExtension->PortExtension[nextPort]);

        // interrupt belongs to this device
        // should always return TRUE
        return TRUE;
    }

    DebugPrint("\tSomething went wrong");
    return FALSE;
}// -- AhciHwInterrupt();

/**
 * @name AhciHwStartIo
 * @not_implemented
 *
 * The Storport driver calls the HwStorStartIo routine one time for each incoming I/O request.
 *
 * @param adapterExtension
 * @param Srb
 *
 * @return
 * return TRUE if the request was accepted
 * return FALSE if the request must be submitted later
 */
BOOLEAN
AhciHwStartIo (
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    UCHAR function, pathId;
    PAHCI_ADAPTER_EXTENSION adapterExtension;

    DebugPrint("AhciHwStartIo()\n");

    pathId = Srb->PathId;
    function = Srb->Function;
    adapterExtension = AdapterExtension;

    if (!IsPortValid(adapterExtension, pathId))
    {
        Srb->SrbStatus = SRB_STATUS_NO_DEVICE;
        StorPortNotification(RequestComplete, adapterExtension, Srb);
        return TRUE;
    }

    // https://msdn.microsoft.com/windows/hardware/drivers/storage/handling-srb-function-pnp
    // If the function member of an SRB is set to SRB_FUNCTION_PNP,
    // the SRB is a structure of type SCSI_PNP_REQUEST_BLOCK.
    if (function == SRB_FUNCTION_PNP)
    {
        PSCSI_PNP_REQUEST_BLOCK pnpRequest;

        pnpRequest = (PSCSI_PNP_REQUEST_BLOCK)Srb;
        if ((pnpRequest->SrbPnPFlags & SRB_PNP_FLAGS_ADAPTER_REQUEST) != 0)
        {
            if ((pnpRequest->PnPAction == StorRemoveDevice) ||
                (pnpRequest->PnPAction == StorSurpriseRemoval))
            {
                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                adapterExtension->StateFlags.Removed = 1;
                DebugPrint("\tAdapter removed\n");
            }
            else if (pnpRequest->PnPAction == StorStopDevice)
            {
                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                DebugPrint("\tRequested to Stop the adapter\n");
            }
            else
            {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            }

            StorPortNotification(RequestComplete, adapterExtension, Srb);
            return TRUE;
        }
    }

    if (function == SRB_FUNCTION_EXECUTE_SCSI)
    {
        // https://msdn.microsoft.com/en-us/windows/hardware/drivers/storage/handling-srb-function-execute-scsi
        // On receipt of an SRB_FUNCTION_EXECUTE_SCSI request, a miniport driver's HwScsiStartIo
        // routine does the following:
        //
        // - Gets and/or sets up whatever context the miniport driver maintains in its device,
        //   logical unit, and/or SRB extensions
        //   For example, a miniport driver might set up a logical unit extension with pointers
        //   to the SRB itself and the SRB DataBuffer pointer, the SRB DataTransferLength value,
        //   and a driver-defined value (or CDB SCSIOP_XXX value) indicating the operation to be
        //   carried out on the HBA.
        //
        // - Calls an internal routine to program the HBA, as partially directed by the SrbFlags,
        //   for the requested operation
        //   For a device I/O operation, such an internal routine generally selects the target device
        //   and sends the CDB over the bus to the target logical unit.
        if (Srb->CdbLength > 0)
        {
            PCDB cdb = (PCDB)&Srb->Cdb;
            if (cdb->CDB10.OperationCode == SCSIOP_INQUIRY)
            {
                Srb->SrbStatus = DeviceInquiryRequest(adapterExtension, Srb, cdb);
            }
            else
            {
                Srb->SrbStatus = SRB_STATUS_NO_DEVICE;
            }
        }
        else
        {
            Srb->SrbStatus = SRB_STATUS_BAD_FUNCTION;
        }

        StorPortNotification(RequestComplete, adapterExtension, Srb);
        return TRUE;
    }

    DebugPrint("\tUnknown function code recieved: %x\n", function);
    Srb->SrbStatus = SRB_STATUS_BAD_FUNCTION;
    StorPortNotification(RequestComplete, adapterExtension, Srb);
    return TRUE;
}// -- AhciHwStartIo();

/**
 * @name AhciHwResetBus
 * @not_implemented
 *
 * The HwStorResetBus routine is called by the port driver to clear error conditions.
 *
 * @param adapterExtension
 * @param PathId
 *
 * @return
 * return TRUE if bus was successfully reset
 */
BOOLEAN
AhciHwResetBus (
    __in PVOID AdapterExtension,
    __in ULONG PathId
    )
{
    STOR_LOCK_HANDLE lockhandle;
    PAHCI_ADAPTER_EXTENSION adapterExtension;

    DebugPrint("AhciHwResetBus()\n");

    adapterExtension = AdapterExtension;

    if (IsPortValid(AdapterExtension, PathId))
    {
        AhciZeroMemory(&lockhandle, sizeof(lockhandle));

        // Acquire Lock
        StorPortAcquireSpinLock(AdapterExtension, InterruptLock, NULL, &lockhandle);

        // TODO: Perform port reset

        // Release lock
        StorPortReleaseSpinLock(AdapterExtension, &lockhandle);
    }

    return FALSE;
}// -- AhciHwResetBus();

/**
 * @name AhciHwFindAdapter
 * @implemented
 *
 * The HwStorFindAdapter routine uses the supplied configuration to determine whether a specific
 * HBA is supported and, if it is, to return configuration information about that adapter.
 *
 *  10.1 Platform Communication
 *  http://www.intel.in/content/dam/www/public/us/en/documents/technical-specifications/serial-ata-ahci-spec-rev1_2.pdf

 * @param DeviceExtension
 * @param HwContext
 * @param BusInformation
 * @param ArgumentString
 * @param ConfigInfo
 * @param Reserved3
 *
 * @return
 *      SP_RETURN_FOUND
 *          Indicates that a supported HBA was found and that the HBA-relevant configuration information was successfully determined and set in the PORT_CONFIGURATION_INFORMATION structure.
 *
 *      SP_RETURN_ERROR
 *          Indicates that an HBA was found but there was an error obtaining the configuration information. If possible, such an error should be logged with StorPortLogError.
 *
 *      SP_RETURN_BAD_CONFIG
 *          Indicates that the supplied configuration information was invalid for the adapter.
 *
 *      SP_RETURN_NOT_FOUND
 *          Indicates that no supported HBA was found for the supplied configuration information.
 *
 * @remarks Called by Storport.
 */
ULONG
AhciHwFindAdapter (
    __in PVOID AdapterExtension,
    __in PVOID HwContext,
    __in PVOID BusInformation,
    __in PVOID ArgumentString,
    __inout PPORT_CONFIGURATION_INFORMATION ConfigInfo,
    __in PBOOLEAN Reserved3
    )
{
    ULONG ghc;
    ULONG index;
    ULONG portCount, portImplemented;
    ULONG pci_cfg_len;
    UCHAR pci_cfg_buf[sizeof(PCI_COMMON_CONFIG)];
    PACCESS_RANGE accessRange;

    PAHCI_MEMORY_REGISTERS abar;
    PPCI_COMMON_CONFIG pciConfigData;
    PAHCI_ADAPTER_EXTENSION adapterExtension;

    DebugPrint("AhciHwFindAdapter()\n");

    adapterExtension = AdapterExtension;
    adapterExtension->SlotNumber = ConfigInfo->SlotNumber;
    adapterExtension->SystemIoBusNumber = ConfigInfo->SystemIoBusNumber;

    // get PCI configuration header
    pci_cfg_len = StorPortGetBusData(
                        adapterExtension,
                        PCIConfiguration,
                        adapterExtension->SystemIoBusNumber,
                        adapterExtension->SlotNumber,
                        pci_cfg_buf,
                        sizeof(PCI_COMMON_CONFIG));

    if (pci_cfg_len != sizeof(PCI_COMMON_CONFIG))
    {
        DebugPrint("\tpci_cfg_len != %d :: %d", sizeof(PCI_COMMON_CONFIG), pci_cfg_len);
        return SP_RETURN_ERROR;//Not a valid device at the given bus number
    }

    pciConfigData = pci_cfg_buf;
    adapterExtension->VendorID = pciConfigData->VendorID;
    adapterExtension->DeviceID = pciConfigData->DeviceID;
    adapterExtension->RevisionID = pciConfigData->RevisionID;
    // The last PCI base address register (BAR[5], header offset 0x24) points to the AHCI base memory, it’s called ABAR (AHCI Base Memory Register).
    adapterExtension->AhciBaseAddress = pciConfigData->u.type0.BaseAddresses[5] & (0xFFFFFFF0);

    DebugPrint("\tVendorID:%d  DeviceID:%d  RevisionID:%d\n", adapterExtension->VendorID,
                                                              adapterExtension->DeviceID,
                                                              adapterExtension->RevisionID);

    // 2.1.11
    abar = NULL;
    if (ConfigInfo->NumberOfAccessRanges > 0)
    {
        for (index = 0; index < ConfigInfo->NumberOfAccessRanges; index++)
        {
            accessRange = *ConfigInfo->AccessRanges;
            if (accessRange[index].RangeStart.QuadPart == adapterExtension->AhciBaseAddress)
            {
                abar = StorPortGetDeviceBase(adapterExtension,
                                             ConfigInfo->AdapterInterfaceType,
                                             ConfigInfo->SystemIoBusNumber,
                                             accessRange[index].RangeStart,
                                             accessRange[index].RangeLength,
                                             !accessRange[index].RangeInMemory);
                break;
            }
        }
    }

    if (abar == NULL)
    {
        DebugPrint("\tabar == NULL\n");
        return SP_RETURN_ERROR; // corrupted information supplied
    }

    adapterExtension->ABAR_Address = abar;
    adapterExtension->CAP = StorPortReadRegisterUlong(adapterExtension, &abar->CAP);
    adapterExtension->CAP2 = StorPortReadRegisterUlong(adapterExtension, &abar->CAP2);
    adapterExtension->Version = StorPortReadRegisterUlong(adapterExtension, &abar->VS);
    adapterExtension->LastInterruptPort = -1;

    // 10.1.2
    // 1. Indicate that system software is AHCI aware by setting GHC.AE to ‘1’.
    // 3.1.2 -- AE bit is read-write only if CAP.SAM is '0'
    ghc = StorPortReadRegisterUlong(adapterExtension, &abar->GHC);
    // AE := Highest Significant bit of GHC
    if ((ghc & AHCI_Global_HBA_CONTROL_AE) != 0)// Hmm, controller was already in power state
    {
        // reset controller to have it in known state
        DebugPrint("\tAE Already set, Reset()\n");
        if (!AhciAdapterReset(adapterExtension))
        {
            DebugPrint("\tReset Failed!\n");
            return SP_RETURN_ERROR;// reset failed
        }
    }

    ghc = AHCI_Global_HBA_CONTROL_AE;// only AE=1
    StorPortWriteRegisterUlong(adapterExtension, &abar->GHC, ghc);

    adapterExtension->IS = &abar->IS;
    adapterExtension->PortImplemented = StorPortReadRegisterUlong(adapterExtension, &abar->PI);

    if (adapterExtension->PortImplemented == 0)
    {
        DebugPrint("\tadapterExtension->PortImplemented == 0\n");
        return SP_RETURN_ERROR;
    }

    ConfigInfo->MaximumTransferLength = MAXIMUM_TRANSFER_LENGTH;//128 KB
    ConfigInfo->NumberOfPhysicalBreaks = 0x21;
    ConfigInfo->MaximumNumberOfTargets = 1;
    ConfigInfo->MaximumNumberOfLogicalUnits = 1;
    ConfigInfo->ResetTargetSupported = TRUE;
    ConfigInfo->NumberOfBuses = MAXIMUM_AHCI_PORT_COUNT;
    ConfigInfo->SynchronizationModel = StorSynchronizeFullDuplex;
    ConfigInfo->ScatterGather = TRUE;

    // Turn IE -- Interrupt Enabled
    ghc |= AHCI_Global_HBA_CONTROL_IE;
    StorPortWriteRegisterUlong(adapterExtension, &abar->GHC, ghc);

    // allocate necessary resource for each port
    if (!AhciAllocateResourceForAdapter(adapterExtension, ConfigInfo))
    {
        DebugPrint("\tAhciAllocateResourceForAdapter() == FALSE\n");
        return SP_RETURN_ERROR;
    }

    for (index = 0; index < adapterExtension->PortCount; index++)
    {
        if ((adapterExtension->PortImplemented & (0x1 << index)) != 0)
            AhciPortInitialize(&adapterExtension->PortExtension[index]);
    }

    return SP_RETURN_FOUND;
}// -- AhciHwFindAdapter();

/**
 * @name DriverEntry
 * @implemented
 *
 * Initial Entrypoint for storahci miniport driver
 *
 * @param DriverObject
 * @param RegistryPath
 *
 * @return
 * NT_STATUS in case of driver loaded successfully.
 */
ULONG
DriverEntry (
    __in PVOID DriverObject,
    __in PVOID RegistryPath
    )
{
    HW_INITIALIZATION_DATA hwInitializationData;
    ULONG i, status;

    DebugPrint("Storahci Loaded\n");

    // initialize the hardware data structure
    AhciZeroMemory(&hwInitializationData, sizeof(HW_INITIALIZATION_DATA));

    // set size of hardware initialization structure
    hwInitializationData.HwInitializationDataSize = sizeof(HW_INITIALIZATION_DATA);

    // identity required miniport entry point routines
    hwInitializationData.HwStartIo = AhciHwStartIo;
    hwInitializationData.HwResetBus = AhciHwResetBus;
    hwInitializationData.HwInterrupt = AhciHwInterrupt;
    hwInitializationData.HwInitialize = AhciHwInitialize;
    hwInitializationData.HwFindAdapter = AhciHwFindAdapter;

    // adapter specific information
    hwInitializationData.NeedPhysicalAddresses = TRUE;
    hwInitializationData.TaggedQueuing = TRUE;
    hwInitializationData.AutoRequestSense = TRUE;
    hwInitializationData.MultipleRequestPerLu = TRUE;

    hwInitializationData.NumberOfAccessRanges = 6;
    hwInitializationData.AdapterInterfaceType = PCIBus;
    hwInitializationData.MapBuffers = STOR_MAP_NON_READ_WRITE_BUFFERS;

    // set required extension sizes
    hwInitializationData.SrbExtensionSize = sizeof(AHCI_SRB_EXTENSION);
    hwInitializationData.DeviceExtensionSize = sizeof(AHCI_ADAPTER_EXTENSION);

    // register our hw init data
    status = StorPortInitialize(DriverObject,
                                RegistryPath,
                                &hwInitializationData,
                                NULL);

    DebugPrint("\tstatus:%x\n", status);
    return status;
}// -- DriverEntry();

/**
 * @name AhciATA_CFIS
 * @not_implemented
 *
 * create ATA CFIS from Srb
 *
 * @param PortExtension
 * @param Srb
 *
 */
VOID
AhciATA_CFIS (
    __in PAHCI_PORT_EXTENSION PortExtension,
    __in PAHCI_SRB_EXTENSION SrbExtension
    )
{
    DebugPrint("AhciATA_CFIS()\n");

}// -- AhciATA_CFIS();

/**
 * @name AhciATAPI_CFIS
 * @not_implemented
 *
 * create ATAPI CFIS from Srb
 *
 * @param PortExtension
 * @param Srb
 *
 */
VOID
AhciATAPI_CFIS (
    __in PAHCI_PORT_EXTENSION PortExtension,
    __in PAHCI_SRB_EXTENSION SrbExtension
    )
{
    DebugPrint("AhciATAPI_CFIS()\n");

}// -- AhciATAPI_CFIS();

/**
 * @name AhciBuild_PRDT
 * @not_implemented
 *
 * Build PRDT for data transfer
 *
 * @param PortExtension
 * @param Srb
 *
 * @return
 * Return number of entries in PRDT.
 */
ULONG
AhciBuild_PRDT (
    __in PAHCI_PORT_EXTENSION PortExtension,
    __in PAHCI_SRB_EXTENSION SrbExtension
    )
{
    DebugPrint("AhciBuild_PRDT()\n");

    return -1;
}// -- AhciBuild_PRDT();

/**
 * @name AhciProcessSrb
 * @implemented
 *
 * Prepare Srb for IO processing
 *
 * @param PortExtension
 * @param Srb
 * @param SlotIndex
 *
 */
VOID
AhciProcessSrb (
    __in PAHCI_PORT_EXTENSION PortExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in ULONG SlotIndex
    )
{
    ULONG prdtlen, sig, length;
    PAHCI_SRB_EXTENSION SrbExtension;
    PAHCI_COMMAND_HEADER CommandHeader;
    PAHCI_ADAPTER_EXTENSION AdapterExtension;
    STOR_PHYSICAL_ADDRESS CommandTablePhysicalAddress;

    DebugPrint("AhciProcessSrb()\n");

    NT_ASSERT(Srb->PathId == PortExtension->PortNumber);

    SrbExtension = Srb->SrbExtension;
    AdapterExtension = PortExtension->AdapterExtension;

    NT_ASSERT(SrbExtension != NULL);
    NT_ASSERT(SrbExtension->AtaFunction != 0);

    if ((SrbExtension->AtaFunction == ATA_FUNCTION_ATA_IDENTIFY) &&
        (SrbExtension->Task.CommandReg == IDE_COMMAND_NOT_VALID))
    {
        // Here we are safe to check SIG register
        sig = StorPortReadRegisterUlong(AdapterExtension, &PortExtension->Port->SIG);
        if (sig == 0x101)
        {
            SrbExtension->Task.CommandReg = IDE_COMMAND_IDENTIFY;
        }
        else
        {
            SrbExtension->Task.CommandReg = IDE_COMMAND_ATAPI_IDENTIFY;
        }
    }

    NT_ASSERT(SlotIndex < AHCI_Global_Port_CAP_NCS(AdapterExtension->CAP));
    SrbExtension->SlotIndex = SlotIndex;

    // program the CFIS in the CommandTable
    CommandHeader = &PortExtension->CommandList[SlotIndex];

    if (IsAtaCommand(SrbExtension->AtaFunction))
    {
        AhciATA_CFIS(PortExtension, SrbExtension);
    }
    else if (IsAtapiCommand(SrbExtension->AtaFunction))
    {
        AhciATAPI_CFIS(PortExtension, SrbExtension);
    }

    prdtlen = 0;
    if (IsDataTransferNeeded(SrbExtension))
    {
        prdtlen = AhciBuild_PRDT(PortExtension, SrbExtension);
        NT_ASSERT(prdtlen != -1);
    }

    // Program the command header
    CommandHeader->DI.PRDTL = prdtlen; // number of entries in PRD table
    CommandHeader->DI.CFL = 5;
    CommandHeader->DI.W = (SrbExtension->Flags & ATA_FLAGS_DATA_OUT) ? 1 : 0;
    CommandHeader->DI.P = 0;    // ATA Specifications says so
    CommandHeader->DI.PMP = 0;  // Port Multiplier

    // Reset -- Manual Configuation
    CommandHeader->DI.R = 0;
    CommandHeader->DI.B = 0;
    CommandHeader->DI.C = 0;

    CommandHeader->PRDBC = 0;

    CommandHeader->Reserved[0] = 0;
    CommandHeader->Reserved[1] = 0;
    CommandHeader->Reserved[2] = 0;
    CommandHeader->Reserved[3] = 0;

    // set CommandHeader CTBA
    // I am really not sure if SrbExtension is 128 byte aligned or not
    // Command FIS will not work if it is not so.
    CommandTablePhysicalAddress = StorPortGetPhysicalAddress(AdapterExtension,
                                                             NULL,
                                                             SrbExtension,
                                                             &length);

    // command table alignment
    NT_ASSERT((CommandTablePhysicalAddress.LowPart % 128) == 0);

    CommandHeader->CTBA0 = CommandTablePhysicalAddress.LowPart;

    if (IsAdapterCAPS64(AdapterExtension->CAP))
    {
        CommandHeader->CTBA_U0 = CommandTablePhysicalAddress.HighPart;
    }

    // mark this slot
    PortExtension->OccupiedSlots |= SlotIndex;
    return;
}// -- AhciProcessSrb();

/**
 * @name AhciActivatePort
 * @not_implemented
 *
 * Program Port and populate command list
 *
 * @param PortExtension
 *
 */
VOID
AhciActivatePort (
    __in PAHCI_PORT_EXTENSION PortExtension
    )
{
    DebugPrint("AhciActivatePort()\n");

    return;
}// -- AhciActivatePort();

/**
 * @name AhciProcessIO
 * @implemented
 *
 * Acquire Exclusive lock to port, populate pending commands to command List
 * program controller's port to process new commands in command list.
 *
 * @param AdapterExtension
 * @param PathId
 * @param Srb
 *
 */
VOID
AhciProcessIO (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in UCHAR PathId,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    STOR_LOCK_HANDLE lockhandle;
    PSCSI_REQUEST_BLOCK tmpSrb;
    PAHCI_PORT_EXTENSION PortExtension;
    ULONG commandSlotMask, occupiedSlots, slotIndex, NCS;

    DebugPrint("AhciProcessIO()\n");
    DebugPrint("\tPathId: %d\n", PathId);

    PortExtension = &AdapterExtension->PortExtension[PathId];

    NT_ASSERT(PathId < AdapterExtension->PortCount);

    // add Srb to queue
    AddQueue(&PortExtension->SrbQueue, Srb);

    if (PortExtension->IsActive == FALSE)
        return; // we should wait for device to get active

    AhciZeroMemory(&lockhandle, sizeof(lockhandle));

    // Acquire Lock
    StorPortAcquireSpinLock(AdapterExtension, InterruptLock, NULL, &lockhandle);

    occupiedSlots = PortExtension->OccupiedSlots; // Busy command slots for given port
    NCS = AHCI_Global_Port_CAP_NCS(AdapterExtension->CAP);
    commandSlotMask = (1 << NCS) - 1; // available slots mask

    commandSlotMask = (commandSlotMask & ~occupiedSlots);
    if(commandSlotMask != 0)
    {
        // iterate over HBA port slots
        for (slotIndex = 0; slotIndex < NCS; slotIndex++)
        {
            // find first free slot
            if ((commandSlotMask & (1 << slotIndex)) != 0)
            {
                tmpSrb = RemoveQueue(&PortExtension->SrbQueue);
                if (tmpSrb != NULL)
                {
                    NT_ASSERT(Srb->PathId == PathId);
                    AhciProcessSrb(PortExtension, tmpSrb, slotIndex);
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    // program HBA port
    AhciActivatePort(PortExtension);

    // Release Lock
    StorPortReleaseSpinLock(AdapterExtension, &lockhandle);

    return;
}// -- AhciProcessIO();

/**
 * @name DeviceInquiryRequest
 * @implemented
 *
 * Tells wheather given port is implemented or not
 *
 * @param AdapterExtension
 * @param Srb
 * @param Cdb
 *
 * @return
 * return STOR status for DeviceInquiryRequest
 *
 * @remark
 * http://www.seagate.com/staticfiles/support/disc/manuals/Interface%20manuals/100293068c.pdf
 */
ULONG
DeviceInquiryRequest (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in PCDB Cdb
    )
{
    PVOID DataBuffer;
    ULONG DataBufferLength;
    PAHCI_SRB_EXTENSION SrbExtension;

    DebugPrint("DeviceInquiryRequest()\n");

    SrbExtension = Srb->SrbExtension;

    // 3.6.1
    // If the EVPD bit is set to zero, the device server shall return the standard INQUIRY data
    if (Cdb->CDB6INQUIRY3.EnableVitalProductData == 0)
    {
        DebugPrint("\tEVPD Inquired\n");
        NT_ASSERT(SrbExtension != NULL);

        SrbExtension->AtaFunction = ATA_FUNCTION_ATA_IDENTIFY;
        SrbExtension->Task.CommandReg = IDE_COMMAND_NOT_VALID;
    }
    else
    {
        DebugPrint("\tVPD Inquired\n");

        DataBuffer = Srb->DataBuffer;
        DataBufferLength = Srb->DataTransferLength;

        if (DataBuffer == NULL)
        {
            return SRB_STATUS_INVALID_REQUEST;
        }

        AhciZeroMemory(DataBuffer, DataBufferLength);
    }

    AhciProcessIO(AdapterExtension, Srb->PathId, Srb);
    return SRB_STATUS_SUCCESS;
}// -- DeviceInquiryRequest();

/**
 * @name AhciAdapterReset
 * @implemented
 *
 * 10.4.3 HBA Reset
 * If the HBA becomes unusable for multiple ports, and a software reset or port reset does not correct the
 * problem, software may reset the entire HBA by setting GHC.HR to ‘1’. When software sets the GHC.HR
 * bit to ‘1’, the HBA shall perform an internal reset action. The bit shall be cleared to ‘0’ by the HBA when
 * the reset is complete. A software write of ‘0’ to GHC.HR shall have no effect. To perform the HBA reset,
 * software sets GHC.HR to ‘1’ and may poll until this bit is read to be ‘0’, at which point software knows that
 * the HBA reset has completed.
 * If the HBA has not cleared GHC.HR to ‘0’ within 1 second of software setting GHC.HR to ‘1’, the HBA is in
 * a hung or locked state.
 *
 * @param AdapterExtension
 *
 * @return
 * TRUE in case AHCI Controller RESTARTED successfully. i.e GHC.HR == 0
 */
BOOLEAN
AhciAdapterReset (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    ULONG ghc, ticks, ghcStatus;
    PAHCI_MEMORY_REGISTERS abar = NULL;

    DebugPrint("AhciAdapterReset()\n");

    abar = AdapterExtension->ABAR_Address;
    if (abar == NULL) // basic sanity
    {
        return FALSE;
    }

    // HR -- Very first bit (lowest significant)
    ghc = AHCI_Global_HBA_CONTROL_HR;
    StorPortWriteRegisterUlong(AdapterExtension, &abar->GHC, ghc);

    for (ticks = 0; ticks < 50; ++ticks)
    {
        ghcStatus = StorPortReadRegisterUlong(AdapterExtension, &abar->GHC);
        if ((ghcStatus & AHCI_Global_HBA_CONTROL_HR) == 0)
        {
            break;
        }
        StorPortStallExecution(20000);
    }

    if (ticks == 50)// 1 second
    {
        DebugPrint("\tDevice Timeout\n");
        return FALSE;
    }

    return TRUE;
}// -- AhciAdapterReset();

/**
 * @name AhciZeroMemory
 * @implemented
 *
 * Clear buffer by filling zeros
 *
 * @param Buffer
 * @param BufferSize
 */
__inline
VOID
AhciZeroMemory (
    __out PCHAR Buffer,
    __in ULONG BufferSize
    )
{
    ULONG i;
    for (i = 0; i < BufferSize; i++)
    {
        Buffer[i] = 0;
    }

    return;
}// -- AhciZeroMemory();

/**
 * @name IsPortValid
 * @implemented
 *
 * Tells wheather given port is implemented or not
 *
 * @param AdapterExtension
 * @param PathId
 *
 * @return
 * return TRUE if provided port is valid (implemented) or not
 */
__inline
BOOLEAN
IsPortValid (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in UCHAR pathId
    )
{
    NT_ASSERT(pathId >= 0);

    if (pathId >= AdapterExtension->PortCount)
    {
        return FALSE;
    }

    return AdapterExtension->PortExtension[pathId].IsActive;
}// -- IsPortValid()

/**
 * @name AddQueue
 * @implemented
 *
 * Add Srb to Queue
 *
 * @param Queue
 * @param Srb
 *
 * @return
 * return TRUE if Srb is successfully added to Queue
 *
 */
__inline
BOOLEAN
AddQueue (
    __inout PAHCI_QUEUE Queue,
    __in PVOID Srb
    )
{
    NT_ASSERT(Queue->Head < MAXIMUM_QUEUE_BUFFER_SIZE);
    NT_ASSERT(Queue->Tail < MAXIMUM_QUEUE_BUFFER_SIZE);

    if (Queue->Head == ((Queue->Tail + 1) % MAXIMUM_QUEUE_BUFFER_SIZE))
        return FALSE;

    Queue->Buffer[Queue->Head++] = Srb;
    Queue->Head %= MAXIMUM_QUEUE_BUFFER_SIZE;

    return TRUE;
}// -- AddQueue();

/**
 * @name RemoveQueue
 * @implemented
 *
 * Remove and return Srb from Queue
 *
 * @param Queue
 *
 * @return
 * return Srb
 *
 */
__inline
PVOID
RemoveQueue (
    __inout PAHCI_QUEUE Queue
    )
{
    PVOID Srb;

    NT_ASSERT(Queue->Head < MAXIMUM_QUEUE_BUFFER_SIZE);
    NT_ASSERT(Queue->Tail < MAXIMUM_QUEUE_BUFFER_SIZE);

    if (Queue->Head == Queue->Tail)
        return NULL;

    Srb = Queue->Buffer[Queue->Tail++];
    Queue->Tail %= MAXIMUM_QUEUE_BUFFER_SIZE;

    return Srb;
}// -- RemoveQueue();
