/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/SuiteTestServer/Manager/StmInterface.h#16 $
 * $DateTime: 2016/09/02 16:50:21 $
 *
 * DESCRIPTION: StmInterface
 ******************************************************************************
 *
 * Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#ifndef __SUITE_TEST_MANAGER_INTERFACE__
#define __SUITE_TEST_MANAGER_INTERFACE__

#ifdef SUITETESTMANAGER_EXPORTS
#undef SUITETESTMANAGER_API
#define SUITETESTMANAGER_API __declspec(dllexport)
#else
#undef SUITETESTMANAGER_API
#define SUITETESTMANAGER_API __declspec(dllimport)
#endif

#ifdef SUITETESTMANAGER_STATIC
#undef SUITETESTMANAGER_API
#define SUITETESTMANAGER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

   /* Max string size */
   const int STM_MAX_STRING_SIZE = 512;

   /**
    * Information about the assembly stored by the manager
    */
   typedef struct stm_assembly_info
   {
      /** Index assigned to the assembly */
      int index;
      /** The name of the assembly */
      char name [STM_MAX_STRING_SIZE];
      /** The absolute file and path of the assembly */
      char fileAndPath [STM_MAX_STRING_SIZE];
   } stm_assembly_info;

   /**
    * Factory function to obtain a pointer to the suite test StmInterface.
    * @return void* Pointer to the interface
    */
   SUITETESTMANAGER_API void * stm_create_suite_manager_interface(void);

   /**
    * Release the memory allocated for the StmInterface.
    * @param iface Address of pointer to th suite manager StmInterface.
    */
   SUITETESTMANAGER_API void stm_release_suite_manager_interface(void** iface);

   /**
    * Add a reference assembly to the manager.
    * @param iface Pointer to StmInterface
    * @param name The user defined name for the assembly.
    * @param fileAndPath The absolute path and file name of the reference assembly.
    * @return int '1' if the reference was added successfully, '0' otherwise.
    */
   SUITETESTMANAGER_API int stm_add_reference_assembly(void * iface, const char * name, const char * fileAndPath);

   /**
    * Create the named application and get the application information.
    * @param iface Pointer to StmInterface
    * @param name The user defined name for the assembly.
    * @param info Pointer to storage for assembly information.
    * @return int '1' if application was created successfully, '0' otherwise.
    */
   SUITETESTMANAGER_API int stm_create_application(void * iface, const char * name, stm_assembly_info * info);

   /**
    * Clears and unloads all managed assemblies.
    * @param iface Pointer to StmInterface
    */
   SUITETESTMANAGER_API void stm_clear_assembly_tbl(void * iface);

