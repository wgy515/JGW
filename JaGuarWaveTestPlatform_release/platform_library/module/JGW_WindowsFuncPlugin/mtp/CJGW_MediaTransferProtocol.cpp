#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_MediaTransferProtocol.h>
#include <new>

#define CLIENT_NAME         L"JGW_WindowsFuncPlugin Application"
#define CLIENT_MAJOR_VER    1
#define CLIENT_MINOR_VER    0
#define CLIENT_REVISION     2
#define NUM_OBJECTS_TO_REQUEST  10

namespace JGW
{
    CCJGW_MediaTransferProtocol::CCJGW_MediaTransferProtocol(void)
    {
        HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    }


    CCJGW_MediaTransferProtocol::~CCJGW_MediaTransferProtocol(void)
    {
        //CloseMTPDevice();
        CoUninitialize();
    }

    //     void CCJGW_MediaTransferProtocol::CloseMTPDevice()
    //     {
    //         if (mpDevice) mpDevice->Release();
    //         mpDevice = NULL;
    //     }

    bool JGW_CompareString(const std::wstring& strSrc,const wchar_t* strDes)
    {
        if (!strDes) return true;
        return (NULL == strSrc.compare(strDes));
    }

    bool CCJGW_MediaTransferProtocol::GetMTPDevices(const wchar_t* strManufacturer,const wchar_t* strDescription)
    {
        DWORD pnpDeviceIDCount = 0;
        bool bResult = false;
        IPortableDeviceManager* deviceManager = NULL;
        IPortableDeviceValues* clientInformation = NULL;

        GetClientInformation(&clientInformation);
        //! 创建Device Manager
        HRESULT hr = CoCreateInstance(CLSID_PortableDeviceManager,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&deviceManager));
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to CoCreateInstance CLSID_PortableDeviceManager, hr = 0x%lx", hr);
            goto END;
        }
        //! Get Devices
        hr = deviceManager->GetDevices(nullptr, &pnpDeviceIDCount);
        if (FAILED(hr) || 0 == pnpDeviceIDCount)
        {
            Log4WE_F(L"! Failed to get number of devices on the system, hr = 0x%lx", hr);
            goto END;
        }
        //! 遍历 Device
        PWSTR* pnpDeviceIDs = new (std::nothrow) PWSTR[pnpDeviceIDCount];
        if (pnpDeviceIDs == nullptr) goto END;

        ZeroMemory(pnpDeviceIDs, pnpDeviceIDCount * sizeof(PWSTR));
        DWORD retrievedDeviceIDCount = pnpDeviceIDCount;
        hr = deviceManager->GetDevices(pnpDeviceIDs, &retrievedDeviceIDCount);
        if (SUCCEEDED(hr))
        {
            // For each device found, display the devices friendly name,
            // manufacturer, and description strings.
            for (DWORD index = 0; index < retrievedDeviceIDCount; index++)
            {
                if ( !JGW_CompareString(DisplayManufacturer(deviceManager, pnpDeviceIDs[index]),strManufacturer) 
                    || !JGW_CompareString(DisplayDescription(deviceManager, pnpDeviceIDs[index]),strDescription)) continue;
                //! 判断是否能够打开MTP Device
                if (OpenMTPDevice(pnpDeviceIDs[index],clientInformation))
                {
                    bResult = true;
                    break;
                }
            }
        }
        else
        {
            Log4WE_F(L"! Failed to get the device list from the system, hr = 0x%lx", hr);
        }
        //</SnippetDeviceEnum3>

        // Free all returned PnPDeviceID strings by using CoTaskMemFree.
        // NOTE: CoTaskMemFree can handle nullptr pointers, so no nullptr
        //       check is needed.
        for (DWORD index = 0; index < pnpDeviceIDCount; index ++)
        {
            CoTaskMemFree(pnpDeviceIDs[index]);
            pnpDeviceIDs[index] = nullptr;
        }

        // Delete the array of PWSTR pointers
        delete [] pnpDeviceIDs;
        pnpDeviceIDs = nullptr;     
END:
        if (clientInformation) clientInformation->Release();
        clientInformation = NULL;
        if (deviceManager) deviceManager->Release();
        deviceManager = NULL;
        return bResult;
}

