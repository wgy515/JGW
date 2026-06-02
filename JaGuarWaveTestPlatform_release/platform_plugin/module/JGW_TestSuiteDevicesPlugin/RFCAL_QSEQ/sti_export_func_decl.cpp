/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/SuiteTestServer/Manager/sti_export_func_decl.cpp#11 $
 * $DateTime: 2015/12/22 17:37:27 $
 *
 * DESCRIPTION: sti_export_func_decl
 ******************************************************************************
 *
 * Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#include "stdafx.h"
#include "sti_export_func_decl.h"

/**
 * --- Function name strings ---
 */

/* Suite Interface */
static const char * STI_CREATE_SUITE_INTERFACE = "sti_create_suite_interface";
static const char * STI_RELEASE_SUITE_INTERFACE = "sti_release_suite_interface";
/* Configuration */
//static const char * STI_ADD_ANCILLARY_LIBRARY = "sti_add_ancillary_library";
static const char * STI_ADD_STATION_CONFIG_ITEM = "sti_add_station_config_item";
static const char * STI_CLEAR_STATION_CONFIG = "sti_clear_station_config";
static const char * STI_ADD_GPIB_EQUIP_CONFIG_ITEM = "sti_add_gpib_equip_config_item";
static const char * STI_ADD_VISA_EQUIP_CONFIG_ITEM = "sti_add_visa_equip_config_item";
static const char * STI_ADD_IP_EQUIP_CONFIG_ITEM = "sti_add_ip_equip_config_item";
static const char * STI_ADD_RS232_EQUIP_CONFIG_ITEM = "sti_add_rs232_equip_config_item";
static const char * STI_CLEAR_EQUIP_CONFIG = "sti_clear_equip_config";
static const char * STI_LOAD_DATABASE_CONFIG = "sti_load_database_configuration";
/* Log */
static const char * STI_REQUEST_SINGLE_TEST_LOG = "sti_request_single_test_log";
static const char * STI_REQUEST_TEST_LOG_ID = "sti_request_test_log_id";
//static const char * STI_RELEASE_TEST_LOG = "sti_release_test_log";
static const char * STI_REQUEST_TEST_LOG_NAME = "sti_request_test_log_name";
static const char * STI_REQUEST_COMPLETE_TEST_LOG = "sti_request_complete_test_log";
/* PathLoss */
//static const char * STI_LOAD_XML_RF_NET_LOSS = "sti_load_xml_rf_net_loss";
static const char * STI_LOAD_SINGLE_RF_NET_LOSS = "sti_load_single_rf_net_loss";
static const char * STI_LOAD_MULTIPLE_RF_NET_LOSS = "sti_load_multiple_rf_net_loss";
static const char * STI_LOAD_MULTIPLE_CONFIG_RF_NET_LOSS = "sti_load_multiple_config_rf_net_loss";
static const char * STI_CLEAR_RF_NET_LOSS = "sti_clear_rf_net_loss";
/* System */
static const char * STI_SET_LOG_FILENAME = "sti_set_log_filename";
static const char * STI_SET_ENVIRONMENT = "sti_set_environment_var";
static const char * STI_GET_ENVIRONMENT = "sti_get_environment_var";
static const char * STI_REGISTER_SYSTEM_EVENT_HANDLER = "sti_register_system_event_handler";
/* Test */
static const char * STI_EXECUTE = "sti_execute";
static const char * STI_CLEAR_TEST_DB = "sti_clear_test_db";
static const char * STI_ADD_TEST = "sti_add_test";
static const char * STI_ADD_PARAMETER = "sti_add_parameter";
static const char * STI_GET_TEST_LIST = "sti_get_test_list";
static const char * STI_EXECUTE_TEST_ID = "sti_execute_test_id";
static const char * STI_EXECUTE_ALL = "sti_execute_all";
static const char * STI_EXECUTE_TEST_NAME = "sti_execute_test_name";
static const char * STI_CLOSE_HANDLE = "sti_close_handle";
//static const char * STI_SET_RGI_PARAMETER = "sti_setrgi_parameter";
//static const char * STI_CLOSE_HANDLE = "sti_closeHandle";
/******************************************************************************
 * FUNCTION: sti_initialize_export_decl
 *
 * DESCRIPTION: See header file
 *****************************************************************************/
