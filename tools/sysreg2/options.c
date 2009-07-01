/*
 * PROJECT:     ReactOS System Regression Testing Utility
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Loading the utility settings
 * COPYRIGHT:   Copyright 2008-2009 Christoph von Wittich <christoph_vw@reactos.org>
 *              Copyright 2009 Colin Finck <colin@reactos.org>
 */

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
    if (obj)
        xmlXPathFreeObject(obj);

    obj = xmlXPathEval(BAD_CAST"string(/settings/@vm)",ctxt);
    if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                     (obj->stringval != NULL) && (obj->stringval[0] != 0)))
    {
        strncpy(AppSettings.Name, obj->stringval, 79);
    }
    if (obj)
        xmlXPathFreeObject(obj);

    obj = xmlXPathEval(BAD_CAST"number(/settings/general/timeout/@ms)",ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NUMBER))
    {
        /* when no value is set - return value is negative
         * which means infinite */
        AppSettings.Timeout = (int)obj->floatval;
    }
    if (obj)
        xmlXPathFreeObject(obj);

    obj = xmlXPathEval(BAD_CAST"number(/settings/general/maxcachehits/@value)",ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NUMBER))
    {
        AppSettings.MaxCacheHits = (unsigned int)obj->floatval;
    }
    if (obj)
        xmlXPathFreeObject(obj);

    obj = xmlXPathEval(BAD_CAST"number(/settings/general/maxretries/@value)",ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NUMBER))
    {
        AppSettings.MaxRetries = (unsigned int)obj->floatval;
    }
    if (obj)
        xmlXPathFreeObject(obj);

    obj = xmlXPathEval(BAD_CAST"number(/settings/general/hdd/@size)",ctxt);
    if ((obj != NULL) && (obj->type == XPATH_NUMBER))
    {
        if (obj->floatval <= 0)
            AppSettings.ImageSize = 512;
        else
            AppSettings.ImageSize = (int)obj->floatval;
    }
    if (obj)
        xmlXPathFreeObject(obj);

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
        if (obj)
            xmlXPathFreeObject(obj);
        strcpy(TempStr, "string(/settings/");
        strcat(TempStr, StageNames[Stage]);
        strcat(TempStr, "/success/@on)");
        obj = xmlXPathEval((xmlChar*) TempStr,ctxt);
        if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
            (obj->stringval != NULL) && (obj->stringval[0] != 0)))
        {
            strncpy(AppSettings.Stage[Stage].Checkpoint, obj->stringval, 79);
        }
        if (obj)
            xmlXPathFreeObject(obj);
    }
    xmlFreeDoc(xml);
    xmlXPathFreeContext(ctxt);

    xml = xmlReadFile(AppSettings.Filename, NULL, 0);
    if (!xml)
        return false;
    ctxt = xmlXPathNewContext(xml);
    if (!ctxt)
    {
        xmlFreeDoc(xml);
        return false;
    }

    obj = xmlXPathEval(BAD_CAST"string(/domain/devices/disk[@device='disk']/source/@file)",ctxt);
    if ((obj != NULL) && ((obj->type == XPATH_STRING) &&
                     (obj->stringval != NULL) && (obj->stringval[0] != 0)))
    {
        strncpy(AppSettings.HardDiskImage, obj->stringval, 254);
    }
    if (obj)
        xmlXPathFreeObject(obj);

    xmlFreeDoc(xml);
    xmlXPathFreeContext(ctxt);
    return true;
}