bool CCJGW_MediaTransferProtocol::OpenMTPDevice(LPCWSTR pszPnPDeviceID,IPortableDeviceValues* clientInformation)
{
    IPortableDevice* pDevice = NULL;
    IPortableDeviceContent* content = NULL;
    IEnumPortableDeviceObjectIDs* enumObjectIDs = NULL;
    bool bResult = false;

    HRESULT hr = CoCreateInstance(CLSID_PortableDeviceFTM,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pDevice));
    if (FAILED(hr)) return false;
    //! 打开MTP设备
    hr = pDevice->Open(pszPnPDeviceID,clientInformation);
    if (hr == E_ACCESSDENIED)
    {
        Log4WE(L"Failed to Open the device for Read Write access, will open it for Read-only access instead");
        clientInformation->SetUnsignedIntegerValue(WPD_CLIENT_DESIRED_ACCESS, GENERIC_READ);
        hr = pDevice->Open(pszPnPDeviceID, clientInformation);
    }
    if (FAILED(hr))
    {
        Log4WE_F(L"! Failed to Open the device, hr = 0x%lx", hr);
        // Release the IPortableDevice interface, because we cannot proceed
        // with an unopen device.
        goto CLEAR_DEVICES;
    }
    //! 查询MTP内容
    // Get an IPortableDeviceContent interface from the IPortableDevice interface to
    // access the content-specific methods.
    hr = pDevice->Content(&content);
    if (FAILED(hr))
    {
        Log4WE_F(L"! Failed to get IPortableDeviceContent from IPortableDevice, hr = 0x%lx", hr);
        pDevice->Close();
        goto CLEAR_DEVICES;
    }
    //! 遍历MTP
    hr = content->EnumObjects(0,                // Flags are unused
        WPD_DEVICE_OBJECT_ID,         // Starting from the passed in object
        nullptr,          // Filter is unused
        &enumObjectIDs);
    if (SUCCEEDED(hr)) bResult = true;

    Log4WE_F(L"! Failed to get IEnumPortableDeviceObjectIDs from IPortableDeviceContent, hr = 0x%lx", hr);
    pDevice->Close();

CLEAR_DEVICES:
    if (enumObjectIDs) enumObjectIDs->Release();
    enumObjectIDs = NULL;
    if (content) content->Release();
    content = NULL;
    if (pDevice) pDevice->Release();
    pDevice = NULL;
    return bResult;
}

std::wstring CCJGW_MediaTransferProtocol::DisplayManufacturer(
    _In_ IPortableDeviceManager* deviceManager,
    _In_ PCWSTR                  pnpDeviceID)
{
    DWORD manufacturerLength = 0;
    std::wstring strManufacturer;

    // 1) Pass nullptr as the PWSTR return string parameter to get the total number
    // of characters to allocate for the string value.
    HRESULT hr = deviceManager->GetDeviceManufacturer(pnpDeviceID, nullptr, &manufacturerLength);
    if (FAILED(hr))
    {
        Log4WE_F(L"! Failed to get number of characters for device manufacturer, hr = 0x%lx", hr);
        return L"";
    }
    else if (manufacturerLength > 0)
    {
        // 2) Allocate the number of characters needed and retrieve the string value.
        PWSTR manufacturer = new (std::nothrow) WCHAR[manufacturerLength];
        if (manufacturer != nullptr)
        {
            ZeroMemory(manufacturer, manufacturerLength * sizeof(WCHAR));
            hr = deviceManager->GetDeviceManufacturer(pnpDeviceID, manufacturer, &manufacturerLength);
            if (SUCCEEDED(hr))
            {
                Log4WE_F(L"Manufacturer:  %ws", manufacturer);
                strManufacturer = manufacturer;
            }
            else
            {
                Log4WE_F(L"! Failed to get device manufacturer, hr = 0x%lx", hr);
            }

            // Delete the allocated manufacturer string
            delete [] manufacturer;
            manufacturer = nullptr;
        }
        else
        {
            Log4WE(L"! Failed to allocate memory for the device manufacturer string");
        }
    }
    else
    {
        Log4WE(L"The device did not provide a manufacturer.");
    }

    return strManufacturer;
}

// Reads and displays the device discription for the specified PnPDeviceID string
std::wstring CCJGW_MediaTransferProtocol::DisplayDescription(
    _In_ IPortableDeviceManager* deviceManager,
    _In_ PCWSTR                  pnpDeviceID)
{
    DWORD descriptionLength = 0;
    std::wstring strDescription;

    // 1) Pass nullptr as the PWSTR return string parameter to get the total number
    // of characters to allocate for the string value.
    HRESULT hr = deviceManager->GetDeviceDescription(pnpDeviceID, nullptr, &descriptionLength);
    if (FAILED(hr))
    {
        Log4WE_F(L"! Failed to get number of characters for device description, hr = 0x%lx", hr);
    }
    else if (descriptionLength > 0)
    {
        // 2) Allocate the number of characters needed and retrieve the string value.
        PWSTR description = new (std::nothrow) WCHAR[descriptionLength];
        if (description != nullptr)
        {
            ZeroMemory(description, descriptionLength * sizeof(WCHAR));
            hr = deviceManager->GetDeviceDescription(pnpDeviceID, description, &descriptionLength);
            if (SUCCEEDED(hr))
            {
                Log4WE_F(L"Description:   %ws", description);
                strDescription = description;
            }
            else
            {
                Log4WE_F(L"! Failed to get device description, hr = 0x%lx", hr);
            }

            // Delete the allocated description string
            delete [] description;
            description = nullptr;
        }
        else
        {
            wprintf(L"! Failed to allocate memory for the device description string");
        }
    }
    else
    {
        wprintf(L"The device did not provide a description.");
    }
    return strDescription;
}

