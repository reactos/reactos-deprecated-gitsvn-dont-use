#include "sysreg.h"

bool LoadSettings(const char* XmlConfig)
{
    xmlDocPtr xml = NULL;
    xmlXPathObjectPtr obj = NULL;
    xmlXPathContextPtr ctxt = NULL;
    char TempStr[255];
    int Stage;
    const char* StageNames[] = {
        "firststage",
        "secondstage",
        "thirdstage"
    };

    xml = xmlReadFile(XmlConfig, NULL, 0);
    if (!xml)
        return false;
    ctxt = xmlXPathNewContext(xml);
    if (!ctxt)
    {
        xmlFreeDoc(xml);
        return false;
    }

    obj = xmlXPathEval(BAD_CAST"string(/settings/@file)",ctxt);
    if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                    (obj->stringval != NULL) && (obj->stringval[0] != 0)))
    {
        strncpy(AppSettings.Filename, obj->stringval, 254);
    }

    obj = xmlXPathEval(BAD_CAST"string(/settings/@name)",ctxt);
    if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                     (obj->stringval != NULL) && (obj->stringval[0] != 0)))
    {
        strncpy(AppSettings.Name, obj->stringval, 79);
    }

    obj = xmlXPathEval(BAD_CAST"number(/settings/general/timeout/@ms)",ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NUMBER))
    {
        /* when no value is set - return value is negative
         * which means infinite */
        AppSettings.Timeout = (int)obj->floatval;
    }

    for (Stage=0;Stage<3;Stage++)
    {
        strcpy(TempStr, "string(/settings/");
        strcat(TempStr, StageNames[Stage]);
        strcat(TempStr, "/@bootdevice)");
        obj = xmlXPathEval((xmlChar*) TempStr,ctxt);
        if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                (obj->stringval != NULL) && (obj->stringval[0] != 0)))
        {
            strncpy(AppSettings.Stage[Stage].BootDevice, obj->stringval, 7);
        }
    }
    xmlFreeDoc(xml);
    xmlXPathFreeContext(ctxt);
    return true;
}

