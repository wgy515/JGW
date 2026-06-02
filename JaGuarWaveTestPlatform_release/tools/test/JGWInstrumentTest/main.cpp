#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_NIGPIBControllImpl/sicl.h>

#include <JGW_NIGPIBControllImpl/IVI Foundation/VISA/WinNT/include/visa.h>
#if 0
#import "GlobMgr.dll" no_namespace 
int _tmain(int argc, _TCHAR* argv[])
{
    IResourceManagerPtr rm;
    IMessagePtr ptr;
    IFormattedIO488Ptr fmioPtr;
    CoInitialize(NULL);
    try{
        // Instantiate the Global Resource Manager and Formatted IO class
        rm.CreateInstance(__uuidof(ResourceManager));
        fmioPtr.CreateInstance(__uuidof(FormattedIO488));

        /* Open the session with the default values for the Lock (None), Timeout(N/A), 
        and OptionString (""). The dev_addr can be something like "GPIB0::1::INSTR".*/
        ptr = rm->Open("GPIB0::17::INSTR", NO_LOCK, 0, "");

        _bstr_t idnstr;
        long count = 100, written;

        //Using IMessage methods

        ptr->Clear();
        written = ptr->WriteString("*IDN?\n");
        idnstr = ptr->ReadString(count);


        //Using formattedIO methods
        //First set the IO stream to use for the formatted IO.
        fmioPtr->IO = ptr;
        _bstr_t str_result;
        fmioPtr->FlushRead();
        fmioPtr->FlushWrite(VARIANT_FALSE);
        fmioPtr->WriteString("*IDN?\n", VARIANT_FALSE);
        str_result = fmioPtr->ReadString();
    }
    catch(_com_error &err)
    {
        if (ptr != NULL)
        {
            ptr->Close();
            ptr = NULL;
        }

        if (rm != NULL)
        {
            rm.Release();
            rm = NULL;
        }

        if (fmioPtr != NULL)
        {
            fmioPtr.Release();
            fmioPtr = NULL;
        }
        MessageBoxA(NULL, err.Description(), "Error Occurred", MB_OK);
        return 0;
    }
    // Note that it is very important to close the opened session before
    // calling CoUninitialize(). Without doing this, there will be a memory 
    // reference problem. 
    ptr->Close();
    ptr = NULL;
    rm.Release();
    rm = NULL;
    fmioPtr.Release();
    fmioPtr = NULL;
    CoUninitialize();
    return 1;
} 

#endif
#if 1
static char outputBuffer[VI_FIND_BUFLEN];
static ViSession defaultRM, instr;
static ViStatus status;
static ViUInt32 count;
static ViUInt16 portNo;
static ViSession instr1, instr2;     
// typedef void* ADBAPIHANDLE;
// 
// /** \brief Creates USB interface object
// 
//   This routine creates an object that represents a USB interface.
//   @param[in] interface_name Name of the interface.
//   @return Handle to the interface object or NULL on failure. If NULL is
//           returned GetLastError() provides extended error information.
// */
// typedef ADBAPIHANDLE  (__cdecl *AdbCreateInterfaceByNameFunc)(const wchar_t* interface_name);
// 
// typedef bool (__cdecl *AdbGetSerialNumberFunc)(ADBAPIHANDLE adb_interface,
//     void* buffer,
//     unsigned long* buffer_char_size,
//     bool ansi);
// 
// typedef bool (__cdecl *AdbCloseHandleFunc)(ADBAPIHANDLE adb_handle);
// //! lpdevice->m_strDbcc_name = "\\?\USB#VID_05C6&PID_9039&MI_01#6&311f0f4b&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}"
int _tmain(int argc, _TCHAR* argv[])
{
#if 1
    /* First we will need to open the default resource manager. */
    status=viOpenDefaultRM (&defaultRM);
    if (status < VI_SUCCESS)
    {
        printf("Could not open a session to the VISA Resource Manager!\n");
        exit (EXIT_FAILURE);
    }

    /* Now we will open a session via TCP/IP to ni.com */
    status = viOpen (defaultRM, "GPIB0::17::INSTR", VI_NULL, VI_NULL, &instr1);
    if (status < VI_SUCCESS)  
    {
        printf ("Cannot open a session to the device.\n");
       // goto Close;
    }

#else

    INST dvm;
    INST sw;
    double res;
    int i;
    /* Log message and terminate on error */
    ionerror (I_ERROR_EXIT);
    //! Using SICL with GPIB 4 Agilent SICL User¡¯s Guide 89
    /* Open the multimeter and switch sessions*/

    dvm = iopen ("gpib0,17");
    //! sw = iopen ("gpib0,9,14");
    itimeout (dvm, 10000);
    //! itimeout (sw, 10000);
    /*Set up trigger*/
//     iprintf (sw, ¡°TRIG:SOUR BUS\n¡±);
//     /*Set up scan list*/
//     iprintf (sw,¡±SCAN (@100:103)\n¡±);
//     iprintf (sw,¡±INIT\n¡±);
//     for (i=1;i<=4;i++)
//     {
//         /* Take a measurement */
//         iprintf (dvm,¡±MEAS:VOLT:DC?\n¡±);
//         /* Read the results */
//         iscanf (dvm,¡±%lf¡±,&res);
//         /* Print the results */
//         printf (¡°Result is %lf\n¡±,res);
//         /* Trigger to close channel */
//         iprintf (sw, ¡°TRIG\n¡±);
//     }
    /* Close the multimeter and switch sessions */
    iclose (dvm);
    //iclose (sw);
    return 0;
#endif
//     std::wstring strApiPath = JGW::JGW_GetApplicationFolder();
//     strApiPath += L"AdbWinApi.dll";
//     HMODULE hModule = LoadLibrary(strApiPath.c_str());
//     if (NULL == hModule) return -1;
// 
//     AdbCreateInterfaceByNameFunc AdbCreateInterfaceByName = (AdbCreateInterfaceByNameFunc)GetProcAddress(hModule,"AdbCreateInterfaceByName");
//     AdbGetSerialNumberFunc AdbGetSerialNumber = (AdbGetSerialNumberFunc)GetProcAddress(hModule,"AdbGetSerialNumber");
//     AdbCloseHandleFunc AdbCloseHandle = (AdbCloseHandleFunc)GetProcAddress(hModule,"AdbCloseHandle");
//     //! next_interface->device_name = 0x0231df9c ""
//     ADBAPIHANDLE pHandle = AdbCreateInterfaceByName(L"\\\\?\\usb#vid_05c6&pid_9039&mi_01#6&311f0f4b&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}");
//     /*\\\\?\\usb#vid_05c6&pid_9039&mi_01#6&311f0f4b&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}*/
//     wchar_t szBuf[256] = {0};
//     unsigned long bufSize = 256;
//     AdbGetSerialNumber(pHandle,szBuf,&bufSize,false);
//     AdbCloseHandle(pHandle);

}
#endif