#include "StdAfx.h"
#include "CLuckyDrawDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_ConfigIni.h"
#include <fstream>
namespace JGW
{
    CCLuckyDrawDlg::CCLuckyDrawDlg(void)
    {
    }


    CCLuckyDrawDlg::~CCLuckyDrawDlg(void)
    {
    }

    void CCLuckyDrawDlg::OnInitWindow()
    {
        LoadDrawLuckyConfig();
        mpLuckyDrawVerticalLayout = static_cast<CListUI*>(m_PaintManager.FindControl(L"LuckyDrawVerticalLayout"));
        mpSaveLossBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"saveLossBtn"));
    }

    void CCLuckyDrawDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpSaveLossBtn)
            {
                OnReDrawLucky();
            }

            if (0 == msg.pSender->GetName().Find(L"LuckyDrawButton") && 0 == JGW_WStrComparenoCaseWStr(L"ButtonUI",msg.pSender->GetClass()))
            {
                OnSelectDrawLucky((CButtonUI*)msg.pSender);
            }
        }
    }

    void CCLuckyDrawDlg::OnCloseWindow()
    {

    }


    void csvline_populate(std::vector<std::wstring> &record, const std::wstring& line, wchar_t delimiter)
    {
        int linepos=0;
        int inquotes=false;
        wchar_t c;
        int linemax=line.length();
        std::wstring curstring;
        record.clear();

        while(line[linepos]!=0 && linepos < linemax)
        {

            c = line[linepos];

            if (!inquotes && curstring.length()==0 && c==L'"')
            {
                //beginquotechar
                inquotes=true;
            }
            else if (inquotes && c==L'"')
            {
                //quotechar
                if ( (linepos+1 <linemax) && (line[linepos+1]==L'"') )
                {
                    //encountered 2 double quotes in a row (resolves to 1 double quote)
                    curstring.push_back(c);
                    linepos++;
                }
                else
                {
                    //endquotechar
                    inquotes=false;
                }
            }
            else if (!inquotes && c==delimiter)
            {
                //end of field
                record.push_back( curstring );
                curstring=L"";
            }
            else if (!inquotes && (c==L'\r' || c==L'\n') )
            {
                record.push_back( curstring );
                return;
            }
            else
            {
                curstring.push_back(c);
            }
            linepos++;
        }
        record.push_back( curstring );
        return;
    }

    void CCLuckyDrawDlg::LoadDrawLuckyConfig()
    {
        CCJGW_ConfigIni configINI;

        std::wstring strConfigFilePath;
        JGW_FormatWString(strConfigFilePath,L"%slucky_draw.config",JGW_GetApplicationFolder());
        configINI.InitIniFilePath(strConfigFilePath.c_str());

        //msLuckyDrawConfig.mnPeopleNumber = configINI.GetIniKeyIntValue(L"MAIN",L"PeopleNumber",10);
        msLuckyDrawConfig.mstrDataFilePath = JGW_RealativePathToAbsPath(configINI.GetIniKeyValue(L"MAIN",L"Data",L"data_excel.xlsx").c_str());
        /*msLuckyDrawConfig.mnCardWidth = configINI.GetIniKeyIntValue(L"MAIN",L"CardWidth",40);*/
#if 0
        msLuckyDrawConfig.mnPerRow = configINI.GetIniKeyIntValue(L"MAIN",L"PerRow",7);
        msLuckyDrawConfig.mnPerCol = configINI.GetIniKeyIntValue(L"MAIN",L"PerCol",20);
#else
        msLuckyDrawConfig.mnColCount = configINI.GetIniKeyIntValue(L"MAIN",L"ColCount",0);
        msLuckyDrawConfig.mnCardWidth = configINI.GetIniKeyIntValue(L"MAIN",L"CardWidth",120);
        msLuckyDrawConfig.mnCardHeight = configINI.GetIniKeyIntValue(L"MAIN",L"CardHeight",180);
#endif
        std::wifstream inputFile;
        inputFile.imbue(std::locale(""));
        inputFile.open(msLuckyDrawConfig.mstrDataFilePath.c_str());
        if (!inputFile.is_open())
        {
            MessageBox(m_PaintManager.GetPaintWindow(),L"Load Data File Fail",L"Error",MB_ICONERROR);
            Close();
            return;
        }

        wchar_t szBuf[2048] = {0};
        size_t desIndex = -1,value2Index = -1,line = 0;
        while (inputFile.getline(szBuf,2048))
        {
            std::vector<std::wstring> vTemp;
            csvline_populate(vTemp, szBuf, L',');
            //JGW_ParserStrW(szBuf,L",",vTemp);
            if (0 == line)
            {
                for (size_t i = 0;i < vTemp.size();i ++)
                {
                    if(0 == JGW_WStrComparenoCaseWStr(L"Description",vTemp[i].c_str()))
                    {
                        desIndex = i;
                    }
                    if(0 == JGW_WStrComparenoCaseWStr(L"Value2",vTemp[i].c_str()))
                    {
                        value2Index = i;
                    }
                }
                line ++;
            }
            else
            {
                S_DATA_INFO sDataInfo;
                if (desIndex >= 0 && desIndex < vTemp.size())
                {
                    sDataInfo.mstrDescription = vTemp[desIndex];
                }
                if (value2Index >= 0 && value2Index < vTemp.size())
                {
                    sDataInfo.mstrValue2 = vTemp[value2Index];
                }
                msLuckyDrawConfig.mvsDataInfo.push_back(sDataInfo);
            }
        }

        /*CCJGW_ExcelLibXL excelLibXL;
        if (!excelLibXL.OpenExcel(msLuckyDrawConfig.mstrDataFilePath))
        {
        MessageBox(m_PaintManager.GetPaintWindow(),L"Load Excel Fail",L"Error",MB_ICONERROR);
        Close();
        return;
        }

        size_t row = excelLibXL.GetSheetMaxRow();
        size_t col = excelLibXL.GetSheetMaxCol();
        for (size_t x = 1;x < row;x++)
        {
        S_DATA_INFO sDataInfo;
        for (size_t y = 0;y < col;y ++)
        {
        if (0 == y)
        {
        sDataInfo.mstrDescription = excelLibXL.ReadExcelContent(x,y);
        }
        else if (1 == y)
        {
        sDataInfo.mstrValue2 = excelLibXL.ReadExcelContent(x,y);
        }   
        }
        msLuckyDrawConfig.mvsDataInfo.push_back(sDataInfo);
        }*/
        msLuckyDrawConfig.mnPeopleNumber = static_cast<int>(msLuckyDrawConfig.mvsDataInfo.size());
    }

    void CCLuckyDrawDlg::OnReDrawLucky()
    {
        mvSmokingList.clear();
        mpLuckyDrawVerticalLayout->GetList()->RemoveAll();
        mpLuckyDrawVerticalLayout->GetHeader()->RemoveAll();
#if 0
        int col = msLuckyDrawConfig.mnPerCol/*mpLuckyDrawVerticalLayout->GetWidth() / msLuckyDrawConfig.mnCardWidth*/;
        int row = msLuckyDrawConfig.mnPerRow/*(0 == msLuckyDrawConfig.mnPeopleNumber % col) ? msLuckyDrawConfig.mnPeopleNumber/col : msLuckyDrawConfig.mnPeopleNumber/col + 1*/;
        int index = 1;
        std::wstring strTemp;
        for (int x = 0;x < row;x ++)
        {
            CHorizontalLayoutUI* pHorizontal = new CHorizontalLayoutUI;
            pHorizontal->SetManager(&m_PaintManager,mpLuckyDrawVerticalLayout);
            mpLuckyDrawVerticalLayout->Add(pHorizontal);
            for (int y = 0;y < col;y++)
            {
                if (index <= msLuckyDrawConfig.mnPeopleNumber)
                {
                    CButtonUI* pButton = new CButtonUI;
                    pButton->SetManager(&m_PaintManager,pHorizontal);
                    pButton->SetBkImage(L"project_front_bg.png");
                    pButton->SetNormalImage(L"project_front_bg.png");
                    pButton->SetHotImage(L"project_front_bg.png");
                    pButton->SetPushedImage(L"project_front_bg.png");
                    pButton->SetAttribute(L"padding",L"5,5,5,5");
                    //pButton->SetMaxHeight(msLuckyDrawConfig.mnCardWidth * 2);
                    JGW_FormatWString(strTemp,L"LuckyDrawButton%d",index++);
                    pButton->SetName(strTemp.c_str());
                    pHorizontal->Add(pButton);
                }
                else
                {
                    CControlUI* pControl = new CControlUI;
                    pControl->SetAttribute(L"padding",L"5,5,5,5");
                    pHorizontal->Add(pControl);
                }
            }
        }
#else
        int col = 0,row = 0;
        if (msLuckyDrawConfig.mnColCount > 0)
        {
            col = msLuckyDrawConfig.mnColCount;
        }
        else
        {
            col = (mpLuckyDrawVerticalLayout->GetWidth() - 30) / (msLuckyDrawConfig.mnCardWidth + 10);
        }
        row = (0 == msLuckyDrawConfig.mnPeopleNumber % col) ? msLuckyDrawConfig.mnPeopleNumber/col : msLuckyDrawConfig.mnPeopleNumber/col + 1;
        int index = 1;
        std::wstring strTemp;
#if 0
        for (int x = 0;x < row;x ++)
        {
            CHorizontalLayoutUI* pHorizontal = new CHorizontalLayoutUI;
            pHorizontal->SetManager(&m_PaintManager,mpLuckyDrawVerticalLayout);
            pHorizontal->SetFixedHeight(msLuckyDrawConfig.mnCardHeight + 10);
            mpLuckyDrawVerticalLayout->Add(pHorizontal);
            for (int y = 0;y < col;y++)
            {
                if (index <= msLuckyDrawConfig.mnPeopleNumber)
                {
                    CButtonUI* pButton = new CButtonUI;
                    pButton->SetManager(&m_PaintManager,pHorizontal);
                    pButton->SetBkImage(L"project_front_bg.png");
                    pButton->SetNormalImage(L"project_front_bg.png");
                    pButton->SetHotImage(L"project_front_bg.png");
                    pButton->SetPushedImage(L"project_front_bg.png");
                    pButton->SetAttribute(L"padding",L"5,5,5,5");
                    pButton->SetFixedWidth(msLuckyDrawConfig.mnCardWidth + 10);
                    JGW_FormatWString(strTemp,L"LuckyDrawButton%d",index++);
                    pButton->SetName(strTemp.c_str());
                    pHorizontal->Add(pButton);
                }
                else
                {
                    CControlUI* pControl = new CControlUI;
                    pControl->SetAttribute(L"padding",L"5,5,5,5");
                    pHorizontal->Add(pControl);
                }
            }
        }
        mpLuckyDrawVerticalLayout->SetFixedHeight((row) * msLuckyDrawConfig.mnCardHeight);
#else
        for (int x = 0;x < row;x ++)
        {
            CListContainerElementUI* pHorizontal = new CListContainerElementUI;
            pHorizontal->SetManager(&m_PaintManager,mpLuckyDrawVerticalLayout);
            pHorizontal->SetFixedHeight(msLuckyDrawConfig.mnCardHeight + 10);
            CLabelUI* pLabelUI = new CLabelUI;
            pLabelUI->SetTextColor(0xFFFF0000);
            JGW_FormatWString(strTemp,L"%d",x + 1);
            pLabelUI->SetText(strTemp.c_str());
            pLabelUI->SetAttribute(L"width",L"30");
            //! align="center"
            pLabelUI->SetAttribute(L"align",L"center");
            mpLuckyDrawVerticalLayout->Add(pHorizontal);
            pHorizontal->Add(pLabelUI);
            for (int y = 0;y < col;y++)
            {
                if (index <= msLuckyDrawConfig.mnPeopleNumber)
                {
                    CButtonUI* pButton = new CButtonUI;
                    pButton->SetManager(&m_PaintManager,pHorizontal);
                    pButton->SetBkImage(L"project_front_bg.png");
                    pButton->SetNormalImage(L"project_front_bg.png");
                    pButton->SetHotImage(L"project_front_bg.png");
                    pButton->SetPushedImage(L"project_front_bg.png");
                    pButton->SetAttribute(L"padding",L"5,5,5,5");
                    //!pButton->SetFixedWidth(msLuckyDrawConfig.mnCardWidth + 10);
                    JGW_FormatWString(strTemp,L"LuckyDrawButton%d",index++);
                    pButton->SetName(strTemp.c_str());
                   // JGW_FormatWString(strTemp,L"%d",index - 1);
                    //pButton->SetText(strTemp.c_str());
                    pButton->SetDate(pHorizontal);
                    pHorizontal->Add(pButton);
                }
                else
                {
                    CControlUI* pControl = new CControlUI;
                    pControl->SetAttribute(L"padding",L"5,5,5,5");
                    pHorizontal->Add(pControl);
                }
            }
        }
#endif
#endif


    }

    bool CCLuckyDrawDlg::FindWinIDIsSmoking(int winID)
    {
        for (size_t i = 0;i < mvSmokingList.size();i ++)
        {
            if (winID == mvSmokingList[i])
            {
                return true;
            }
        }
        return false;
    }

    int CCLuckyDrawDlg::GetWinID()
    {
        int winid = 0;
        do 
        {
            winid = rand() % msLuckyDrawConfig.mnPeopleNumber + 1;    
        } while (FindWinIDIsSmoking(winid));
        return winid;
    }

    void CCLuckyDrawDlg::OnSelectDrawLucky(CButtonUI* pButton)
    {
#if 0
        std::wstring strTemp;
        pButton->SetBkImage(L"project_back_bg.png");
        pButton->SetNormalImage(L"project_back_bg.png");
        pButton->SetHotImage(L"project_back_bg.png");
        pButton->SetPushedImage(L"project_back_bg.png");
        pButton->SetDisabledImage(L"project_back_bg.png");
        pButton->SetEnabled(false);
        pButton->SetTextColor(0xFFFF0000);
        int windID = GetWinID();
        JGW_FormatWString(strTemp,L"$21-LUMBER R12 thr Nov Store Sales is %d",windID);
        mvSmokingList.push_back(windID);
        pButton->SetText(strTemp.c_str());
        pButton->SetFont(1);
#else
        CListContainerElementUI* pHorizontal = (CListContainerElementUI*)pButton->GetDate();
        int index = pHorizontal->GetItemIndex(pButton);
        CDialogBuilder builder;
        CVerticalLayoutUI* pVerticalLayoutUI = (CVerticalLayoutUI*)builder.Create(L"smoking_image.xml",NULL,NULL,&m_PaintManager);
        int windID = GetWinID();
        if (windID > 0 && windID < msLuckyDrawConfig.mnPeopleNumber)
        {
            CRichEditUI* pRichEditUI = (CRichEditUI*)(pVerticalLayoutUI->GetItemAt(1));
            pRichEditUI->SetText(msLuckyDrawConfig.mvsDataInfo[windID - 1].mstrDescription.c_str());
            CLabelUI* pLabelUI = (CLabelUI*)(pVerticalLayoutUI->GetItemAt(2));
            pLabelUI->SetText(msLuckyDrawConfig.mvsDataInfo[windID - 1].mstrValue2.c_str());
        }
        pHorizontal->RemoveAt(index);
        pHorizontal->AddAt(pVerticalLayoutUI,index);
        pVerticalLayoutUI->SetAttribute(L"padding",L"5,5,5,5");
#endif
    }

}

