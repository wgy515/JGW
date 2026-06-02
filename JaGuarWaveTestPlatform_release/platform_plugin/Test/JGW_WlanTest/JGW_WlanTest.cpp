// JGW_WlanTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <JGW_Wlan/CJGW_WlanHandle.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

const std::wstring wlan_profile = L"<?xml version=\"1.0\"?>\
	<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
	<name>JW_MIFI_60G_41C2</name>\
	<SSIDConfig>\
	<SSID>\
	<hex>4A575F4D4946495F3630475F34314332</hex>\
	<name>JW_MIFI_60G_41C2</name>\
	</SSID>\
	</SSIDConfig>\
	<connectionType>ESS</connectionType>\
	<connectionMode>manual</connectionMode>\
	<MSM>\
	<security>\
	<authEncryption>\
	<authentication>WPA2PSK</authentication>\
	<encryption>AES</encryption>\
	<useOneX>false</useOneX>\
	</authEncryption>\
	<sharedKey>\
	<keyType>passPhrase</keyType>\
	<protected>true</protected>\
	<keyMaterial>01000000D08C9DDF0115D1118C7A00C04FC297EB010000008FC73F4EE9F30A4E8D02B585848CEE1900000000020000000000106600000001000020000000E841AA5CBCF28D236453E22F5CD8445668D2AAC31DACBA4DAA78BF913A14D485000000000E80000000020000200000003ECD677B04816204140ED6693B0EC315ED689598A355D432164ACEE65865F00C1000000007B5FD4DB9EFFBA65E50134AB6095148400000001D0713FA227A4D392B86D2C3513744A2EF001D3996D318BC31B6499E872FAA347BA26A669CF83A014F1A6F5DF30B560BA31B8D9D4FB97F495D2519474C49ED7D</keyMaterial>\
	</sharedKey>\
	</security>\
	</MSM>\
	<MacRandomization xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v3\">\
	<enableRandomization>false</enableRandomization>\
	</MacRandomization>\
	</WLANProfile>";

const std::wstring gstrWlanProFile = L"<?xml version=\"1.0\"?>\
	<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
	<name>JW_MIFI_60G_41C2</name>\
	<SSIDConfig>\
	<SSID>\
	<name>%s</name>\
	</SSID>\
	</SSIDConfig>\
	<connectionType>ESS</connectionType>\
	<connectionMode>manual</connectionMode>\
	<MSM>\
	<security>\
	<authEncryption>\
	<authentication>WPA2PSK</authentication>\
	<encryption>AES</encryption>\
	<useOneX>false</useOneX>\
	</authEncryption>\
	<sharedKey>\
	<keyType>passPhrase</keyType>\
	<protected>false</protected>\
	<keyMaterial>%s</keyMaterial>\
	</sharedKey>\
	</security>\
	</MSM>\
	<MacRandomization xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v3\">\
	<enableRandomization>false</enableRandomization>\
	</MacRandomization>\
	</WLANProfile>";

int _tmain(int argc, _TCHAR* argv[])
{
	JGW::CCJGW_WlanHandle wlan;
	std::wstring strGUID = wlan.FindWalnGUIDToInterfaceDescriptionKeyword(L"Peraso");
	std::wstring strProfile ;
	std::map<std::wstring,int> mapBss;

	wlan.GetWlanBssList(strGUID,mapBss);

	JGW::JGW_FormatWString(strProfile,gstrWlanProFile.c_str(),L"JW_MIFI_60G_41C2",L"23461768");

	wlan.SetWlanProfile(strGUID,strProfile);
	wlan.ConnectWlan(strGUID,L"JW_MIFI_60G_41C2",L"JW_MIFI_60G_41C2",mapBss[L"JW_MIFI_60G_41C2"]);

	wlan.DisConnectWlan(strGUID);
	return 0;
}