//
//  CONFIGURATION *************************************************************
//
   /**
    * Adds a Library fileName that may be used to accompany the assembly.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information.
    * @param fileName Only the file name of the library to be loaded (must be adjacent to the assembly).
    * @return int '1' if the dll was added successfully, '0' otherwise.
    */
   SUITETESTMANAGER_API void stm_add_ancillary_library(void * iface, const stm_assembly_info * asmInfo, const char * fileName);

   /**
    * Adds a station configuration item
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param itemGroup 0=StringItems, 1=DoubleItems, 2=LongItems, 3=BoolItems
    * @param name Name of the configuration item
    * @param value Value of the configuration item
    */
   SUITETESTMANAGER_API void stm_add_station_config_item(void * iface, const stm_assembly_info * asmInfo, int itemGroup, const char * name, char * value);

   /**
    * Clears the station configuration contents
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    */
   SUITETESTMANAGER_API void stm_clear_station_config(void * iface, const stm_assembly_info * asmInfo);

   /**
    * Adds a GPIB equipment to the configuration container
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param identifier Equipment identifier, see QDART help
    * @param name Equipment name, see QDART help
    * @param type Equipment type, see QDART help
    * @param pad Primary GPIB address of the equipment
    * @param sad (optional) Secondary GPIB address of the equipment
    * @param options Equipment options, see QDART help
    */
   SUITETESTMANAGER_API void stm_add_gpib_equip_config_item(void * iface, const stm_assembly_info * asmInfo,
      const char * identifier, const char * name, const char * type,
      const char * pad, const char * sad, const char * options);

   /**
    * Adds a VISA equipment to the configuration container
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param identifier Equipment identifier, see QDART help
    * @param name Equipment name, see QDART help
    * @param type Equipment type, see QDART help
    * @param address VISA address of the equipment
    * @param options Equipment options, see QDART help
    */
   SUITETESTMANAGER_API void stm_add_visa_equip_config_item(void * iface, const stm_assembly_info * asmInfo,
      const char * identifier, const char * name, const char * type,
      const char * address, const char * options);

   /**
    * Adds an IP equipment to the configuration container
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param identifier Equipment identifier, see QDART help
    * @param name Equipment name, see QDART help
    * @param type Equipment type, see QDART help
    * @param instrument (optional) Instrument number
    * @param address IP address of the equipment
    * @param options Equipment options, see QDART help
    */
   SUITETESTMANAGER_API void stm_add_ip_equip_config_item(void * iface, const stm_assembly_info * asmInfo, const char * identifier,
      const char * name, const char * type, const char * instrument, const char * address,
      const char * options);

   /**
    * Adds a serial equipment to the configuration container
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param identifier Equipment identifier, see QDART help
    * @param name Equipment name, see QDART help
    * @param type Equipment type, see QDART help
    * @param comport COM port number of the equipment
    * @param options Equipment options, see QDART help
    */
   SUITETESTMANAGER_API void stm_add_rs232_equip_config_item(void * iface, const stm_assembly_info * asmInfo, const char * identifier,
      const char * name, const char * type, const char * comport, const char * options);

   /**
    * Clear all equipment stored in the configuration container
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    */
   SUITETESTMANAGER_API void stm_clear_equip_config(void * iface, const stm_assembly_info * asmInfo);

   /**
    * Manually load the database XML configuration files
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    */
   SUITETESTMANAGER_API void stm_load_database_configuration(void * iface, const stm_assembly_info * asmInfo);

//
// LOG ************************************************************************
//

   /**
    * Request the log of the last test executed
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param mode The mode to transform the data log to
    * @param value The testlog. If no last test exists, empty string will be returned.
    *    This string must be freed by caller.
    */
   SUITETESTMANAGER_API void stm_request_single_test_log(void * iface, const stm_assembly_info * asmInfo, int mode, char ** value);

   /**
    * Request the log of the specified test id
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param mode The mode to transform the data log to
    * @param testId The test ID of the log to request
    * @param value The testlog. If no last test exists, empty string will be returned.
    *   This string must be freed by caller.
    */
   SUITETESTMANAGER_API void stm_request_test_log_id(void * iface, const stm_assembly_info * asmInfo, int mode, int testId, char ** value);

   /**
    * Release the log acquired with stm_request_single_test_log or stm_request_test_log_id
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param value char buffer to be released
    */
   SUITETESTMANAGER_API void stm_release_test_log(void * iface, const stm_assembly_info * asmInfo, char ** value);

   /**
    * Request the log of the specified test name.  On string collisions,
    * the log of the first test with the name will be returned.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param mode The mode to transform the data log to
    * @param testName The test name of the log to request
    * @param value The testlog. If no last test exists, empty string will be returned.
    *   This string must be freed by caller.
    */
   SUITETESTMANAGER_API void stm_request_test_log_name(void * iface, const stm_assembly_info * asmInfo, int mode, const char * testName, char ** value);

   /**
    * Request the log of the complete test log.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param mode The mode to transform the data log to
    * @param value The testlog. If no last test exists, empty string will be returned.
    *   This string must be freed by caller.
    */
   SUITETESTMANAGER_API void stm_request_complete_test_log(void * iface, const stm_assembly_info * asmInfo, int mode, char ** value);

