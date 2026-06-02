#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    enum NVMEM_TEMPLATE
    {
        // Token: 0x040001F1 RID: 497
        templateNone,
        // Token: 0x040001F2 RID: 498
        templateQC98xx = 20
    };

    // Token: 0x0200004F RID: 79
    enum NVMEM_SIZE
    {
        // Token: 0x04000201 RID: 513
        MEM_AUTO,
        // Token: 0x04000202 RID: 514
        MEM_1024,
        // Token: 0x04000203 RID: 515
        MEM_2048,
        // Token: 0x04000204 RID: 516
        MEM_4096,
        // Token: 0x04000205 RID: 517
        MEM_8192
    };

    enum NVMEM_SECTION
    {
        // Token: 0x04000207 RID: 519
        SECTION_NONE = -1,
        // Token: 0x04000208 RID: 520
        SECTION_ALL,
        // Token: 0x04000209 RID: 521
        SECTION_ID,
        // Token: 0x0400020A RID: 522
        SECTION_MAC,
        // Token: 0x0400020B RID: 523
        SECTION_CAL2G_OLPC,
        // Token: 0x0400020C RID: 524
        SECTION_CAL5G_OLPC,
        // Token: 0x0400020D RID: 525
        SECTION_CTL2G,
        // Token: 0x0400020E RID: 526
        SECTION_CTL5G,
        // Token: 0x0400020F RID: 527
        SECTION_CONFIG,
        // Token: 0x04000210 RID: 528
        SECTION_CUSTOM,
        // Token: 0x04000211 RID: 529
        SECTION_VIDPID,
        // Token: 0x04000212 RID: 530
        SECTION_SDIOPID,
        // Token: 0x04000213 RID: 531
        SECTION_XTAL,
        // Token: 0x04000214 RID: 532
        SECTION_RXCAL5G,
        // Token: 0x04000215 RID: 533
        SECTION_RXCAL2G,
        // Token: 0x04000216 RID: 534
        SECTION_CAL2G_CLPC,
        // Token: 0x04000217 RID: 535
        SECTION_CAL5G_CLPC
    };

    class CTSE_WlanMemCommit : public CTSE_TestBase
    {
    public:
        CTSE_WlanMemCommit(void);
        ~CTSE_WlanMemCommit(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        NVMEM_TEMPLATE meTemplate;
        NVMEM_OPTION meWriteOption;
        NVMEM_SIZE meMemSize;
        bool mbCompress;
        bool mbOverwrite;
        std::string mstrSection;
        std::vector<int> mvSectionId;
    };

}

