/*++

Copyright (c) 2002-2016 Alexandr A. Telyatnikov (Alter)

Module Name:
    bm_devs.h

Abstract:
    This file contains actual list of 'well known' PCI IDE controllers
    and must be included only in stub bm_devs.cpp

Author:
    Alexander A. Telyatnikov (Alter)

Environment:
    kernel and user mode

Notes:

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Revision History:

Licence:
    GPLv2

--*/

#include "bm_devs_decl.h"

BUSMASTER_CONTROLLER_INFORMATION_BASE const BusMasterAdapters[] = {

    PCI_DEV_HW_SPEC_BM( 0005, 1191, 0x00, ATA_UDMA2, "Acard ATP850"     , ATPOLD | UNIATA_SIMPLEX_ONLY            ),
    PCI_DEV_HW_SPEC_BM( 0006, 1191, 0x00, ATA_UDMA4, "Acard ATP860A"    , UNIATA_NO80CHK                          ),
    PCI_DEV_HW_SPEC_BM( 0007, 1191, 0x00, ATA_UDMA4, "Acard ATP860R"    , UNIATA_NO80CHK                          ),
    PCI_DEV_HW_SPEC_BM( 0008, 1191, 0x00, ATA_UDMA6, "Acard ATP865A"    , UNIATA_NO80CHK                          ),
    PCI_DEV_HW_SPEC_BM( 0009, 1191, 0x00, ATA_UDMA6, "Acard ATP865R"    , UNIATA_NO80CHK                          ),
                                          
    PCI_DEV_HW_SPEC_BM( 5289, 10b9, 0x00, ATA_SA150, "ALI M5289"        , UNIATA_SATA | UNIATA_NO_SLAVE           ),
    PCI_DEV_HW_SPEC_BM( 5288, 10b9, 0x00, ATA_SA300, "ALI M5288"        , UNIATA_SATA | UNIATA_NO_SLAVE           ),
    PCI_DEV_HW_SPEC_BM( 5287, 10b9, 0x00, ATA_SA150, "ALI M5287"        , UNIATA_SATA | UNIATA_NO_SLAVE           ),
    PCI_DEV_HW_SPEC_BM( 5281, 10b9, 0x00, ATA_SA150, "ALI M5281"        , UNIATA_SATA | UNIATA_NO_SLAVE           ),
    PCI_DEV_HW_SPEC_BM( 5228, 10b9, 0xc5, ATA_UDMA6, "ALI M5228 UDMA6"  , ALINEW                                  ),
    PCI_DEV_HW_SPEC_BM( 5229, 10b9, 0xc5, ATA_UDMA6, "ALI M5229 UDMA6"  , ALINEW                                  ),
    PCI_DEV_HW_SPEC_BM( 5229, 10b9, 0xc4, ATA_UDMA5, "ALI M5229 UDMA5"  , ALINEW                                  ),
    PCI_DEV_HW_SPEC_BM( 5229, 10b9, 0xc2, ATA_UDMA4, "ALI M5229 UDMA4"  , ALINEW                                  ),
    PCI_DEV_HW_SPEC_BM( 5229, 10b9, 0x20, ATA_UDMA2, "ALI M5229 UDMA2"  , ALIOLD                                  ),
    PCI_DEV_HW_SPEC_BM( 5229, 10b9, 0x00, ATA_WDMA2, "ALI M5229 WDMA2"  , ALIOLD                                  ),

    PCI_DEV_HW_SPEC_BM( 7401, 1022, 0x00, ATA_UDMA2, "AMD 755"          , 0 | 0x00                        ),
    PCI_DEV_HW_SPEC_BM( 7409, 1022, 0x00, ATA_UDMA4, "AMD 756"          , 0 | UNIATA_NO80CHK              ),
    PCI_DEV_HW_SPEC_BM( 7411, 1022, 0x00, ATA_UDMA5, "AMD 766"          , 0 | AMDBUG                      ),
    PCI_DEV_HW_SPEC_BM( 7441, 1022, 0x00, ATA_UDMA5, "AMD 768"          , 0                               ),
    PCI_DEV_HW_SPEC_BM( 7469, 1022, 0x00, ATA_UDMA6, "AMD 8111"         , 0                               ),
    PCI_DEV_HW_SPEC_BM( 208F, 1022, 0x00, ATA_UDMA4, "AMD CS5535"       , CYRIX_35                        ),
    PCI_DEV_HW_SPEC_BM( 209a, 1022, 0x00, ATA_UDMA5, "AMD CS5536"       , 0                               ),

    PCI_DEV_HW_SPEC_BM( 4349, 1002, 0x00, ATA_UDMA5, "ATI IXP200"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 4369, 1002, 0x00, ATA_UDMA6, "ATI IXP300"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 4376, 1002, 0x00, ATA_UDMA6, "ATI IXP400"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 436e, 1002, 0x00, ATA_SA150, "ATI IXP300"       , SIIMIO | SIIBUG |                UNIATA_SATA     ),
    PCI_DEV_HW_SPEC_BM( 4379, 1002, 0x00, ATA_SA150, "ATI IXP400"       , SIIMIO | SIIBUG | SIINOSATAIRQ | UNIATA_SATA     ),
    PCI_DEV_HW_SPEC_BM( 437a, 1002, 0x00, ATA_SA300, "ATI IXP400"       , SIIMIO | SIIBUG | SIINOSATAIRQ | UNIATA_SATA     ),
    PCI_DEV_HW_SPEC_BM( 438c, 1002, 0x00, ATA_UDMA6, "ATI IXP600"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 4380, 1002, 0x00, ATA_SA300, "ATI IXP600"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 439c, 1002, 0x00, ATA_UDMA6, "ATI IXP700"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 4390, 1002, 0x00, ATA_SA300, "ATI IXP700"       , ATI700 | UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 4391, 1002, 0x00, ATA_SA300, "ATI IXP700"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 4392, 1002, 0x00, ATA_SA300, "ATI IXP700"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 4393, 1002, 0x00, ATA_SA300, "ATI IXP700"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 4394, 1002, 0x00, ATA_SA300, "ATI IXP700/800"   , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 4395, 1002, 0x00, ATA_SA300, "ATI IXP700/800"   , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 780c, 1002, 0x00, ATA_UDMA6, "ATI Hudson-2"     , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 7800, 1002, 0x00, ATA_SA300, "ATI Hudson-2"     , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 7801, 1002, 0x00, ATA_SA300, "ATI Hudson-2"     , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 7802, 1002, 0x00, ATA_SA300, "ATI Hudson-2"     , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 7803, 1002, 0x00, ATA_SA300, "ATI Hudson-2"     , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 7804, 1002, 0x00, ATA_SA300, "ATI Hudson-2"     , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 0004, 1103, 0x05, ATA_UDMA6, "HighPoint HPT372" , HPT372 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0004, 1103, 0x03, ATA_UDMA5, "HighPoint HPT370" , HPT370 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0004, 1103, 0x02, ATA_UDMA4, "HighPoint HPT368" , HPT366 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0004, 1103, 0x00, ATA_UDMA4, "HighPoint HPT366" , HPT366 | HPTOLD | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0005, 1103, 0x01, ATA_UDMA6, "HighPoint HPT372" , HPT372 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0005, 1103, 0x00, ATA_UDMA4, "HighPoint HPT372" , HPT372 | HPTOLD | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0006, 1103, 0x01, ATA_UDMA6, "HighPoint HPT302" , HPT372 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0007, 1103, 0x01, ATA_UDMA6, "HighPoint HPT371" , HPT372 | 0x00   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0008, 1103, 0x07, ATA_UDMA6, "HighPoint HPT374" , HPT374 | 0x00   | UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 1230, 8086, 0x00, ATA_WDMA2, "Intel PIIX"       , UNIATA_CHAN_TIMINGS                     ),
    PCI_DEV_HW_SPEC_BM( 7010, 8086, 0x00, ATA_WDMA2, "Intel PIIX3"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 7111, 8086, 0x00, ATA_UDMA2, "Intel PIIX3"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 7199, 8086, 0x00, ATA_UDMA2, "Intel PIIX4"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 84ca, 8086, 0x00, ATA_UDMA2, "Intel PIIX4"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 7601, 8086, 0x00, ATA_UDMA2, "Intel ICH0"       , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 2421, 8086, 0x00, ATA_UDMA4, "Intel ICH"        , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 2411, 8086, 0x00, ATA_UDMA4, "Intel ICH"        , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 244a, 8086, 0x00, ATA_UDMA5, "Intel ICH2"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 244b, 8086, 0x00, ATA_UDMA5, "Intel ICH2"       , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 248a, 8086, 0x00, ATA_UDMA5, "Intel ICH3"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 248b, 8086, 0x00, ATA_UDMA5, "Intel ICH3"       , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 24cb, 8086, 0x00, ATA_UDMA5, "Intel ICH4"       , ICH4_FIX | UNIATA_NO_DPC                ),
    PCI_DEV_HW_SPEC_BM( 24ca, 8086, 0x00, ATA_UDMA5, "Intel ICH4"       , ICH4_FIX | UNIATA_NO_DPC                ),

    PCI_DEV_HW_SPEC_BM( 24db, 8086, 0x00, ATA_UDMA5, "Intel ICH5 EB"    , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 24d1, 8086, 0x00, ATA_SA150, "Intel ICH5 EB1"   , ICH5 | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 24df, 8086, 0x00, ATA_SA150, "Intel ICH5 EB2"   , ICH5 | UNIATA_SATA                      ),

    PCI_DEV_HW_SPEC_BM( 25a2, 8086, 0x00, ATA_UDMA5, "Intel 6300ESB"    , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 25a3, 8086, 0x00, ATA_SA150, "Intel 6300ESB1"   , ICH5 | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 25b0, 8086, 0x00, ATA_SA150, "Intel 6300ESB2"   , ICH5 | UNIATA_SATA                      ),

    PCI_DEV_HW_SPEC_BM( 266f, 8086, 0x00, ATA_UDMA5, "Intel ICH6"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 2651, 8086, 0x00, ATA_SA150, "Intel ICH6"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2652, 8086, 0x00, ATA_SA150, "Intel ICH6"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2653, 8086, 0x00, ATA_SA150, "Intel ICH6M"      , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 27df, 8086, 0x00, ATA_UDMA5, "Intel ICH7"       , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 27c0, 8086, 0x00, ATA_SA300, "Intel ICH7 S1"    , ICH7 | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 27c1, 8086, 0x00, ATA_SA300, "Intel ICH7"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 27c3, 8086, 0x00, ATA_SA300, "Intel ICH7"       , UNIATA_SATA                             ),
    PCI_DEV_HW_SPEC_BM( 27c4, 8086, 0x00, ATA_SA150, "Intel ICH7M R1"   , ICH7 | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 27c5, 8086, 0x00, ATA_SA150, "Intel ICH7M"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 27c6, 8086, 0x00, ATA_SA150, "Intel ICH7M"      , UNIATA_SATA                             ),

    PCI_DEV_HW_SPEC_BM( 269e, 8086, 0x00, ATA_UDMA5, "Intel 63XXESB2"   , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 2680, 8086, 0x00, ATA_SA300, "Intel 63XXESB2"   , UNIATA_SATA                             ),
    PCI_DEV_HW_SPEC_BM( 2681, 8086, 0x00, ATA_SA300, "Intel 63XXESB2"   , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2682, 8086, 0x00, ATA_SA300, "Intel 63XXESB2"   , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2683, 8086, 0x00, ATA_SA300, "Intel 63XXESB2"   , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 2820, 8086, 0x00, ATA_SA300, "Intel ICH8"       , I6CH                                    ),
    PCI_DEV_HW_SPEC_BM( 2821, 8086, 0x00, ATA_SA300, "Intel ICH8"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2822, 8086, 0x00, ATA_SA300, "Intel ICH8"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2824, 8086, 0x00, ATA_SA300, "Intel ICH8"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2825, 8086, 0x00, ATA_SA300, "Intel ICH8"       , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 2828, 8086, 0x00, ATA_SA300, "Intel ICH8M"      , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 2829, 8086, 0x00, ATA_SA300, "Intel ICH8M"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 282a, 8086, 0x00, ATA_SA300, "Intel ICH8M"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2850, 8086, 0x00, ATA_UDMA5, "Intel ICH8M"      , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 2920, 8086, 0x00, ATA_SA300, "Intel ICH9"       , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 2926, 8086, 0x00, ATA_SA300, "Intel ICH9"       , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 2921, 8086, 0x00, ATA_SA300, "Intel ICH9"       , UNIATA_SATA | UNIATA_AHCI               ),/* ??? */
    PCI_DEV_HW_SPEC_BM( 2922, 8086, 0x00, ATA_SA300, "Intel ICH9"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2923, 8086, 0x00, ATA_SA300, "Intel ICH9"       , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2925, 8086, 0x00, ATA_SA300, "Intel ICH9"       , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 2928, 8086, 0x00, ATA_SA300, "Intel ICH9M"      , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 2929, 8086, 0x00, ATA_SA300, "Intel ICH9M"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 292a, 8086, 0x00, ATA_SA300, "Intel ICH9M"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 292d, 8086, 0x00, ATA_SA300, "Intel ICH9M"      , I6CH2 | UNIATA_SATA                     ),
    
    PCI_DEV_HW_SPEC_BM( 3a20, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 3a26, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 3a22, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3a25, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 3a00, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 3a06, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 3a02, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3a05, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),
/*
    PCI_DEV_HW_SPEC_BM( ????, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( ????, 8086, 0x00, ATA_SA300, "Intel ICH10"      , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( ????, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( ????, 8086, 0x00, ATA_SA300, "Intel ICH10"      , UNIATA_SATA | UNIATA_AHCI               ),
*/
    PCI_DEV_HW_SPEC_BM( 3b20, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 3b21, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 3b22, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3b23, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3b25, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3b26, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 3b28, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 3b29, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3b2c, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 3b2d, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 3b2e, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 3b2f, 8086, 0x00, ATA_SA300, "Intel 5 Series/3400" , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 1c00, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 1c01, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 1c02, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1c03, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1c04, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1c05, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1c08, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 1c09, 8086, 0x00, ATA_SA300, "Intel Cougar Point"  , I6CH2 | UNIATA_SATA                     ),

    PCI_DEV_HW_SPEC_BM( 1d00, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 1d02, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1d04, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1d06, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 2826, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1d08, 8086, 0x00, ATA_SA300, "Intel Patsburg"      , I6CH2 | UNIATA_SATA                     ),

    PCI_DEV_HW_SPEC_BM( 1e00, 8086, 0x00, ATA_SA300, "Intel Panther Point" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 1e01, 8086, 0x00, ATA_SA300, "Intel Panther Point" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 1e02, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e03, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e04, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e05, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e06, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e07, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e08, 8086, 0x00, ATA_SA300, "Intel Panther Point" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 1e09, 8086, 0x00, ATA_SA300, "Intel Panther Point" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 1e0e, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 1e0f, 8086, 0x00, ATA_SA300, "Intel Panther Point" , UNIATA_SATA | UNIATA_AHCI               ),

    PCI_DEV_HW_SPEC_BM( 8c00, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 8c01, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , I6CH | UNIATA_SATA                      ),
    PCI_DEV_HW_SPEC_BM( 8c02, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c03, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c04, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c05, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c06, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c07, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c08, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 8c09, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , I6CH2 | UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 8c0e, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),
    PCI_DEV_HW_SPEC_BM( 8c0f, 8086, 0x00, ATA_SA300, "Intel Lynx Point" , UNIATA_SATA | UNIATA_AHCI               ),

//    PCI_DEV_HW_SPEC_BM( 3200, 8086, 0x00, ATA_SA150, "Intel 31244"      , UNIATA_SATA                             ),
    PCI_DEV_HW_SPEC_BM( 811a, 8086, 0x00, ATA_UDMA5, "Intel SCH"        , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 2323, 8086, 0x00, ATA_SA300, "Intel DH98xxCC"   , UNIATA_SATA | UNIATA_AHCI                  ),

    PCI_DEV_HW_SPEC_BM( 23a3, 8086, 0x00, ATA_SA300, "COLETOCRK" , I6CH2 | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 23a1, 8086, 0x00, ATA_SA300, "COLETOCRK" , I6CH2 | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 23a6, 8086, 0x00, ATA_SA300, "COLETOCRK" , UNIATA_SATA | UNIATA_AHCI              ),
    
    PCI_DEV_HW_SPEC_BM( 2360, 197b, 0x00, ATA_SA300, "JMB360"           , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 2361, 197b, 0x00, ATA_UDMA6, "JMB361"           , 0                               ),
    PCI_DEV_HW_SPEC_BM( 2362, 197b, 0x00, ATA_SA300, "JMB362"           , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 2363, 197b, 0x00, ATA_UDMA6, "JMB363"           , 0                               ),
    PCI_DEV_HW_SPEC_BM( 2365, 197b, 0x00, ATA_UDMA6, "JMB365"           , 0                               ),
    PCI_DEV_HW_SPEC_BM( 2366, 197b, 0x00, ATA_UDMA6, "JMB366"           , 0                               ),
    PCI_DEV_HW_SPEC_BM( 2368, 197b, 0x00, ATA_UDMA6, "JMB368"           , 0                               ),
/*    
    PCI_DEV_HW_SPEC_BM( 5040, 11ab, 0x00, ATA_SA150, "Marvell 88SX5040" , UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 5041, 11ab, 0x00, ATA_SA150, "Marvell 88SX5041" , UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 5080, 11ab, 0x00, ATA_SA150, "Marvell 88SX5080" , UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 5081, 11ab, 0x00, ATA_SA150, "Marvell 88SX5081" , UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 6041, 11ab, 0x00, ATA_SA300, "Marvell 88SX6041" , UNIATA_SATA                     ),
    PCI_DEV_HW_SPEC_BM( 6081, 11ab, 0x00, ATA_SA300, "Marvell 88SX6081" , UNIATA_SATA                     ),*/
    PCI_DEV_HW_SPEC_BM( 6101, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6101" , 0                               ),
    PCI_DEV_HW_SPEC_BM( 6102, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6102" , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 6111, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6111" , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 6121, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6121" , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 6141, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6141" , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 6145, 11ab, 0x00, ATA_UDMA6, "Marvell 88SX6145" , UNIATA_SATA | UNIATA_AHCI       ),
    PCI_DEV_HW_SPEC_BM( 9123, 1b4b, 0x00, ATA_UDMA6, "Marvell 88SX9123" , UNIATA_SATA | UNIATA_AHCI       ),
/*    PCI_DEV_HW_SPEC_BM( 91a4, 1b4b, 0x00, ATA_UDMA6, "Marvell 88SE912x" , 0                                       ),*/

    PCI_DEV_HW_SPEC_BM( 01bc, 10de, 0x00, ATA_UDMA5, "nVidia nForce"    , 0                               ),
    PCI_DEV_HW_SPEC_BM( 0065, 10de, 0x00, ATA_UDMA6, "nVidia nForce2"   , 0                               ),
    PCI_DEV_HW_SPEC_BM( 0085, 10de, 0x00, ATA_UDMA6, "nVidia nForce2 Pro",0                               ),
    PCI_DEV_HW_SPEC_BM( 008e, 10de, 0x00, ATA_SA150, "nVidia nForce2 Pro S1",UNIATA_SATA                  ),
    PCI_DEV_HW_SPEC_BM( 00d5, 10de, 0x00, ATA_UDMA6, "nVidia nForce3"   , 0                               ),
    PCI_DEV_HW_SPEC_BM( 00e5, 10de, 0x00, ATA_UDMA6, "nVidia nForce3 Pro",0                               ),
    PCI_DEV_HW_SPEC_BM( 00e3, 10de, 0x00, ATA_SA150, "nVidia nForce3 Pro S1",UNIATA_SATA                  ),
    PCI_DEV_HW_SPEC_BM( 00ee, 10de, 0x00, ATA_SA150, "nVidia nForce3 Pro S2",UNIATA_SATA                  ),
    PCI_DEV_HW_SPEC_BM( 0035, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP", 0                               ),
    PCI_DEV_HW_SPEC_BM( 0036, 10de, 0x00, ATA_SA150, "nVidia nForce MCP S1",NV4OFF | UNIATA_SATA          ),
    PCI_DEV_HW_SPEC_BM( 003e, 10de, 0x00, ATA_SA150, "nVidia nForce MCP S2",NV4OFF | UNIATA_SATA          ),
    PCI_DEV_HW_SPEC_BM( 0053, 10de, 0x00, ATA_UDMA6, "nVidia nForce CK804", 0                             ),
    PCI_DEV_HW_SPEC_BM( 0054, 10de, 0x00, ATA_SA300, "nVidia nForce CK804 S1",NV4OFF | UNIATA_SATA        ),
    PCI_DEV_HW_SPEC_BM( 0055, 10de, 0x00, ATA_SA300, "nVidia nForce CK804 S2",NV4OFF | UNIATA_SATA        ),
    PCI_DEV_HW_SPEC_BM( 0265, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP51", 0                             ),
    PCI_DEV_HW_SPEC_BM( 0266, 10de, 0x00, ATA_SA300, "nVidia nForce MCP51 S1",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 0267, 10de, 0x00, ATA_SA300, "nVidia nForce MCP51 S2",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 036e, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP55", 0                             ),
    PCI_DEV_HW_SPEC_BM( 037e, 10de, 0x00, ATA_SA300, "nVidia nForce MCP55 S1",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 037f, 10de, 0x00, ATA_SA300, "nVidia nForce MCP55 S2",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 03ec, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP61", 0                             ),
    PCI_DEV_HW_SPEC_BM( 03e7, 10de, 0x00, ATA_SA300, "nVidia nForce MCP61 S1",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 03f6, 10de, 0x00, ATA_SA300, "nVidia nForce MCP61 S2",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 03f7, 10de, 0x00, ATA_SA300, "nVidia nForce MCP61 S3",NV4OFF | NVQ | UNIATA_SATA  ),
    PCI_DEV_HW_SPEC_BM( 0448, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP65", 0                             ),
    PCI_DEV_HW_SPEC_BM( 044c, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 044d, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 044e, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 044f, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 045c, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 045d, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 045e, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 045f, 10de, 0x00, ATA_SA300, "nVidia nForce MCP65 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0560, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP67", 0                             ),
    PCI_DEV_HW_SPEC_BM( 0550, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0551, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0552, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0553, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0554, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0555, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0556, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0557, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0558, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A8", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0559, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 A9", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 055A, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 AA", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 055B, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 AB", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0584, 10de, 0x00, ATA_SA300, "nVidia nForce MCP67 AC", UNIATA_SATA | UNIATA_AHCI  ),

    PCI_DEV_HW_SPEC_BM( 056c, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP73", 0                             ),
    PCI_DEV_HW_SPEC_BM( 07f0, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f1, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f2, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f3, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f4, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f5, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f6, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f7, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f8, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A8", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07f9, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 A9", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 07fa, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 AA", UNIATA_SATA | UNIATA_AHCI  ),

    PCI_DEV_HW_SPEC_BM( 07fb, 10de, 0x00, ATA_SA300, "nVidia nForce MCP73 AB", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0759, 10de, 0x00, ATA_UDMA6, "nVidia nForce MCP77", 0                             ),
    PCI_DEV_HW_SPEC_BM( 0ad0, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad1, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad2, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad3, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad4, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad5, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad6, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad7, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad8, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A8", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ad9, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 A9", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ada, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 AA", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0adb, 10de, 0x00, ATA_SA300, "nVidia nForce MCP77 AB", UNIATA_SATA | UNIATA_AHCI  ),

    PCI_DEV_HW_SPEC_BM( 0ab4, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ab5, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ab6, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ab7, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ab8, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0ab9, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0aba, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0abb, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0abc, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A8", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0abd, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 A9", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0abe, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 AA", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0abf, 10de, 0x00, ATA_SA300, "nVidia nForce MCP79 AB", UNIATA_SATA | UNIATA_AHCI  ),

    PCI_DEV_HW_SPEC_BM( 0d84, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A0", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d85, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A1", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d86, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A2", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d87, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A3", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d88, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A4", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d89, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A5", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8a, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A6", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8b, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A7", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8c, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A8", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8d, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 A9", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8e, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 AA", UNIATA_SATA | UNIATA_AHCI  ),
    PCI_DEV_HW_SPEC_BM( 0d8f, 10de, 0x00, ATA_SA300, "nVidia nForce MCP89 AB", UNIATA_SATA | UNIATA_AHCI  ),

    PCI_DEV_HW_SPEC_BM( 0502, 100b, 0x00, ATA_UDMA2, "National Geode SC1100", 0                                   ),
    PCI_DEV_HW_SPEC_BM( 002d, 100b, 0x00, ATA_UDMA4, "National Geode CS5535", CYRIX_35                            ),

    PCI_DEV_HW_SPEC_BM( 4d33, 105a, 0x00, ATA_UDMA2, "Promise PDC20246" , PROLD | 0x00                            ),
    PCI_DEV_HW_SPEC_BM( 4d38, 105a, 0x00, ATA_UDMA4, "Promise PDC20262" , PRNEW | 0x00                            ),
    PCI_DEV_HW_SPEC_BM( 0d38, 105a, 0x00, ATA_UDMA4, "Promise PDC20263" , PRNEW | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0d30, 105a, 0x00, ATA_UDMA5, "Promise PDC20265" , PRNEW | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 4d30, 105a, 0x00, ATA_UDMA5, "Promise PDC20267" , PRNEW | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 4d68, 105a, 0x00, ATA_UDMA5, "Promise PDC20268" , PRTX  | PRTX4   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 4d69, 105a, 0x00, ATA_UDMA6, "Promise PDC20269" , PRTX  | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6268, 105a, 0x00, ATA_UDMA5, "Promise PDC20270" , PRTX  | PRTX4   | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6269, 105a, 0x00, ATA_UDMA6, "Promise PDC20271" , PRTX  | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 1275, 105a, 0x00, ATA_UDMA6, "Promise PDC20275" , PRTX  | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 5275, 105a, 0x00, ATA_UDMA6, "Promise PDC20276" , PRTX  | PRSX6K  | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 7275, 105a, 0x00, ATA_UDMA6, "Promise PDC20277" , PRTX  | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 3318, 105a, 0x00, ATA_SA150, "Promise PDC20318" , PRMIO | PRSATA  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3319, 105a, 0x00, ATA_SA150, "Promise PDC20319" , PRMIO | PRSATA  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3371, 105a, 0x00, ATA_SA150, "Promise PDC20371" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3375, 105a, 0x00, ATA_SA150, "Promise PDC20375" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3376, 105a, 0x00, ATA_SA150, "Promise PDC20376" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3377, 105a, 0x00, ATA_SA150, "Promise PDC20377" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3373, 105a, 0x00, ATA_SA150, "Promise PDC20378" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3372, 105a, 0x00, ATA_SA150, "Promise PDC20379" , PRMIO | PRCMBO  | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3571, 105a, 0x00, ATA_SA150, "Promise PDC20571" , PRMIO | PRCMBO2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3d75, 105a, 0x00, ATA_SA150, "Promise PDC20575" , PRMIO | PRCMBO2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3574, 105a, 0x00, ATA_SA150, "Promise PDC20579" , PRMIO | PRCMBO2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3570, 105a, 0x00, ATA_SA300, "Promise PDC20771" , PRMIO | PRCMBO2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3d73, 105a, 0x00, ATA_SA300, "Promise PDC40775" , PRMIO | PRCMBO2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 6617, 105a, 0x00, ATA_UDMA6, "Promise PDC20617" , PRMIO | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6626, 105a, 0x00, ATA_UDMA6, "Promise PDC20618" , PRMIO | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6629, 105a, 0x00, ATA_UDMA6, "Promise PDC20619" , PRMIO | 0x00    | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6620, 105a, 0x00, ATA_UDMA6, "Promise PDC20620" , PRMIO | 0x00    | UNIATA_RAID_CONTROLLER),
/*    PCI_DEV_HW_SPEC_BM( 6621, 105a, 0x00, ATA_UDMA6, "Promise PDC20621" , PRMIO | PRSX4X  | UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6622, 105a, 0x00, ATA_UDMA6, "Promise PDC20622" , PRMIO | PRSX4X  | UNIATA_RAID_CONTROLLER),*/
    PCI_DEV_HW_SPEC_BM( 3571, 105a, 0x00, ATA_SA150, "Promise PDC40518" , PRMIO | PRSATA2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3d75, 105a, 0x00, ATA_SA150, "Promise PDC40519" , PRMIO | PRSATA2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3574, 105a, 0x00, ATA_SA150, "Promise PDC40718" , PRMIO | PRSATA2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3570, 105a, 0x00, ATA_SA300, "Promise PDC40719" , PRMIO | PRSATA2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 3d73, 105a, 0x00, ATA_SA300, "Promise PDC40779" , PRMIO | PRSATA2 | UNIATA_RAID_CONTROLLER | UNIATA_SATA),

    PCI_DEV_HW_SPEC_BM( 0211, 1166, 0x00, ATA_UDMA2, "ServerWorks ROSB4", SWKS33  | UNIATA_NO_DPC                 ),
    PCI_DEV_HW_SPEC_BM( 0212, 1166, 0x92, ATA_UDMA5, "ServerWorks CSB5" , SWKS100                                 ),
    PCI_DEV_HW_SPEC_BM( 0212, 1166, 0x00, ATA_UDMA4, "ServerWorks CSB5" , SWKS66                                  ),
    PCI_DEV_HW_SPEC_BM( 0213, 1166, 0x00, ATA_UDMA5, "ServerWorks CSB6" , SWKS100                                 ),
    PCI_DEV_HW_SPEC_BM( 0217, 1166, 0x00, ATA_UDMA4, "ServerWorks CSB6" , SWKS66                                  ),
    PCI_DEV_HW_SPEC_BM( 0214, 1166, 0x00, ATA_UDMA5, "ServerWorks HT1000" , SWKS100                               ),
    PCI_DEV_HW_SPEC_BM( 024b, 1166, 0x00, ATA_SA150, "ServerWorks HT1000" , SWKS100                               ),
    PCI_DEV_HW_SPEC_BM( 024a, 1166, 0x00, ATA_SA150, "ServerWorks HT1000" , SWKSMIO                               ),
    PCI_DEV_HW_SPEC_BM( 0240, 1166, 0x00, ATA_SA150, "ServerWorks K2"     , SWKSMIO                               ),
    PCI_DEV_HW_SPEC_BM( 0241, 1166, 0x00, ATA_SA150, "ServerWorks Frodo4" , SWKSMIO                               ),
    PCI_DEV_HW_SPEC_BM( 0242, 1166, 0x00, ATA_SA150, "ServerWorks Frodo8" , SWKSMIO                               ),

    PCI_DEV_HW_SPEC_BM( 3114, 1095, 0x00, ATA_SA150, "SiI 3114"         , SIIMIO | SII4CH | UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 3512, 1095, 0x02, ATA_SA150, "SiI 3512"         , SIIMIO |          UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 3112, 1095, 0x02, ATA_SA150, "SiI 3112"         , SIIMIO |          UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 0240, 1095, 0x02, ATA_SA150, "SiI 3112"         , SIIMIO |          UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 3512, 1095, 0x00, ATA_SA150, "SiI 3512"         , SIIMIO | SIIBUG | UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 3112, 1095, 0x00, ATA_SA150, "SiI 3112"         , SIIMIO | SIIBUG | UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 0240, 1095, 0x00, ATA_SA150, "SiI 3112"         , SIIMIO | SIIBUG | UNIATA_SATA           ),
    PCI_DEV_HW_SPEC_BM( 0680, 1095, 0x00, ATA_UDMA6, "SiI 0680"         , SIIMIO | SIISETCLK                      ),
    PCI_DEV_HW_SPEC_BM( 0649, 1095, 0x00, ATA_UDMA5, "CMD 649"          , SIICMD | SIIINTR | UNIATA_NO_DPC_ATAPI  ),
    PCI_DEV_HW_SPEC_BM( 0648, 1095, 0x00, ATA_UDMA4, "CMD 648"          , SIICMD | SIIINTR                        ),
    PCI_DEV_HW_SPEC_BM( 0646, 1095, 0x07, ATA_UDMA2, "CMD 646U2"        , SIICMD | 0                              ),
    PCI_DEV_HW_SPEC_BM( 0646, 1095, 0x00, ATA_WDMA2, "CMD 646"          , SIICMD | 0                              ),
    PCI_DEV_HW_SPEC_BM( 0640, 1095, 0x00, ATA_PIO4 , "CMD 640"          , SIIOLD | SIIENINTR | UNIATA_SIMPLEX_ONLY),

/**    PCI_DEV_HW_SPEC_BM( 0963, 1039, 0x00, ATA_UDMA6, "SiS 963"          , SIS133NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0962, 1039, 0x00, ATA_UDMA6, "SiS 962"          , SIS133NEW                               ),

    PCI_DEV_HW_SPEC_BM( 0755, 1039, 0x00, ATA_UDMA6, "SiS 755"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0752, 1039, 0x00, ATA_UDMA6, "SiS 752"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0751, 1039, 0x00, ATA_UDMA6, "SiS 751"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0750, 1039, 0x00, ATA_UDMA6, "SiS 750"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0748, 1039, 0x00, ATA_UDMA6, "SiS 748"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0746, 1039, 0x00, ATA_UDMA6, "SiS 746"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0745, 1039, 0x00, ATA_UDMA5, "SiS 745"          , SIS100NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0740, 1039, 0x00, ATA_UDMA5, "SiS 740"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0735, 1039, 0x00, ATA_UDMA5, "SiS 735"          , SIS100NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0733, 1039, 0x00, ATA_UDMA5, "SiS 733"          , SIS100NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0730, 1039, 0x00, ATA_UDMA5, "SiS 730"          , SIS100OLD                               ),

    PCI_DEV_HW_SPEC_BM( 0658, 1039, 0x00, ATA_UDMA6, "SiS 658"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0655, 1039, 0x00, ATA_UDMA6, "SiS 655"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0652, 1039, 0x00, ATA_UDMA6, "SiS 652"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0651, 1039, 0x00, ATA_UDMA6, "SiS 651"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0650, 1039, 0x00, ATA_UDMA6, "SiS 650"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0648, 1039, 0x00, ATA_UDMA6, "SiS 648"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0646, 1039, 0x00, ATA_UDMA6, "SiS 645DX"        , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0645, 1039, 0x00, ATA_UDMA6, "SiS 645"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0640, 1039, 0x00, ATA_UDMA4, "SiS 640"          , SIS_SOUTH                               ),
    PCI_DEV_HW_SPEC_BM( 0635, 1039, 0x00, ATA_UDMA5, "SiS 635"          , SIS100NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0633, 1039, 0x00, ATA_UDMA5, "SiS 633"          , SIS100NEW                               ),
    PCI_DEV_HW_SPEC_BM( 0630, 1039, 0x00, ATA_UDMA5, "SiS 630S"         , SIS100OLD                               ),
    PCI_DEV_HW_SPEC_BM( 0630, 1039, 0x00, ATA_UDMA4, "SiS 630"          , SIS66,                                  ),
    PCI_DEV_HW_SPEC_BM( 0620, 1039, 0x00, ATA_UDMA4, "SiS 620"          , SIS66,                                  ),

    PCI_DEV_HW_SPEC_BM( 0550, 1039, 0x00, ATA_UDMA5, "SiS 550"          , SIS66,                                  ),
    PCI_DEV_HW_SPEC_BM( 0540, 1039, 0x00, ATA_UDMA4, "SiS 540"          , SIS66,                                  ),
    PCI_DEV_HW_SPEC_BM( 0530, 1039, 0x00, ATA_UDMA4, "SiS 530"          , SIS66,                                  ),
*/
    PCI_DEV_HW_SPEC_BM( 5513, 1039, 0xc2, ATA_UDMA2, "SiS ATA-xxx"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 5513, 1039, 0x00, ATA_WDMA2, "SiS ATA-xxx"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 5518, 1039, 0x00, ATA_UDMA6, "SiS 962/3"        , SIS133NEW | SIS_BASE                            ),
    PCI_DEV_HW_SPEC_BM( 0601, 1039, 0x00, ATA_WDMA2, "SiS ATA-xxx"      , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 1183, 1039, 0x00, ATA_SA150, "SiS 1183 SATA" , SISSATA),
    PCI_DEV_HW_SPEC_BM( 1182, 1039, 0x00, ATA_SA150, "SiS 1182"      , SISSATA   | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 0183, 1039, 0x00, ATA_SA150, "SiS 183 RAID"  , SISSATA   | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 0182, 1039, 0x00, ATA_SA150, "SiS SATA 182"  , SISSATA   | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 0181, 1039, 0x00, ATA_SA150, "SiS SATA 181"  , SISSATA   | UNIATA_SATA),
    PCI_DEV_HW_SPEC_BM( 0180, 1039, 0x00, ATA_SA150, "SiS SATA 180"  , SISSATA   | UNIATA_SATA),

/*    PCI_DEV_HW_SPEC_BM( 0586, 1106, 0x41, ATA_UDMA2, "VIA 82C586B"      , VIA33  | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 0586, 1106, 0x40, ATA_UDMA2, "VIA 82C586B"      , VIA33  | VIAPRQ                         ),
    PCI_DEV_HW_SPEC_BM( 0586, 1106, 0x02, ATA_UDMA2, "VIA 82C586B"      , VIA33  | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 0586, 1106, 0x00, ATA_WDMA2, "VIA 82C586"       , VIA33  | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 0596, 1106, 0x12, ATA_UDMA4, "VIA 82C596B"      , VIA66  | VIACLK                         ),
    PCI_DEV_HW_SPEC_BM( 0596, 1106, 0x00, ATA_UDMA2, "VIA 82C596"       , VIA33  | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 0686, 1106, 0x40, ATA_UDMA5, "VIA 82C686B"      , VIA100 | VIABUG                         ),
    PCI_DEV_HW_SPEC_BM( 0686, 1106, 0x10, ATA_UDMA4, "VIA 82C686A"      , VIA66  | VIACLK                         ),
    PCI_DEV_HW_SPEC_BM( 0686, 1106, 0x00, ATA_UDMA2, "VIA 82C686"       , VIA33  | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 8231, 1106, 0x00, ATA_UDMA5, "VIA 8231"         , VIA100 | VIABUG                         ),
    PCI_DEV_HW_SPEC_BM( 3074, 1106, 0x00, ATA_UDMA5, "VIA 8233"         , VIA100 | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 3109, 1106, 0x00, ATA_UDMA5, "VIA 8233C"        , VIA100 | 0x00                           ),
    PCI_DEV_HW_SPEC_BM( 3147, 1106, 0x00, ATA_UDMA6, "VIA 8233A"        , VIA133 | VIAAST                         ),
    PCI_DEV_HW_SPEC_BM( 3177, 1106, 0x00, ATA_UDMA6, "VIA 8235"         , VIA133 | VIAAST                         ),
*/
    PCI_DEV_HW_SPEC_BM( 0571, 1106, 0x00, ATA_UDMA2, "VIA ATA-xxx"      , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 0581, 1106, 0x00, ATA_UDMA6, "VIA UATA-xxx"     , 0                                       ),
    /* has no SATA registers, all mapped to PATA-style regs */
    PCI_DEV_HW_SPEC_BM( 5324, 1106, 0x00, ATA_SA150, "VIA SATA-xxx"     , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 3164, 1106, 0x00, ATA_UDMA6, "VIA 6410"         , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 3149, 1106, 0x00, ATA_SA150, "VIA 6420"         , 0      | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 3249, 1106, 0x00, ATA_SA150, "VIA 6421"         , VIABAR | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 0591, 1106, 0x00, ATA_SA150, "VIA 8237A"        , 0      | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 5337, 1106, 0x00, ATA_SA150, "VIA 8237S"        , 0      | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 5372, 1106, 0x00, ATA_SA300, "VIA 8237"         , 0      | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 7372, 1106, 0x00, ATA_SA300, "VIA 8237"         , 0      | UNIATA_SATA                    ),
    PCI_DEV_HW_SPEC_BM( 3349, 1106, 0x00, ATA_SA150, "VIA 8251"         , 0      | UNIATA_SATA | UNIATA_AHCI      ),

    PCI_DEV_HW_SPEC_BM( c693, 1080, 0x00, ATA_WDMA2, "Cypress 82C693"   ,0                                        ),

/*                                                                                     
    PCI_DEV_HW_SPEC_BM( 4d68, 105a, 0, 0, "Promise TX2 ATA-100 controller",          UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6268, 105a, 0, 0, "Promise TX2 ATA-100 controller",          UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 4d69, 105a, 0, 0, "Promise TX2 ATA-133 controller",          UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 5275, 105a, 0, 0, "Promise TX2 ATA-133 controller",          UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 6269, 105a, 0, 0, "Promise TX2 ATA-133 controller",          UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 7275, 105a, 0, 0, "Promise TX2 ATA-133 controller",          UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 4d33, 105a, 0, 0, "Promise Ultra/FastTrak-33 controller",    UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 0d38, 105a, 0, 0, "Promise FastTrak 66 controller",          0),
    PCI_DEV_HW_SPEC_BM( 4d38, 105a, 0, 0, "Promise Ultra/FastTrak-66 controller",    UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 4d30, 105a, 0, 0, "Promise Ultra/FastTrak-100 controller",   UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0d30, 105a, 0, 0, "Promise OEM ATA-100 controllers",         UNIATA_RAID_CONTROLLER),

    PCI_DEV_HW_SPEC_BM( 0004, 1103, 0, 0, "HighPoint HPT366/368/370/372 controller", UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0005, 1103, 0, 0, "HighPoint HPT372 controller",             UNIATA_RAID_CONTROLLER),
    PCI_DEV_HW_SPEC_BM( 0008, 1103, 0, 0, "HighPoint HPT374 controller",             UNIATA_RAID_CONTROLLER),
*/
    PCI_DEV_HW_SPEC_BM( 0001, 16ca, 0x00, ATA_WDMA2, "Cenatek Rocket Drive",0                                     ),

    PCI_DEV_HW_SPEC_BM( 0000, 1078, 0x00, ATA_WDMA2, "Cyrix 5510"       , CYRIX_OLD                               ),
    PCI_DEV_HW_SPEC_BM( 0002, 1078, 0x00, ATA_WDMA2, "Cyrix 5520"       , CYRIX_OLD                               ),
    PCI_DEV_HW_SPEC_BM( 0102, 1078, 0x00, ATA_UDMA2, "Cyrix 5530"       , CYRIX_3x                                ),

    PCI_DEV_HW_SPEC_BM( 1000, 1042, 0x00, ATA_PIO4,  "RZ 100x"          , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 1001, 1042, 0x00, ATA_PIO4,  "RZ 100x"          , 0                                       ),

    PCI_DEV_HW_SPEC_BM( 8172, 1283, 0x00, ATA_UDMA2, "IT8172"           , 0                                       ),
    PCI_DEV_HW_SPEC_BM( 8213, 1283, 0x00, ATA_UDMA6, "IT8213F"          , ITE_133_NEW                             ),
    PCI_DEV_HW_SPEC_BM( 8212, 1283, 0x00, ATA_UDMA6, "IT8212F"          , ITE_133                                 ),
    PCI_DEV_HW_SPEC_BM( 8211, 1283, 0x00, ATA_UDMA6, "IT8211F"          , ITE_133                                 ),

    PCI_DEV_HW_SPEC_BM( 0044, 169c, 0x00, ATA_UDMA2, "Netcell SR3000/5000", 0                                     ),

    PCI_DEV_HW_SPEC_BM( 8013, 3388, 0x00, ATA_DMA,   "HiNT VXII EIDE"   , 0                                       ),

    // Terminator
    PCI_DEV_HW_SPEC_BM( ffff, ffff, 0xff, BMLIST_TERMINATOR, NULL       , BMLIST_TERMINATOR )
};

const ULONG _NUM_BUSMASTER_ADAPTERS = (sizeof(BusMasterAdapters) / sizeof(BUSMASTER_CONTROLLER_INFORMATION_BASE));