int sti_initialize_export_decl(HMODULE asm_ptr, sti_export_func_decl * exported)
{  
   if(0==asm_ptr || 0==exported)
   {
      return 0;
   }

   bool success = true;

   /* Suite Interface */
   exported->sti_create_suite_interface = (sti_create_suite_interface_fn)GetProcAddress(asm_ptr,STI_CREATE_SUITE_INTERFACE);
   exported->sti_release_suite_interface = (sti_release_suite_interface_fn)GetProcAddress(asm_ptr,STI_RELEASE_SUITE_INTERFACE);
   success &= (0!=exported->sti_create_suite_interface);
   success &= (0!=exported->sti_release_suite_interface);
   exported->sti_close_handle = (sti_close_handle_fn)GetProcAddress(asm_ptr,STI_CLOSE_HANDLE);

   if( !exported->sti_close_handle )
   {
	   exported->sti_close_handle = (sti_close_handle_fn)GetProcAddress(asm_ptr,"sti_closeHandle");
   }

   /* Configuration */
 //  exported->sti_add_ancillary_library = (sti_add_ancillary_library_fn)GetProcAddress(asm_ptr,STI_ADD_ANCILLARY_LIBRARY);
   exported->sti_add_station_config_item = (sti_add_station_config_item_fn)GetProcAddress(asm_ptr,STI_ADD_STATION_CONFIG_ITEM);
   exported->sti_clear_station_config = (sti_clear_station_config_fn)GetProcAddress(asm_ptr,STI_CLEAR_STATION_CONFIG);
   exported->sti_add_gpib_equip_config_item = (sti_add_gpib_equip_config_item_fn)GetProcAddress(asm_ptr,STI_ADD_GPIB_EQUIP_CONFIG_ITEM);
   exported->sti_add_visa_equip_config_item = (sti_add_visa_equip_config_item_fn)GetProcAddress(asm_ptr,STI_ADD_VISA_EQUIP_CONFIG_ITEM);
   exported->sti_add_ip_equip_config_item = (sti_add_ip_equip_config_item_fn)GetProcAddress(asm_ptr,STI_ADD_IP_EQUIP_CONFIG_ITEM);
   exported->sti_add_rs232_equip_config_item = (sti_add_rs232_equip_config_item_fn)GetProcAddress(asm_ptr,STI_ADD_RS232_EQUIP_CONFIG_ITEM);
   exported->sti_clear_equip_config = (sti_clear_equip_config_fn)GetProcAddress(asm_ptr,STI_CLEAR_EQUIP_CONFIG);
   exported->sti_load_database_configuration = (sti_load_database_configuration_fn)GetProcAddress(asm_ptr,STI_LOAD_DATABASE_CONFIG);
//   success &= (0!=exported->sti_add_ancillary_library);
   success &= (0!=exported->sti_add_station_config_item);
   success &= (0!=exported->sti_clear_station_config);
   success &= (0!=exported->sti_add_gpib_equip_config_item);
   success &= (0!=exported->sti_add_visa_equip_config_item);
   success &= (0!=exported->sti_add_ip_equip_config_item);
   success &= (0!=exported->sti_add_rs232_equip_config_item);
   success &= (0!=exported->sti_add_rs232_equip_config_item);
   success &= (0!=exported->sti_clear_equip_config);
   success &= (0!=exported->sti_load_database_configuration);

   /* Log */
   exported->sti_request_single_test_log = (sti_request_single_test_log_fn)GetProcAddress(asm_ptr,STI_REQUEST_SINGLE_TEST_LOG);
   exported->sti_request_test_log_id = (sti_request_test_log_id_fn)GetProcAddress(asm_ptr,STI_REQUEST_TEST_LOG_ID);
  // exported->sti_release_test_log = (sti_release_test_log_fn)GetProcAddress(asm_ptr,STI_RELEASE_TEST_LOG);
   exported->sti_request_test_log_name = (sti_request_test_log_name_fn)GetProcAddress(asm_ptr,STI_REQUEST_TEST_LOG_NAME);
   exported->sti_request_complete_test_log = (sti_request_complete_test_log_fn)GetProcAddress(asm_ptr,STI_REQUEST_COMPLETE_TEST_LOG);
   success &= (0!=exported->sti_request_single_test_log);
   success &= (0!=exported->sti_request_test_log_id);
 //  success &= (0!=exported->sti_release_test_log);
   success &= (0!=exported->sti_request_test_log_name);
   success &= (0!=exported->sti_request_complete_test_log);

   /* PathLoss */
 //  exported->sti_load_xml_rf_net_loss = (sti_load_xml_rf_net_loss_fn)GetProcAddress(asm_ptr,STI_LOAD_XML_RF_NET_LOSS);
   exported->sti_load_single_rf_net_loss = (sti_load_single_rf_net_loss_fn)GetProcAddress(asm_ptr,STI_LOAD_SINGLE_RF_NET_LOSS);
   exported->sti_load_multiple_rf_net_loss = (sti_load_multiple_rf_net_loss_fn)GetProcAddress(asm_ptr,STI_LOAD_MULTIPLE_RF_NET_LOSS);
   exported->sti_load_multiple_config_rf_net_loss = (sti_load_multiple_config_rf_net_loss_fn)GetProcAddress(asm_ptr,STI_LOAD_MULTIPLE_CONFIG_RF_NET_LOSS);
   exported->sti_clear_rf_net_loss = (sti_clear_rf_net_loss_fn)GetProcAddress(asm_ptr,STI_CLEAR_RF_NET_LOSS);
//   success &= (0!=exported->sti_load_xml_rf_net_loss);
   success &= (0!=exported->sti_load_single_rf_net_loss);
   success &= (0!=exported->sti_load_multiple_rf_net_loss);
   success &= (0!=exported->sti_load_multiple_config_rf_net_loss);
   success &= (0!=exported->sti_clear_rf_net_loss);

   /* System */
   exported->sti_set_log_filename = (sti_set_log_filename_fn)GetProcAddress(asm_ptr,STI_SET_LOG_FILENAME);
   exported->sti_set_environment_var = (sti_set_environment_var_fn)GetProcAddress(asm_ptr,STI_SET_ENVIRONMENT);
   exported->sti_get_environment_var = (sti_get_environment_var_fn)GetProcAddress(asm_ptr,STI_GET_ENVIRONMENT);
   exported->sti_register_system_event_handler = (sti_register_system_event_handler_fn)GetProcAddress(asm_ptr,STI_REGISTER_SYSTEM_EVENT_HANDLER);
   success &= (0!=exported->sti_set_log_filename);
   success &= (0!=exported->sti_set_environment_var);
   success &= (0!=exported->sti_get_environment_var);
   success &= (0!=exported->sti_register_system_event_handler);

   /* Test */
   exported->sti_execute = (sti_execute_fn)GetProcAddress(asm_ptr,STI_EXECUTE);
   exported->sti_clear_test_db = (sti_clear_test_db_fn)GetProcAddress(asm_ptr,STI_CLEAR_TEST_DB);
   exported->sti_add_test = (sti_add_test_fn)GetProcAddress(asm_ptr,STI_ADD_TEST);
   exported->sti_add_parameter = (sti_add_parameter_fn)GetProcAddress(asm_ptr,STI_ADD_PARAMETER);
   exported->sti_get_test_list = (sti_get_test_list_fn)GetProcAddress(asm_ptr,STI_GET_TEST_LIST);
   exported->sti_execute_test_id = (sti_execute_test_id_fn)GetProcAddress(asm_ptr,STI_EXECUTE_TEST_ID);
   exported->sti_execute_all = (sti_execute_all_fn)GetProcAddress(asm_ptr,STI_EXECUTE_ALL);
   exported->sti_execute_test_name = (sti_execute_test_name_fn)GetProcAddress(asm_ptr,STI_EXECUTE_TEST_NAME);
   success &= (0!=exported->sti_execute);
   success &= (0!=exported->sti_clear_test_db);
   success &= (0!=exported->sti_add_test);
   success &= (0!=exported->sti_add_parameter);
   success &= (0!=exported->sti_get_test_list);
   success &= (0!=exported->sti_execute_test_id);
   success &= (0!=exported->sti_execute_all);
   success &= (0!=exported->sti_execute_test_name);

   if(!success)
   {
      //::MessageBox(NULL, L"Failed to GetProcAddress from RF Test Server DLL: SuiteTestManager.dll can't locate a necessary function from the RF Test Server DLL.  You may be using an older version of RF Test Server which doesn't support the currently loaded interface", L"SuiteTestManager", 0);
   }

   return (success) ? 1 : 0;
}
