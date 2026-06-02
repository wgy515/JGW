#pragma once
#include <usb/usbioctl.h>
#include <string>

typedef struct nodelistheader_s   NODELISTHEADER;
typedef NODELISTHEADER                     * PNODELISTHEADER;

typedef struct _STRING_DESCRIPTOR_NODE
{
    struct _STRING_DESCRIPTOR_NODE    *Next;
    UCHAR                           DescriptorIndex;
    USHORT                          LanguageID;
    USB_STRING_DESCRIPTOR           StringDescriptor[0];
} STRING_DESCRIPTOR_NODE, * PSTRING_DESCRIPTOR_NODE;
//
typedef struct
{
    PUSB_NODE_INFORMATION               HubInfo;        // NULL if not a HUB
    LPTSTR                               HubName;        // NULL if not a HUB
    PUSB_NODE_CONNECTION_INFORMATION    ConnectionInfo ; // NULL if root HUB
    PUSB_DESCRIPTOR_REQUEST             ConfigDesc;     // NULL if root HUB
    PSTRING_DESCRIPTOR_NODE             StringDescs;
} USBDEVICEINFO, * PUSBDEVICEINFO;

namespace JGW
{
    typedef struct  
    {
        size_t mnPort;
        std::wstring mstrPhysicalAddress;
    }S_USB_SORT_INFO;

//     typedef struct  
//     {
//         int         m_nPid;
//         int         m_nVid;
//     }PIDANDVIDINFO,* PPIDANDVIDINFO;
}
