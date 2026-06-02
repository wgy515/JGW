#include "stdafx.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

const wchar_t* GetStringEn(E_STRING_TYPE eStringType)
{
    switch (eStringType)
    {
    case E_LOAD_MESCONFIG_FAIL:
        return L"MES Configuration file failed to load";
    case E_PROMPT_TYPE:
        return L"Prompt";
    case E_APPLICATON_REPEAT_RUN_TYPE:
        return L"The program is running, whether to wait for the program to exit normally!\n1, please wait 10S to open the program \n2, close the process in the task management.\n3click cancel button, the program will automatically close the previous instance";
    case E_PASSWORD_ENCRYPTION_FAILED_TYPE:
        return L"Password encryption failed";
    case E_PASSWORD_DECRYPTION_FAILED_TYPE:
        return L"Password decryption failed";
    case E_CHILDFORM_INIT_FAIL_TYPE:
        return L"Failed to initialize child form";
    case E_CONTROL_FAILED_TOLOAD:
        return L"Control failed to load";
    case E_VIEW_INIT_ERROR:
        return L"View initialization error";
    case E_VIEW_LAYOUT_FILE_LOADING_ERROR:
        return L"View layout file loading error";
    case E_DAY_SHIFT_TYPE:
        return L"Day Shift";
    case E_NIGHT_SHIFT_TYPE:
        return L"Night Shift";
    case E_GET_PLUGIN_INFO_BUTTON_NOY_AVAILABLE:
        return L"The plug-in information corresponding to the current button is not available";
    case E_NOT_SELECTED_TASK:
        return L"Order information not selected";
    case E_SELECT_CURRENT_ORDER_INFO:
        return L"Please select the current test order information";
    case E_INITIALIZATION_FAILUER:
        return L"Initialization failure";
    case E_PLUGIN_INITIALIZATION_ERROR:
        return L"Error in plug-in initialization";
    case E_ACCOUNT_DEADLINE_PERMANENT:
        return L"Account Deadline: PERMANENT";
    case E_ACCOUNT_LOGIN_EXPIREAD_RELOGIN:
        return L"The account login has expired, please login again";
    case E_ACCOUNT_DEADLINE_SEC:
        return L"Account Deadline: %d Second";
    case E_ACCOUNT_DEADLINE_MINUTE:
        return L"Account Deadline: %d M";
    case E_ACCOUNT_DEADLINE_HOUR:
        return L"Account Deadline: %d H %d M";
    case E_DEVICE_POOL_CREATION_FAILED:
        return L"Device pool creation failed";
    case E_MES_UNKNOWN_STATUS_TYPE:
        return L" MES Unknown Status";
    case E_MES_ONLINE_MODE_TYPE:
        return L" MES Online Mode";
    case E_NO_ORDER_OR_WORKORDER_SELECTED:
        return L" No order or work order selected";
    case E_MES_ACCOUNT_LOGIN_ERROR:
        return L" MES Account login error";
    case E_MES_OFFLINE_MODE:
        return L" MES Off-Line Mode";
    case E_EXCEPTION_ERROR_TYPE:
        return L"Exception";
    case E_HELP_SERVICE_CONNECTION_EXCEPTION:
        return L"An exception has occurred in the upgrade service connection";
    case E_HELP_CONNECT_FTP_ERROR:
        return L"Unable to connect to FTP server due to an exception, please check network connection \n click OK: Exit current program, click Cancel: Ignore update error message to enter main program";
    case E_HELP_TOOL_UPDATE_INTSALL_VERSION_TIP:
        return L"It is detected that there is a new version of the tool that needs to be updated (when the new version is updated, uninstall the old version first, and then install the new version)";
    case E_HELP_INSTALL_VERSION_MSGTIP:
        return L"Install version update";
    case E_HELP_TOOL_UPDATE_PACKAGE_VERSION_TIP:
        return L"An update to the toolkit is detected (when the toolkit is updated, the current test tool platform is shut down and the toolkit is updated)";
    case E_HELP_PACKAGE_VERSION_MSGTIP:
        return L"Toolkit update";
    case E_HELP_UPDATE_TIMEOUNT_TIP:
        return L"Program update appears timeout automatically, please confirm whether the download speed is too slow or other exceptions caused by!!\n Click OK: Exit the current program, click Cancel: Continue to wait for the program to download updates";
    case E_HELP_UPDATE_TIMEOUNT_TIPMSG:
        return L"The program automatically updates the timeout";
    case E_HELP_CLOSE_UPDATE_APPLICATION:
        return L"Close the current program, or ignore automatic program update \n Click abort: Close the current program, \n click ignore: Ignore automatic program update \n Click retry: Continue with program update";
    case E_HELP_CLOSE_IGNORE_UPDATE_TIPMSG:
        return L"Whether to turn off or ignore automatic program updates";
    case E_MES_SELECT_STATION_ID:
        return L"Please select station ID";
    case E_MES_BARCODE_STATIONID:
        return L"Please select the station id corresponding to the current Barcode";
    case E_MES_NO_GET_TASKNUMBER:
        return L"The current order number was not obtained";
    case E_MES_GET_IMEI_TASKNUMBER_FAIL:
        return L"Obtaining the order number corresponding to IMEI failed";
    case E_MES_GET_SN_TASKNUMBER_FAIL:
        return L"Obtaining the order number corresponding to SN Barcode failed";
    case E_MES_GET_TASKNUMBER_FAIL:
        return L"Failed to get the task number";
    case E_MES_GET_WorkOrderNumber_TASKNUMBER_FAIL:
        return L"Obtaining the task number corresponding to WorkOrderNumber failed";
    case E_MES_SERVICE_APPLICATION_FAILED_TO_START:
        return L"The MES service application failed to start";
    case E_MES_SERVICE_APPLICATION_SUCCESSFULLY_TO_START:
        return L"The MES service application has been started successfully";
    case E_MES_STARTING_SERVICE_PLEASE_WAIT:
        return L"Starting the MES service application, please wait...";
    case E_INPUT_GRETERTHAN_EQUAL_LESSTHAN_CHARACTERS:
        return L"Please enter %d characters greater than or equal to and less than %d characters";
    case E_INPUT_PREFIX_NOTMATCH_PLEASE_REINPUT:
        return L"The input character prefix does not match. Please re-enter the character with the prefix %s";
    case E_INPUT_IMEI_VERIFICATION_FAIL:
        return L"IMEI verification code generation failed";
    case E_INPUT_IMEI_CHECKCODE:
        return L"The current segment'S IMEI check code is: %s";
    case E_MES_WORKORDERNUMBER_IS_EMPTY:
        return L"The current order number is empty";
    case E_CHILDFORM_PARAM_IS_NOT_EMPTY:
        return L"The subform initialization parameter cannot be null";
    case E_HELP_SERVICE_APPLICATION_FAILED_TO_START:
        return L"The automatic update service application failed to start";
    case E_HELP_SERVICE_APPLICATION_SUCCESSFULLY_TO_START:
        return L"The automatic update service application has been started successfully";
    case E_HELP_STARTING_SERVICE_PLEASE_WAIT:
        return L"The Automatic update service application is starting, please wait...";
    case E_MES_WARNING_OFFLINE_MODE:
        return L"Warning: The current MES system is in offline mode";
    case E_MES_WARNING_STATION_NOT_PROCESS:
        return L"Warning: The current site is not going through the MES process";
    case E_MES_CONNECTING_DATA_SERVICE:
        return L"Connecting to MES data Service";
    case E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING:
        return L"Connection to the MES data service failed. Please check whether the MES service is enabled";
    case E_MES_CONNECT_SUCCESSFULLY:
        return L"Connected to MES data service successfully";
    case E_MES_CONNECT_SERVICE_FAILED:
        return L"Communication with the MES service failed";
    case E_WIFI_CONNECTING_DATA_SERVICE:
        return L"Connecting to WIFI Service";
    case E_WIFI_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING:
        return L"Connection to the WIFI service failed. Please check whether the WIFI service is enabled";
    case E_WIFI_CONNECT_SUCCESSFULLY:
        return L"Connected to WIFI service successfully";
    case E_CPE_DOWNLOAD_PORTHASSORTED_RESORT:
        return L"The current port has been sorted. If you need to sort, please clear the sorted information again";
    case E_CPE_DOWNLOAD_PORT_IS_SORT_NEXTINDEX_SORT:
        return L"Whether the current sort has been completed to proceed to the next index sort";
    case E_CPE_DOWNLOAD_INDEX_OUTOFRANGE_CHECK_ISSORT:
        return L"The current thread index is out of range, check that the current port is not sorted";
    case E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG:
        return L"The device has been inserted during the operation of the download program. It is not allowed to modify the configuration file after the device is inserted. Please reopen the download program to modify the configuration file";
    case E_TSE_INITFAIL_CHECKCONFIG_LOG:
        return L"Initialization configuration failed, please check whether the configuration is normal, see Log for details";
    case E_TSE_LOAD_MODULE_FAIL:
        return L"Failed to load the TSE module";
    case E_XML_FAILED_PARSE_CONFIG:
        return L"Failed to parse the xml configuration file(%s)";
    case E_TSE_XMLCONFIG_NO_TEST_PROJECT:
        return L"There are no test items in the current configuration file";
    case E_TSE_TEST_INDEX_RULE:
        return L"Please test in test order, or click on the last test method to reset the current test.";
    case E_TSE_FILE_NOT_EXIST:
        return L"File does not exist ";
    }
    return L"";
}