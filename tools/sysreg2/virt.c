#include "sysreg.h"
#include <libvirt.h>

const char* GetConsole(virDomainPtr vDomPtr)
{
    xmlDocPtr xml = NULL;
    xmlXPathObjectPtr obj = NULL;
    xmlXPathContextPtr ctxt = NULL;
    char* XmlDoc;
    const char* RetVal = NULL;

    XmlDoc = virDomainGetXMLDesc(vDomPtr, 0);
    if (!XmlDoc)
        return NULL;

    xml = xmlReadDoc((const xmlChar *) XmlDoc, "domain.xml", NULL,
            XML_PARSE_NOENT | XML_PARSE_NONET |
            XML_PARSE_NOWARNING);
    free(XmlDoc);
    if (!xml)
        return NULL;

    ctxt = xmlXPathNewContext(xml);
    if (!ctxt)
    {
        xmlFreeDoc(xml);
        return NULL;
    }

    obj = xmlXPathEval(BAD_CAST "string(/domain/devices/console/@tty)", ctxt);
    if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                         (obj->stringval != NULL) && (obj->stringval[0] != 0)))
    {
        RetVal = (const char*)obj->stringval;
    }

    xmlFreeDoc(xml);
    xmlXPathFreeContext(ctxt);
    return RetVal;
}

bool IsVirtualMachineRunning(virConnectPtr vConn, const char* name)
{
    int* ids = NULL;
    int numids;
    int maxids = 0;
    const char* domname;
    virDomainPtr vDomPtr = NULL;

    maxids = virConnectNumOfDomains(vConn);
    if (maxids < 0)
        return false;

    ids = malloc(sizeof(int) * maxids);
    if (!ids)
        return false;
    
    numids = virConnectListDomains(vConn, &ids[0], maxids);
    if (numids > -1)	
    {
        int i;
        for(i=0; i<numids; i++)
        {
            vDomPtr = virDomainLookupByID(vConn, ids[i]);
            domname = virDomainGetName(vDomPtr);
            if (strcasecmp(name, domname) == 0)
            {
                virDomainFree(vDomPtr);
                return true; 
            }
            virDomainFree(vDomPtr);
        }
    }
    return false;
}

virDomainPtr LaunchVirtualMachine(virConnectPtr vConn, const char* XmlFileName, const char* BootDevice)
{
    xmlDocPtr xml = NULL;
    xmlXPathObjectPtr obj = NULL;
    xmlXPathContextPtr ctxt = NULL;
    char* XmlDoc;
    char* buffer;
    const char* name;
    char* domname;
    int len = 0;

    buffer = ReadFile(XmlFileName);
    if (buffer == NULL)
        return NULL;

    xml = xmlReadDoc((const xmlChar *) buffer, "domain.xml", NULL,
                      XML_PARSE_NOENT | XML_PARSE_NONET |
                      XML_PARSE_NOWARNING);
    if (!xml)
        return NULL;

    ctxt = xmlXPathNewContext(xml);
    if (!ctxt)
        return NULL;

    obj = xmlXPathEval(BAD_CAST "/domain/os/boot", ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NODESET) 
            && (obj->nodesetval != NULL) && (obj->nodesetval->nodeTab != NULL)) 
    {
        xmlSetProp(obj->nodesetval->nodeTab[0], "dev", BootDevice);
    }

    free(buffer);
    xmlDocDumpMemory(xml, (xmlChar**) &buffer, &len);
    xmlFreeDoc(xml);
    xmlXPathFreeContext(ctxt);
    xmlXPathFreeObject(obj);

    virDomainPtr vDomPtr = virDomainDefineXML(vConn, buffer);
    xmlFree((xmlChar*)buffer);
    if (vDomPtr)
    {
        if (virDomainCreate(vDomPtr) != 0)
        {
            virDomainUndefine(vDomPtr);
            vDomPtr = NULL;
        }
        else
        {
            /* workaround a bug in libvirt */
            name = virDomainGetName(vDomPtr);
            domname = strdup(name);
            virDomainFree(vDomPtr);
            vDomPtr = virDomainLookupByName(vConn, domname);
            free(domname);
        }
    }
    return vDomPtr;
}


int main()
{
    virConnectPtr vConn;
    virDomainPtr vDom;
    int Stage;
    int Stages = 1; /* 1 for testing, should be set to 3 later */ 
    char qemu_img_cmdline[300];
    FILE* file;

    if (!LoadSettings("sysreg.xml"))
    {
        printf("Can not load configuration file\n");
        return EXIT_FAILURE;
    }

    vConn = virConnectOpen("qemu:///session");

    if (IsVirtualMachineRunning(vConn, AppSettings.Name))
    {
        printf("Error: Virtual Machine is already running.\n");
        return EXIT_FAILURE;
    }

    if (file = fopen(AppSettings.HardDiskImage, "r"))
    {
        fclose(file);
        remove(AppSettings.HardDiskImage);
    }

    sprintf(qemu_img_cmdline, "qemu-img create -f qcow2 %s %dM", 
            AppSettings.HardDiskImage, AppSettings.ImageSize);
    FILE* p = popen(qemu_img_cmdline, "r");
    char buf[100];
    while(feof(p)==0)
    {
        memset(buf,0,strlen(buf));
        fgets(buf,100,p);
        printf("%s\n",buf);
    }
    pclose(p); 

    for (Stage=0;Stage<Stages; Stage++)
    {
        vDom = LaunchVirtualMachine(vConn, AppSettings.Filename,
                AppSettings.Stage[Stage].BootDevice);
        {
            if (vDom)
            {
                printf("Domain %s started.\n", virDomainGetName(vDom));
                ProcessDebugData(GetConsole(vDom), 
                                 AppSettings.Timeout, Stage);
                virDomainDestroy(vDom);
                virDomainUndefine(vDom);
                virDomainFree(vDom);
            }
        }	
    }

    virConnectClose(vConn);
    return EXIT_SUCCESS;
}

