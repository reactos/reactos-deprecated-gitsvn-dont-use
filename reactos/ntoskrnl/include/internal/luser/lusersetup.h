#ifndef _NTOSKRNL_LUSER_LUSERSETUP_H
#define _NTOSKRNL_LUSER_LUSERSETUP_H

void SetupLdt();
void RegisterSigstack();
void RegisterSegv();

#endif/*_NTOSKRNL_LUSER_LUSERSETUP_H*/