//
//  PATH LOSS *****************************************************************
//

   /**
    * Uses Xml file to load loss value to the calibration
    * manager.  Frequency and losses will be stored in a their respective table that
    * shall be specific to the technologies and RF path specified in the Xml.
    *
    * @param iface Pointer to StiInterface
    * @param fileName file path to the Xml file
    */
   SUITETESTMANAGER_API void stm_load_xml_rf_net_loss(void * iface, const stm_assembly_info * asmInfo, const char * fileName);

   /**
    * Adds a single RF network frequency vs loss value to the calibration
    * manager.  Frequency and losses will be stored in a single table that
    * shall be agnostic to all technologies and RF path selections.
    *
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param freqMhz Frequency in Mhz
    * @param lossDb RF loss in dB
    */
   SUITETESTMANAGER_API void stm_load_single_rf_net_loss(void * iface, const stm_assembly_info * asmInfo, double freqMhz, double lossDb);

   /**
    * Adds a path specific RF network frequency vs loss value to the calibration
    * manager.  Frequency and losses shall be stored in path specific tables.
    *
    * Tables can be differentiated between unique RF network paths, like PRx/DRx or
    * Main/Aux.
    *
    * The default path numbers are defined by the SUITE tool in a XML string
    * declared as "g__configXMLStr".  However the path numbers can be overriden
    * using station config items.
    *
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param path Path number
    * @param freqMhz Frequency in Mhz
    * @param lossDb RF loss in dB
    */
   SUITETESTMANAGER_API void stm_load_multiple_rf_net_loss(void * iface, const stm_assembly_info * asmInfo, int path, double freqMhz, double lossDb);

   /**
    * Adds a path specific RF network frequency vs loss value to the calibration
    * manager.  Frequency and losses shall be stored in both configuration technology
    * and path specific tables.
    *
    * Tables can be differentiated between unique configuration names and
    * RF network paths.  Configuration names are strings such as WCDMA, CDMA, GSM, etc...
    * These strings are normally declared as a parameter of the "SetCalConfigUtility" test.
    *
    * The default path numbers are defined by the SUITE tool in a XML string
    * declared as "g__configXMLStr".  However the path numbers can be overriden
    * using station config items.
    *
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param path Path number
    * @param freqMhz Frequency in Mhz
    * @param lossDb RF loss in dB
    */
   SUITETESTMANAGER_API void stm_load_multiple_config_rf_net_loss(void * iface, const stm_assembly_info * asmInfo, const char * configName, int path, double freqMhz, double lossDb);

   /**
    * Clears all loss tables in the calibration manager.
    *
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    */
   SUITETESTMANAGER_API void stm_clear_rf_net_loss(void * iface, const stm_assembly_info * asmInfo);

//
//  SYSTEM ********************************************************************
//

   /**
    * Sets the SUITE XML log filename.  This is required by the tool to resolve,
    * the working both the source of the test data and the working path directory
    * of that source.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param name The name and path of the QSPR XTT
    */
   SUITETESTMANAGER_API void stm_set_log_filename(void * iface, const stm_assembly_info * asmInfo, const char * name);

   /**
    * Set an environment variable to the server.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param key Environment variable name
    * @param value Environment variable value
    */
   SUITETESTMANAGER_API void stm_set_environment_var(void * iface, const stm_assembly_info * asmInfo, const char * key, const char * value);

   /**
    * Get an environment variable from the server.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param key Environment variable name
    * @return std::string Environment variable value
    */
   SUITETESTMANAGER_API char * stm_get_environment_var(void * iface, const stm_assembly_info * asmInfo, const char * key);

   /**
    * Register a callback function to handle system events.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param context If using C++ and classes, context is the this pointer.
    * @param evtcb The callback function to be invoked when a message is received.
    *          The callback receives the following parameters:
    *             context - The same context that was specified when registering the callback
    *             type    - The message type (1) Debug message (2) Log message (3) error message (4) TPL message (5) HTML message
    *             msg     - A constant pointer to the string message. The pointer is valid until the callback returns. After that,
    *                       the pointer is deallocated. If the message is needed after the callback returns, users should make a copy of the string.
    */
   SUITETESTMANAGER_API void stm_register_system_event_handler(void * iface, const stm_assembly_info * asmInfo, void * context, void(*evt_handler)(void * context, int type, const char * msg));

