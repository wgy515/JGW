#include "StdAfx.h"
#include "TSE_LoadUIConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSE_LoadUIConfig::CTSE_LoadUIConfig(void)
    {
    }


    CTSE_LoadUIConfig::~CTSE_LoadUIConfig(void)
    {
    }

    bool CTSE_LoadUIConfig::LoadUIConfigToXml(const std::string& strUIConfigXmlPath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot = NULL,*pPluginNode = NULL;

        if ( !ptrXmlDoc->LoadFile(strUIConfigXmlPath.c_str()) )
        {
            return false;
        }
        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        if (!ParesePluginNode(pRoot->FirstChildElement("plugin")))
        {
            return false;
        }

        return true;
    }

    const std::vector<S_PLUGIN_GROUP_INFO>& CTSE_LoadUIConfig::GetPluginGroupInfos()
    {
        return mvsPluginGroupInfo;
    }

    bool CTSE_LoadUIConfig::ParesePluginNode( TiXmlElement* pPluginNode )
    {
        if ( !pPluginNode ) return false;
        TiXmlElement* pGroupNode = pPluginNode->FirstChildElement("group");
        TiXmlElement* pPlnNode = NULL;
        S_PLUGIN_GROUP_INFO sGroupInfo;
        S_PLUGIN_INFO sPluginInfo;
        int id = 1;

        while ( pGroupNode )
        {
            sGroupInfo.mvSubWinInfo.clear();
            GetElementAttributeValue( pGroupNode,"name",sGroupInfo.mstrGroupName );
            JGW::JGW_FormatWString( sGroupInfo.mstrGroupID,L"%d",id++);

            pPlnNode = pGroupNode->FirstChildElement( "pln" );
            while ( pPlnNode )
            {
                if (!GetPlnNodeInfo(pPlnNode,sPluginInfo)) return false;
                sGroupInfo.mvSubWinInfo.push_back(sPluginInfo);
                pPlnNode = pPlnNode->NextSiblingElement("pln");
            }
            mvsPluginGroupInfo.push_back( sGroupInfo );
            pGroupNode = pGroupNode->NextSiblingElement("group");
        }

        return true;
    }

    bool CTSE_LoadUIConfig::GetPlnNodeInfo(TiXmlElement* pPlnNode,S_PLUGIN_INFO& sPluginInfo)
    {
        std::wstring strTemp;
        GetElementAttributeValue( pPlnNode,"stationid",sPluginInfo.mStationID);
        GetElementAttributeValue( pPlnNode,"clsid",sPluginInfo.mstrClsid );
        GetElementAttributeValue( pPlnNode,"viewloader_id",strTemp );
        sPluginInfo.mnViewLoadID = _ttoi( strTemp.c_str() )/* > 0?1:0*/;
        /*if (0 <= sPluginInfo.mnViewLoadID && sPluginInfo.mnViewLoadID <= msUIProjectInfo.msViewLoaderInfo.mvViewLoadKey_Value.size())*/
        GetElementAttributeValue( pPlnNode,"module_folder",sPluginInfo.mstrModuleFolderName );
        GetElementAttributeValue( pPlnNode,"name",sPluginInfo.mstrPluginName );
        GetElementAttributeValue( pPlnNode,"station_name",sPluginInfo.mstrStationName );
        GetElementAttributeValue( pPlnNode,"image_id",strTemp );
        sPluginInfo.mImageGroupID = _ttoi( strTemp.c_str() );
        //! if (sPluginInfo.mImageGroupID >= msUIProjectInfo.mvButtonImage.size()) return false;
        //! sPluginInfo.mpViewLaoderInfo = &msUIProjectInfo.msViewLoaderInfo;

        return true;
    }
}