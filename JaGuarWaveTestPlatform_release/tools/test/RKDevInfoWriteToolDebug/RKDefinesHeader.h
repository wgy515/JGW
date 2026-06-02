// defined with this macro as being exported.
#ifndef __RKDEFINES_H__
#define __RKDEFINES_H__

#define CMD_MEAS_BANDA 0
#define CMD_MEAS_BANDB 1
#define CMD_MEAS_BANDG 2
#define CMD_MEAS_BANDN 3
#define CMD_MEAS_BANDAC 4
#define CMD_MEAS_BANDAX 5
#define CMD_SET_CABLOS 6
#define CMD_STOP_GEN_RF 7
#define CMD_STOP_MEAS 8


#define MEAS_B50G 50
#define MEAS_B24G 24

#define MEAS_BW20 20
#define MEAS_BW40 40
#define MEAS_BW80 80

#pragma pack(1)

#define MAX_NUM_LOSS 60
typedef struct
{
    int count;//Attenuation node number
    float freq[MAX_NUM_LOSS]; //Frequency in MHz
    float loss[MAX_NUM_LOSS]; //Cable loss in dB
} AttenuationSetting;

typedef struct _TEST_PARAMS_ {
    wchar_t szDevName[260];
    wchar_t szwtestfw[260];
    wchar_t szdevicescript[260];
    AttenuationSetting *att[8];
    bool bLoadWtestfw;
    bool bForceRoot;
    bool bExitIfFail;
    bool btest2g;
    bool btest5g;
    bool bBanda_rx;
    bool bBanda_tx;

    bool bBandb_rx;
    bool bBandb_tx;

    bool bBandg_rx;
    bool bBandg_tx;

    bool bBandn_rx;
    bool bBandn_tx;

    bool bBandac_rx;
    bool bBandac_tx;

    bool bBandax_rx;
    bool bBandax_tx;

} TESTPARAMS;

typedef struct _MEAS_PARAM_
{
    int rate;
    int ss;
    int channel;
    float freq;
    int ant;
    int standard;
    int band;
    int bandwidth;
    float enp;        /* expected nominal power */
    int tx;
    float cycl;
    float rxlvl;
    char arbfile[260];
    char modulation[32];
} MEASPARAMS;

typedef struct _GPRF_PARAM_
{
    float rate;
    int channel;
    float freq;
    int ant;
    int band;
    int bandwidth;
    float enp;        /* expected nominal power */
    char arbfile[260];
} GPRFPARAMS;

typedef struct _MEAS_RES_
{
    int rate;
    int channel;
    int ant;
    double ClockError;
    double FreqError;
    double BurstPower;
    double EVMRMS;
    double EVMDataCarr;
    double IQOffset;
    int ntsm;
    double tsm[20];
} MEASRES;

typedef enum
{
    TS_OK = 0,
    TS_FAIL = 2,
    TS_START = 3,
} ENUM_TEST_STATUS;
/*
 */
typedef enum
{
    TEST_OPEN_DEVICE = 1,
    TEST_OPEN_DEVICE_FAIL = 2,
    TEST_OPEN_DEVICE_OK = 3,
    TEST_CLOSE_DEVICE = 4,

    TEST_OPEN_TESTER = 5,
    TEST_OPEN_TESTER_FAIL = 6,
    TEST_OPEN_TESTER_OK = 7,
    TEST_CLOSE_TESTER = 8,

    TEST_SET_DEVICE_ROOT = 9,
    TEST_SET_DEVICE_ROOT_FAIL = 10,
    TEST_SET_DEVICE_ROOT_OK = 11,
    TEST_PUSH_WTEST_FW = 12,
    TEST_SWITCH_WTEST = 13,
    TEST_BAND = 14,
    TEST_BAND_FAIL = 15,
    TEST_BAND_OKAY = 16,
} ENUM_TEST_PROMPT;

/* DEVICES things */
typedef struct {
    unsigned int type;
    unsigned short usVid;
    unsigned short usPid;
} RKDEVSCANINFO;

typedef struct __CALLBACK_H_
{
    int nStep;              //
    int nResult;            //pass:0 or fail:1
    void *userdata;
    char info[64];          //item name
    char message[128];      //Item description
    wchar_t messagew[128];  //Item description
} CALLBACK_PARAMS;

#pragma pack()

#define E_OK (0)
#define E_EEROR (-1)
#define E_DEVICE_BUSY (-2)
#define E_PARAM_INVALID (-3)
#define E_DEVICE_NOEXIST (-4)   /* device is not exist */
#define E_DEVICE_NOOPEN (-5)    /* device is not opened */
#define E_DEVICE_IO (-6)        /* io command fail */
#define E_SYSTEM_NOMEM (-7)     /* create buffer fail */
#define E_SYSTEM_THREAD (-8)    /* Create thread fail */
#define E_FILE_NOEXIST (-9)     /* file is not exist */
#ifdef __cplusplus
extern "C" {
#endif

/*
 *PFNLOGLISTENER must same as PFNLOG, else will case crash
 * typedef int (_stdcall *PFNLOG)(void *userdata, int dlLevel, const char *str,...);
 */
typedef int (_stdcall *PFNLOGLISTENER)(void *userdata, int dlLevel, const char *str,...);
typedef int (_stdcall *PFNSTATUS)(CALLBACK_PARAMS *p);

#ifdef __cplusplus
}
#endif

#endif /* __RKDEFINES_H__ end */
