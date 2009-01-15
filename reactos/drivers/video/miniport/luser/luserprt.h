#ifndef LUSER_VIDEOPRT_H
#define LUSER_VIDEOPRT_H

#ifdef _MSC_VER
#include "dderror.h"
#include "devioctl.h"
#else
#include <ntddk.h>
#endif

#include "dderror.h"
#include "miniport.h"
#include "ntddvdeo.h"
#include "video.h"

#define DPRINT1(x...) do { \
    DbgPrint("(%s:%d) ", __FILE__, __LINE__); \
    DbgPrint(x); \
    } while(0)

typedef struct _LUSER_DEVICE_EXTENSION {
    int foo;
} LUSER_DEVICE_EXTENSION, *PLUSER_DEVICE_EXTENSION;

#endif/*LUSER_VIDEOPRT_H*/
