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
    char* names[10];
    int iDomains;
    int iMaxnames = 10;

    iDomains = virConnectListDefinedDomains(vConn, names, iMaxnames);
    if (iDomains != -1)	 
    {
        int i;
        for(i=0; i<iDomains; i++)
        {
            if (strcasecmp(name, names[i]) == 0)
                return true;
        }
    }
    return false;
}

virDomainPtr LaunchVirtualMachine(virConnectPtr vConn, const char*  XmlFileName, const char* BootDevice)
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

    if (!LoadSettings("sysreg.xml"))
    {
        printf("Can not load configuration file\n");
        return EXIT_FAILURE;
    }

    vConn = virConnectOpen("qemu:///session");

    for (Stage=0;Stage<Stages; Stage++)
    {
        if (IsVirtualMachineRunning(vConn, AppSettings.Name))
            printf("Virtual Machine is already running\n");
        else
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
    }

    virConnectClose(vConn);
    return EXIT_SUCCESS;
}

