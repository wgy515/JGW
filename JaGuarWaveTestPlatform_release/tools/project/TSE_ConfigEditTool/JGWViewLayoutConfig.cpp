#include "stdafx.h"
#include "JGWViewLayoutConfig.h"
#include <tchar.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCJGWViewLayoutConfig::CCJGWViewLayoutConfig(void)
    {
        msUIProjectInfo.mnGroupIndex = 0;
        msUIProjectInfo.mnProjectIndex = 0;
        //! project button
        msUIProjectInfo.msProjectButtonLayout.m_strFontID = _T("1");
        msUIProjectInfo.msProjectButtonLayout.m_strHeight = _T("60");
        msUIProjectInfo.msProjectButtonLayout.m_strWidth = _T("110");
        msUIProjectInfo.msProjectButtonLayout.m_strImageHeight = _T("36");
        msUIProjectInfo.msProjectButtonLayout.m_strLabelHeight = _T("20");
        msUIProjectInfo.msProjectButtonLayout.m_strImageWeight = _T("36");
        msUIProjectInfo.msProjectButtonLayout.m_strLabelTextColor = _T("#FFFFFFFF");
        //! group button
        msUIProjectInfo.msGroupButtonLayout.m_strFontID = _T("1");
#if 1
        msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrNormalImage = (_T("file='button_nor.bmp' corner='5,5,5,5'"));
        msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrHotImage = (_T("file='button_down.bmp' corner='5,5,5,5'"));
        msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrPushedImage = (_T("file='button_down.bmp' corner='5,5,5,5'"));
        msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrFocusedImage = (_T("file='button_over.bmp' corner='5,5,5,5'"));
        msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrDisabledImage = (_T("file='disable_btn.png' corner='5,5,5,5'"));
#endif
        msUIProjectInfo.msGroupButtonLayout.m_strHeight = _T("30");
        msUIProjectInfo.msGroupButtonLayout.m_strTextColor = _T("#FF000000");

        msDevicePoolModuleInfo.mstrClsid = _T("JGW::CCJGW_DevicePoolThread");
        msDevicePoolModuleInfo.mstrModuleName = _T("JGW_DevicePoolModule.dll");

        msWelcomPluginInfo.mstrClsid = L"JGW::CCWelcomViewDlg";
        msWelcomPluginInfo.mnViewLoadID = 0;
        msWelcomPluginInfo.mstrPluginName = L"CWelcomPlugin.dll";
        msWelcomPluginInfo.mpViewLaoderInfo = NULL;
        msWelcomPluginInfo.mStationID = L"-1"; //! 站点ID
        msWelcomPluginInfo.mImageGroupID = 0;
        msWelcomPluginInfo.mstrModuleFolderName = L"CCWelcomViewDlg";//! 模块目录名称
        msWelcomPluginInfo.mstrStationName=L"CWelcomPlugin"; //! 站点按钮名称
    }

    LPS_PLUGIN_INFO CCJGWViewLayoutConfig::GetWelcomPluginInfo()
    {
        return &msWelcomPluginInfo;
    }

    CCJGWViewLayoutConfig::~CCJGWViewLayoutConfig(void)
    {
    }

    PS_UI_PROJECT_INFO CCJGWViewLayoutConfig::GetUIProjectInfo()
    {
        return &msUIProjectInfo;
    }

    LPS_DEVICE_POOL_MODULE_INFO CCJGWViewLayoutConfig::GetDevicePoolModuleInfo()
    {
        return &msDevicePoolModuleInfo;
    }

    const std::map<std::wstring,std::vector<S_EXEC_MENUELEMENT_INFO>>& CCJGWViewLayoutConfig::GetExecMeunInfos()
    {
        return msExecMenuInfos;
    }

    bool CCJGWViewLayoutConfig::LoadViewLayoutConfig( std::string& strViewConfigPath )
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot = NULL,*pPluginNode = NULL;

        if ( !ptrXmlDoc->LoadFile(strViewConfigPath.c_str()) )
        {
            return false;
        }
        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        if (!PareseUINode(pRoot->FirstChildElement("ui")) 
            || !ParesePluginNode(pRoot->FirstChildElement("plugin")))
        {
            return false;
        }

        return true;
    }


    void CCJGWViewLayoutConfig::SaveViewLayoutConfig(std::string& strViewConfigPath/*,size_t nPrevPluginInfoIndex*/)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pUINode,*pPrevPluginInfoNode = NULL,*pProjectButtonNode = NULL;
        if (!ptrXmlDoc->LoadFile(strViewConfigPath.c_str())) return ;

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return ;

        pUINode = pRoot->FirstChildElement("ui");
        if (!pUINode) return ;

        pPrevPluginInfoNode = pUINode->FirstChildElement("prev_plugin_info_index");
        if (!pPrevPluginInfoNode) return ;

        std::wstring strTemp;

        JGW_FormatWString(strTemp,L"%d",msUIProjectInfo.mnProjectIndex);
        SetElementAttributeValue(pPrevPluginInfoNode,"project_index",strTemp);

        JGW_FormatWString(strTemp,L"%d",msUIProjectInfo.mnGroupIndex);
        SetElementAttributeValue(pPrevPluginInfoNode,"group_index",strTemp);

        pProjectButtonNode = pUINode->FirstChildElement("project_button");
        if (pProjectButtonNode)
        {
            SetElementTextValue(pProjectButtonNode,"width",msUIProjectInfo.msProjectButtonLayout.m_strWidth);
        }

        ptrXmlDoc->SaveFile(strViewConfigPath);
    }
	
    bool CCJGWViewLayoutConfig::PareseUINode( TiXmlElement* pUINode )
    {
        if ( !pUINode ) return false;
        TiXmlElement* pProjectButtonNode = NULL,*pGroupButtonNode = NULL,*pButtonGroupImage = NULL,*pViewLoaderNode = NULL,*pDevicePoolNode = NULL;

        //! project_button
        {
            TiXmlElement* pPrevPluginInfoNode = pUINode->FirstChildElement("prev_plugin_info_index");
            std::wstring strTemp;
            if (pPrevPluginInfoNode)
            {
                GetElementAttributeValue(pPrevPluginInfoNode,"project_index",strTemp);
                msUIProjectInfo.mnProjectIndex = _ttoi(strTemp.c_str());

                GetElementAttributeValue(pPrevPluginInfoNode,"group_index",strTemp);
                msUIProjectInfo.mnGroupIndex = _ttoi(strTemp.c_str());
            }    
        }
        //! home info
        {
            TiXmlElement* pHomeNode = pUINode->FirstChildElement("home");
            GetPlnNodeInfo(pHomeNode,msWelcomPluginInfo);
        }
        //! EXEC_Menu
        {
            TiXmlElement* pExecMenuNode = pUINode->FirstChildElement("ExecMenu");
            std::wstring strName,strTempValue;
            S_EXEC_MENUELEMENT_INFO sEexMenuElementInfo;
            while (pExecMenuNode)
            {
                GetElementAttributeValue(pExecMenuNode,"name",strName,L"toolBtn");
                TiXmlElement* pExecMenuElementNode = pExecMenuNode->FirstChildElement("MenuElement");
                while (pExecMenuElementNode)
                {
                    GetElementAttributeValue(pExecMenuElementNode,"show",strTempValue,L"1");
                    sEexMenuElementInfo.mnShow = _ttoi(strTempValue.c_str());
                    GetElementAttributeValue(pExecMenuElementNode,"name",sEexMenuElementInfo.mstrMenuElementName);
                    GetElementAttributeValue(pExecMenuElementNode,"text",sEexMenuElementInfo.mstrMenuElementText);
                    GetElementAttributeValue(pExecMenuElementNode,"exec",strTempValue);
                    sEexMenuElementInfo.mstrMenuElementExec = JGW_RealativePathToAbsPath(strTempValue.c_str());
                    msExecMenuInfos[strName].push_back(sEexMenuElementInfo);
                    pExecMenuElementNode = pExecMenuElementNode->NextSiblingElement("MenuElement");
                }
                pExecMenuNode = pExecMenuNode->NextSiblingElement("ExecMenu");
            }
        }
        //! project_button
        {
            pProjectButtonNode = pUINode->FirstChildElement("project_button");
            if ( !pProjectButtonNode ) return false;

            GetElementTextValue( pProjectButtonNode,"font_id",msUIProjectInfo.msProjectButtonLayout.m_strFontID );
            GetElementTextValue( pProjectButtonNode,"image_height",msUIProjectInfo.msProjectButtonLayout.m_strImageHeight );
            GetElementTextValue( pProjectButtonNode,"image_weight",msUIProjectInfo.msProjectButtonLayout.m_strImageWeight );
            GetElementTextValue( pProjectButtonNode,"label_height",msUIProjectInfo.msProjectButtonLayout.m_strLabelHeight );
            GetElementTextValue(pProjectButtonNode,"height",msUIProjectInfo.msProjectButtonLayout.m_strHeight);
            GetElementTextValue(pProjectButtonNode,"width",msUIProjectInfo.msProjectButtonLayout.m_strWidth);
            GetElementTextValue( pProjectButtonNode,"label_textcolor",msUIProjectInfo.msProjectButtonLayout.m_strLabelTextColor );
        }
        //! group_button
        {
            pGroupButtonNode = pUINode->FirstChildElement("group_button");
            if ( !pGroupButtonNode ) return false;

            GetElementTextValue( pGroupButtonNode,"font_id",msUIProjectInfo.msGroupButtonLayout.m_strFontID );
            GetElementTextValue( pGroupButtonNode,"normal_image",msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrNormalImage );
            GetElementTextValue( pGroupButtonNode,"focused_image",msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrFocusedImage );
            GetElementTextValue( pGroupButtonNode,"disable_image",msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrDisabledImage );
            GetElementTextValue( pGroupButtonNode,"hot_image",msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrHotImage );
            GetElementTextValue( pGroupButtonNode,"pushed_image",msUIProjectInfo.msGroupButtonLayout.mSButtonImage.mstrPushedImage );
            GetElementTextValue( pGroupButtonNode,"height",msUIProjectInfo.msGroupButtonLayout.m_strHeight );
            GetElementTextValue( pGroupButtonNode,"textcolor",msUIProjectInfo.msGroupButtonLayout.m_strTextColor );
        }
        //! button_image_group
        {
            pButtonGroupImage = pUINode->FirstChildElement("button_image_group");
            if ( !pButtonGroupImage ) return false;

            TiXmlElement* pImageNode = pButtonGroupImage->FirstChildElement("image");
            S_BUTTON_IMAGE_ sButtonImage;
            while (pImageNode)
            {
                GetElementTextValue( pImageNode,"normal_image",sButtonImage.mstrNormalImage );
                GetElementTextValue( pImageNode,"focused_image",sButtonImage.mstrFocusedImage );
                GetElementTextValue( pImageNode,"disable_image",sButtonImage.mstrDisabledImage );
                GetElementTextValue( pImageNode,"hot_image",sButtonImage.mstrHotImage );
                GetElementTextValue( pImageNode,"pushed_image",sButtonImage.mstrPushedImage );
                msUIProjectInfo.mvButtonImage.push_back(sButtonImage);
                pImageNode = pImageNode->NextSiblingElement("image");
            }
        }
        //! device pool module info
        {
            pDevicePoolNode = pUINode->FirstChildElement("device_pool");
            if (pDevicePoolNode)
            {
                TiXmlElement* pPluginNode = pDevicePoolNode->FirstChildElement("plugin");
                if (pPluginNode)
                {
                    GetElementAttributeValue(pPluginNode,"clsid",msDevicePoolModuleInfo.mstrClsid,L"JGW::CCJGW_DevicePoolThread");
                    GetElementAttributeValue(pPluginNode,"name",msDevicePoolModuleInfo.mstrModuleName,L"JGW_DevicePoolModule.dll");
                }
            }
        }

        //! view loader info
        {
            pViewLoaderNode = pUINode->FirstChildElement("viewloader");
            if (!pViewLoaderNode) return true;
            S_VIEW_LOAD_KEY_VALUE sViewKey_Value;
            std::wstring strTemp;
            DWORD dwID = 0;

            TiXmlElement* pPluginNode = pViewLoaderNode->FirstChildElement("plugin");
            while (pPluginNode)
            {
                GetElementAttributeValue( pPluginNode,"name",sViewKey_Value.mstrViewLoaderPluginName );
                sViewKey_Value.mID_ClsidMap.clear();

                TiXmlElement* pClsidNode = pPluginNode->FirstChildElement("clsid");
                while ( pClsidNode )
                {
                    GetElementAttributeValue( pClsidNode,"id",strTemp );
                    dwID = _ttoi( strTemp.c_str() );
                    GetElementAttributeValue( pClsidNode,"value",strTemp );
                    sViewKey_Value.mID_ClsidMap[dwID] = strTemp;
                    pClsidNode = pClsidNode->NextSiblingElement("clsid");
                }
                msUIProjectInfo.msViewLoaderInfo.mvViewLoadKey_Value.push_back(sViewKey_Value);
                pPluginNode = pPluginNode->NextSiblingElement("plugin");
            }
        }

        return true;
    }

    bool CCJGWViewLayoutConfig::GetPlnNodeInfo(TiXmlElement* pPlnNode,S_PLUGIN_INFO& sPluginInfo)
    {
        std::wstring strTemp;
        GetElementAttributeValue( pPlnNode,"stationid",sPluginInfo.mStationID);
        GetElementAttributeValue( pPlnNode,"clsid",sPluginInfo.mstrClsid );
        GetElementAttributeValue( pPlnNode,"viewloader_id",strTemp );
        sPluginInfo.mnViewLoadID = _ttoi( strTemp.c_str() )/* > 0?1:0*/;
        /*if (0 <= sPluginInfo.mnViewLoadID && sPluginInfo.mnViewLoadID <= msUIProjectInfo.msViewLoaderInfo.mvViewLoadKey_Value.size())*/
        GetElementAttributeValue( pPlnNode,"module_folder",sPluginInfo.mstrModuleFolderName );
        GetElementAttributeValue(pPlnNode,"suite_name",sPluginInfo.mstrSuiteXmlName,L"suite.xml");
        GetElementAttributeValue( pPlnNode,"name",sPluginInfo.mstrPluginName );
        GetElementAttributeValue( pPlnNode,"station_name",sPluginInfo.mstrStationName );
        GetElementAttributeValue( pPlnNode,"image_id",strTemp );
        sPluginInfo.mImageGroupID = _ttoi( strTemp.c_str() );
        if (sPluginInfo.mImageGroupID >= msUIProjectInfo.mvButtonImage.size()) return false;
        sPluginInfo.mpViewLaoderInfo = &msUIProjectInfo.msViewLoaderInfo;

        Log4WD_F(_T("CCJGWViewLayoutConfig::ParesePluginNode\t\n\t\tstationid:%s,\n\t\tImageGroupID:%d,\n\t\tUseViewLoader:%d,\n\t\tclsid:%s,\n\t\tModuleFolderName:%s,\n\t\tStationName:%s,\n\t\tPluginName:%s"),sPluginInfo.mStationID.c_str(),sPluginInfo.mImageGroupID,sPluginInfo.mnViewLoadID,\
            sPluginInfo.mstrClsid.c_str(),\
            sPluginInfo.mstrModuleFolderName.c_str(),\
            sPluginInfo.mstrStationName.c_str(),\
            sPluginInfo.mstrPluginName.c_str()\
            );
        return true;
    }

    bool CCJGWViewLayoutConfig::ParesePluginNode( TiXmlElement* pPluginNode )
    {
        if ( !pPluginNode ) return false;
        TiXmlElement* pProjectNode = pPluginNode->FirstChildElement("project");
        // load project node
        while (pProjectNode)
        {
            S_PROJECT_GROUP_PLUGIN_INFO sProjectGroupPluginInfo;
            GetElementAttributeValue(pProjectNode,"name",sProjectGroupPluginInfo.mstrProjectName);
            GetElementAttributeValue(pProjectNode,"menu_name",sProjectGroupPluginInfo.mstrProjectMenuName);
            TiXmlElement* pGroupNode = pProjectNode->FirstChildElement("group");
            TiXmlElement* pPlnNode = NULL;
            S_PLUGIN_GROUP_INFO sGroupInfo;
            S_PLUGIN_INFO sPluginInfo;
            int id = 1;
            //! load group node
            while (pGroupNode)
            {
                sGroupInfo.mvSubWinInfo.clear();
                GetElementAttributeValue( pGroupNode,"name",sGroupInfo.mstrGroupName );
                JGW::JGW_FormatWString( sGroupInfo.mstrGroupID,L"%d",id++);
                //! load pln node
                pPlnNode = pGroupNode->FirstChildElement( "pln" );
                while ( pPlnNode )
                {
                    if (!GetPlnNodeInfo(pPlnNode,sPluginInfo)) return false;
                    sGroupInfo.mvSubWinInfo.push_back(sPluginInfo);
                    pPlnNode = pPlnNode->NextSiblingElement("pln");
                }
                sProjectGroupPluginInfo.mvAllPluginInfo.push_back( sGroupInfo );
                pGroupNode = pGroupNode->NextSiblingElement("group");
            }

            pProjectNode = pProjectNode->NextSiblingElement("project");
            msUIProjectInfo.mvProjectGroupPluginInfo.push_back(sProjectGroupPluginInfo);
        }

        if (msUIProjectInfo.mvProjectGroupPluginInfo.size() > msUIProjectInfo.mnProjectIndex)
        {
            msUIProjectInfo.mvAllPluginInfo.insert(msUIProjectInfo.mvAllPluginInfo.begin(),msUIProjectInfo.mvProjectGroupPluginInfo[msUIProjectInfo.mnProjectIndex].mvAllPluginInfo.begin(),msUIProjectInfo.mvProjectGroupPluginInfo[msUIProjectInfo.mnProjectIndex].mvAllPluginInfo.end());
        }
        else
        {
            msUIProjectInfo.mnProjectIndex = 0;
            if (!msUIProjectInfo.mvProjectGroupPluginInfo.empty() && !msUIProjectInfo.mvProjectGroupPluginInfo[0].mvAllPluginInfo.empty())
                msUIProjectInfo.mvAllPluginInfo.insert(msUIProjectInfo.mvAllPluginInfo.begin(),msUIProjectInfo.mvProjectGroupPluginInfo[0].mvAllPluginInfo.begin(),msUIProjectInfo.mvProjectGroupPluginInfo[0].mvAllPluginInfo.end());
        }

        return true;
    }
}