// Creates and populates an IPortableDeviceValues with information about
// this application.  The IPortableDeviceValues is used as a parameter
// when calling the IPortableDevice::Open() method.
void CCJGW_MediaTransferProtocol::GetClientInformation(IPortableDeviceValues** clientInformation)
{
    // Client information is optional.  The client can choose to identify itself, or
    // to remain unknown to the driver.  It is beneficial to identify yourself because
    // drivers may be able to optimize their behavior for known clients. (e.g. An
    // IHV may want their bundled driver to perform differently when connected to their
    // bundled software.)

    // CoCreate an IPortableDeviceValues interface to hold the client information.
    //<SnippetDeviceEnum7>
    HRESULT hr = CoCreateInstance(CLSID_PortableDeviceValues,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(clientInformation));
    //</SnippetDeviceEnum7>
    //<SnippetDeviceEnum8>
    if (SUCCEEDED(hr))
    {
        // Attempt to set all bits of client information
        hr = (*clientInformation)->SetStringValue(WPD_CLIENT_NAME, CLIENT_NAME);
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to set WPD_CLIENT_NAME, hr = 0x%lx", hr);
        }

        hr = (*clientInformation)->SetUnsignedIntegerValue(WPD_CLIENT_MAJOR_VERSION, CLIENT_MAJOR_VER);
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to set WPD_CLIENT_MAJOR_VERSION, hr = 0x%lx", hr);
        }

        hr = (*clientInformation)->SetUnsignedIntegerValue(WPD_CLIENT_MINOR_VERSION, CLIENT_MINOR_VER);
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to set WPD_CLIENT_MINOR_VERSION, hr = 0x%lx", hr);
        }

        hr = (*clientInformation)->SetUnsignedIntegerValue(WPD_CLIENT_REVISION, CLIENT_REVISION);
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to set WPD_CLIENT_REVISION, hr = 0x%lx", hr);
        }

        //  Some device drivers need to impersonate the caller in order to function correctly.  Since our application does not
        //  need to restrict its identity, specify SECURITY_IMPERSONATION so that we work with all devices.
        hr = (*clientInformation)->SetUnsignedIntegerValue(WPD_CLIENT_SECURITY_QUALITY_OF_SERVICE, SECURITY_IMPERSONATION);
        if (FAILED(hr))
        {
            Log4WE_F(L"! Failed to set WPD_CLIENT_SECURITY_QUALITY_OF_SERVICE, hr = 0x%lx", hr);
        }
    }
    else
    {
        Log4WE_F(L"! Failed to CoCreateInstance CLSID_PortableDeviceValues, hr = 0x%lx", hr);
    }
    //</SnippetDeviceEnum8>
}

// Recursively called function which enumerates using the specified
// object identifier as the parent.
//     void CCJGW_MediaTransferProtocol::RecursiveEnumerate(
//         _In_ PCWSTR                  objectID,
//         _In_ IPortableDeviceContent* content)
//     {
//         ComPtr<IEnumPortableDeviceObjectIDs> enumObjectIDs;
// 
//         // Print the object identifier being used as the parent during enumeration.
//         wprintf(L"%ws\n", objectID);
// 
//         ComPtr<IPortableDeviceProperties> deviceProperties;
//         ComPtr<IPortableDeviceValues> deviceValues;
//         HRESULT hr = content->Properties(&deviceProperties);
// 
//         hr = deviceProperties->GetValues(objectID, NULL, &deviceValues);
//         PWSTR szBuf[260] = {0};
//         hr = deviceValues->GetStringValue(WPD_OBJECT_NAME, szBuf);
//         wprintf(L"%ws\n", szBuf[0]);
// 
//         // Get an IEnumPortableDeviceObjectIDs interface by calling EnumObjects with the
//         // specified parent object identifier.
//         hr = content->EnumObjects(0,                // Flags are unused
//             objectID,         // Starting from the passed in object
//             nullptr,          // Filter is unused
//             &enumObjectIDs);
//         if (FAILED(hr))
//         {
//             wprintf(L"! Failed to get IEnumPortableDeviceObjectIDs from IPortableDeviceContent, hr = 0x%lx\n", hr);
//         }
// 
//         // Loop calling Next() while S_OK is being returned.
//         while(hr == S_OK)
//         {
//             DWORD  numFetched = 0;
//             PWSTR  objectIDArray[NUM_OBJECTS_TO_REQUEST] = {0};
//             hr = enumObjectIDs->Next(NUM_OBJECTS_TO_REQUEST,    // Number of objects to request on each NEXT call
//                 objectIDArray,             // Array of PWSTR array which will be populated on each NEXT call
//                 &numFetched);              // Number of objects written to the PWSTR array
//             if (SUCCEEDED(hr))
//             {
//                 // Traverse the results of the Next() operation and recursively enumerate
//                 // Remember to free all returned object identifiers using CoTaskMemFree()
//                 for (DWORD index = 0; (index < numFetched) && (objectIDArray[index] != nullptr); index++)
//                 {
//                     RecursiveEnumerate(objectIDArray[index], content);
// 
//                     // Free allocated PWSTRs after the recursive enumeration call has completed.
//                     CoTaskMemFree(objectIDArray[index]);
//                     objectIDArray[index] = nullptr;
//                 }
//             }
//         }
//     }
}