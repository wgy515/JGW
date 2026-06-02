/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/SuiteTestServer/Manager/sti_export_func_decl.h#8 $
 * $DateTime: 2016/04/17 23:56:28 $
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
#ifndef __STI_EXPORT_FUNC_DECL__
#define __STI_EXPORT_FUNC_DECL__

/**
 * --- Function pointer declarations ---
 */

/* Suite Interface */
typedef void*(_cdecl  *sti_create_suite_interface_fn)(void);
typedef void(_cdecl *sti_release_suite_interface_fn)(void** iface);

/* Configuration */
typedef void(_cdecl *sti_add_station_config_item_fn)(void * iface, int itemGroup, const char * name, char * value);
typedef void(_cdecl *sti_clear_station_config_fn)(void * iface);
typedef void(_cdecl *sti_add_gpib_equip_config_item_fn)(void * iface, const char * identifier, const char * name, const char * type, const char * pad, const char * sad, const char * options);
typedef void(_cdecl *sti_add_visa_equip_config_item_fn)(void * iface, const char * identifier, const char * name, const char * type, const char * address, const char * options);
typedef void(_cdecl *sti_add_ip_equip_config_item_fn)(void * iface, const char * identifier, const char * name, const char * type, const char * instrument, const char * address, const char * options);
typedef void(_cdecl *sti_add_rs232_equip_config_item_fn)(void * iface, const char * identifier, const char * name, const char * type, const char * comport, const char * options);
typedef void(_cdecl *sti_clear_equip_config_fn)(void * iface);
typedef void(_cdecl *sti_load_database_configuration_fn)(void * iface);
typedef void(_cdecl *sti_add_ancillary_library_fn)(void * iface, const char * file);

/* Log */
typedef void(_cdecl *sti_request_single_test_log_fn)(void * iface, int mode, char ** value);
typedef void(_cdecl *sti_request_test_log_id_fn)(void * iface, int mode, int testId, char ** value);
typedef void(_cdecl *sti_release_test_log_fn)(void * iface, char ** value);
typedef void(_cdecl *sti_request_test_log_name_fn)(void * iface, int mode, const char * testName, char ** value);
typedef void(_cdecl *sti_request_complete_test_log_fn)(void * iface, int mode, char ** value);

/* PathLoss */
typedef void(_cdecl *sti_load_single_rf_net_loss_fn)(void * iface, double freqMhz, double lossDb);
typedef void(_cdecl *sti_load_xml_rf_net_loss_fn)(void * iface, const char * fileName);
typedef void(_cdecl *sti_load_multiple_rf_net_loss_fn)(void * iface, int path, double freqMhz, double lossDb);
typedef void(_cdecl *sti_load_multiple_config_rf_net_loss_fn)(void * iface, const char * configName, int path, double freqMhz, double lossDb);
typedef void(_cdecl *sti_clear_rf_net_loss_fn)(void * iface);

/* System */
typedef void(*sti_sys_evt_handler)(void * context, int type, const char * msg);
typedef void(_cdecl *sti_set_log_filename_fn)(void * iface, const char * name);
typedef void(_cdecl *sti_set_environment_var_fn)(void * iface, const char * key, const char * value);
typedef char*(_cdecl *sti_get_environment_var_fn)(void * iface, const char * key);
typedef void(_cdecl *sti_register_system_event_handler_fn)(void * iface, void * context, sti_sys_evt_handler evtcb);

/* Test */
typedef int(_cdecl *sti_execute_fn)(void * iface, const char * testname, const char * realname, const char ** arrParamKeys, int keysCount, const char ** arrParamValues, int valuesCount);
typedef void(_cdecl *sti_clear_test_db_fn)(void * iface);
typedef int(_cdecl *sti_add_test_fn)(void * iface, const char * testName, const char * realName);
typedef void(_cdecl *sti_add_parameter_fn)(void * iface, int testId, const char * parameterName, const char * parameterValue, const char * upperlimit, const char * lowerlimit, const char * paramMode);
typedef char**(_cdecl *sti_get_test_list_fn)(void * iface, int verbose, char ** arrTestList, int * count);
typedef int(_cdecl *sti_execute_test_id_fn)(void * iface, int testId);
typedef int(_cdecl *sti_execute_all_fn)(void * iface);
typedef int(_cdecl *sti_execute_test_name_fn)(void * iface, const char * testName);
typedef void(_cdecl *sti_close_handle_fn)(void * iface);

/**
 * Structure defined to contain interface function pointers.
 */
typedef struct sti_export_func_decl
{
/* SUITE INTERFACE */
   sti_create_suite_interface_fn                 sti_create_suite_interface;
   sti_release_suite_interface_fn                sti_release_suite_interface;
/* CONFIGURATION */
   sti_add_ancillary_library_fn                  sti_add_ancillary_library;
   sti_add_station_config_item_fn                sti_add_station_config_item;
   sti_clear_station_config_fn                   sti_clear_station_config;
   sti_add_gpib_equip_config_item_fn             sti_add_gpib_equip_config_item;
   sti_add_visa_equip_config_item_fn             sti_add_visa_equip_config_item;
   sti_add_ip_equip_config_item_fn               sti_add_ip_equip_config_item;
   sti_add_rs232_equip_config_item_fn            sti_add_rs232_equip_config_item;
   sti_clear_equip_config_fn                     sti_clear_equip_config;
   sti_load_database_configuration_fn            sti_load_database_configuration;
/* LOG */
   sti_request_single_test_log_fn                sti_request_single_test_log;
   sti_request_test_log_id_fn                    sti_request_test_log_id;
   sti_release_test_log_fn                       sti_release_test_log;
   sti_request_test_log_name_fn                  sti_request_test_log_name;
   sti_request_complete_test_log_fn              sti_request_complete_test_log;
/* PATH LOSS */
   sti_load_xml_rf_net_loss_fn                   sti_load_xml_rf_net_loss;
   sti_load_single_rf_net_loss_fn                sti_load_single_rf_net_loss;
   sti_load_multiple_rf_net_loss_fn              sti_load_multiple_rf_net_loss;
   sti_load_multiple_config_rf_net_loss_fn       sti_load_multiple_config_rf_net_loss;
   sti_clear_rf_net_loss_fn                      sti_clear_rf_net_loss;
/* SYSTEM */
   sti_set_log_filename_fn                       sti_set_log_filename;
   sti_set_environment_var_fn                    sti_set_environment_var;
   sti_get_environment_var_fn                    sti_get_environment_var;
   sti_register_system_event_handler_fn          sti_register_system_event_handler;
/* TEST */
   sti_execute_fn                                sti_execute;
   sti_clear_test_db_fn                          sti_clear_test_db;
   sti_add_test_fn                               sti_add_test;
   sti_add_parameter_fn                          sti_add_parameter;
   sti_get_test_list_fn                          sti_get_test_list;
   sti_execute_test_id_fn                        sti_execute_test_id;
   sti_execute_all_fn                            sti_execute_all;
   sti_execute_test_name_fn                      sti_execute_test_name;
   sti_close_handle_fn							 sti_close_handle;

} sti_export_func_decl;

/**
 * Initialize the interface function pointers to the provided DLL
 * @param asm_ptr Pointer to the DLL module.
 * @param exported Pointer to the function pointer structure.
 * @return int 1 when all interfaces were sucessfully resolved.
 *    Otherwise, 0 if one or more interface were found in the DLL.
 */
int sti_initialize_export_decl(HMODULE asm_ptr, sti_export_func_decl * exported);

#endif  /* __STI_EXPORT_FUNC_DECL__ */