//
//  TEST **********************************************************************
//
   /**
    * DEPRECATED
    * Execute a test using the provided test information.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param testname The name of the test node as described by the QSPR XTT
    * @param realname The name of the class associated with the test node
    * @param arrParamKeys A string array of parameter names associated with the test
    * @param keysCount The number of strings in the key array.
    * @param arrParamValues A string array of parameter values associated with the test
    * @param valuesCount The number of strings in the value array.
    * @return int The result of the test
    */
   SUITETESTMANAGER_API int stm_execute(void * iface, const stm_assembly_info * asmInfo, const char * testname, const char * realname,
      const char ** arrParamKeys, int keysCount,
      const char ** arrParamValues, int valuesCount);

   /**
    * Clear the test information datastructure in the server.
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    */
   SUITETESTMANAGER_API void stm_clear_test_db(void * iface, const stm_assembly_info * asmInfo);

    /**
    * Adds a test to the test information datastructure in the server
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param testName The name of the test node as described by the QSPR XTT
    * @param realName The name of the class associated with the test node
    * @return int A unique ID which the server associated with the test entry
    */
   SUITETESTMANAGER_API int stm_add_test(void * iface, const stm_assembly_info * asmInfo, const char * testName, const char * realName);

    /**
    * Adds a parameter to a test in the test information datastructure in the server
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param testId The unique id which the server used to associate with the test
    * @param parameterName The name of the parameter
    * @param parameterValue The value of the parameter
    * @param upperlimit The upper limit of the parameter.  This parameter is only required for
    * parameters with paramMode specified as "Output" and is not used for "Input" parameters.
    * The limit value should be represented in the same units and format as parameterValue.  When a limit
    * is specified for an output parameter, the result of the test is compared against the specified
    * limit bounds.  If the test result lies within the limit bounds, a PASS determination shall be
    * made; otherwise the test shall report FAIL.
    * @param lowerlimit The lower limit of the parameter.  This parameter is only required for
    * parameters with paramMode specified as "Output" and is not used for "Input" parameters.
    * The limit value should be represented in the same units and format as parameterValue.  When a limit
    * is specified for an output parameter, the result of the test is compared against the specified
    * limit bounds.  If the test result lies within the limit bounds, a PASS determination shall be
    * made; otherwise the test shall report FAIL.
    * @param paramMode Specifies if the parameter mode is input and/or output.  Acceptable strings shall
    * be, "Input" or "Output".  Input parameters drive the test exection, where-as output parameters are
    * used by tests to populate test results.  Using output parameters, test PASS/FAIL determination can
    * be based on test return values compared to the limit bounds.
    */
   SUITETESTMANAGER_API void stm_add_parameter(void * iface, const stm_assembly_info * asmInfo, int testId, const char * parameterName,
      const char * parameterValue, const char * upperlimit,
      const char * lowerlimit, const char * paramMode);

   /**
    * Gets the test list as stored by the server
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param verbose A verbose output will include a listing of test parameters and other relavant information.
    *    0 = NOT verbose; 1 = verbose;
    * @return std::vector<std::string> A string array of the test list
    */
   SUITETESTMANAGER_API char** stm_get_test_list(void * iface, const stm_assembly_info * asmInfo, int verbose, char ** arrTestList, int * count);

   /**
    * Execute a test based on the server assigned unique test id
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param testId The unique id which the server used to associate with the test
    * @return int Result of the test
    */
   SUITETESTMANAGER_API int stm_execute_test_id(void * iface, const stm_assembly_info * asmInfo, int testId);

   /**
    * Execute all tests stored by the server in id sequence
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @return int Result of the test
    */
   SUITETESTMANAGER_API int stm_execute_all(void * iface, const stm_assembly_info * asmInfo);

   /**
    * Execute a test by the test name as stored by the server
    * @param iface Pointer to StmInterface
    * @param asmInfo Pointer to the assembly information
    * @param testName The name of the test.  If multiple tests of the
    *   same name appear more than once, then only the first occurance
    *   shall be executed.
    * @return int Result of the test
    */
   SUITETESTMANAGER_API int stm_execute_test_name(void * iface, const stm_assembly_info * asmInfo, const char * testName);

#ifdef __cplusplus
}   // extern "C"
#endif


#endif

