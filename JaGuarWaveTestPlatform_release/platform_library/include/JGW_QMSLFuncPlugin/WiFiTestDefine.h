#pragma once
#include <vector>
namespace JGW
{
    struct S_WLAN_PARAM_INFO
    {
        const char* szIndex;
        const char* szParamDes;
    };
    //! txMode
    const S_WLAN_PARAM_INFO gvTxModeParamInfo[] = {{"0","Disable Tx"},
    {"1","Cont. Unmod Tx(Sine)"},
    {"2","Cont. Mod Tx Frame"},
    {"3","Cont. Tx TX99"},
    {"4","Cont. Tx TX100"}
    }; 
    //! channel  2412 2417 2422

    //! tpcm 
    /*

    For "ForcedGain", use string "1".
    For "TxPowerAuto", use string "2".
    For "TxPowerForce_CLPC", use string ¡°0¡±.
    For "TxPowerForce_SCPC", use string ¡°0¡±.
    For "TxPowerForce_OLPC", use string ¡°0¡±.
    For "ForcedDesiredGain", use string "4".
    */
    const S_WLAN_PARAM_INFO gvTxPowerControlInfo[] = {
        {"0","TxPowerForce_CLPC/TxPowerForce_SCPC/TxPowerForce_OLPC"},
        {"1","ForcedGain"},
        {"2","TxPowerAuto"},
        {"3",""},
        {"4","ForcedDesiredGain"}
    };
    //! txPower0"

    //! wlanMode
    /*
    No HT: No Channel Bonding
    HT20: 20 MHz bandwidth 802.11n
    HT40+: 40 MHz bandwidth, primary high, 802.11n
    HT40: 40 MHz bandwidth, primary low, 802.11n
    CCK: CCK bandwidth
    VHT20: 20 MHz bandwidth 802.11ac
    VHT40+: 40 MHz bandwidth, primary high, 802.11n
    VHT40-: 40 MHz bandwidth, primary low, 802.11n
    VHT80_0: 80 MHz bandwidth 20/40 MHz low, 40/80 MHz low
    VHT80_1: 20/40 MHz low, 40/80 MHz high
    VHT80_2: 20/40 MHz high, 40/80 MHz low
    VHT80_3: 20/40 MHz

    For "No HT", use data string "0".
    For "HT20", use data string "1".
    For "HT40+", use data string "2".
    For "HT40-", use data string "3".
    For "CCK", use data string "4".
    For "VHT20", use data string "5".
    For "VHT40+", use data string "6".
    For "VHT40-", use data string "7".
    For "VHT80_0", use data string "8".
    For "VHT80_1", use data string "9".
    For "VHT80_2", use data string "10".
    For "VHT80_3", use data string "11".
    For "DSRC10", use data string "0".
    */  

    const S_WLAN_PARAM_INFO gvHTModeInfo[] = {
        {"0","No HT/DSRC10"},{"1","HT20"},{"2","HT40+"},{"3","HT40-"},{"4","CCK"},{"5","VHT20"},
        {"6","VHT40+"},{"7","VHT40-"},{"8","VHT80_0"},{"9","VHT80_1"},{"10","VHT80_2"},
        {"11","VHT80_3"}
    };

   
    //! rateBitIndex0  b CCK def: 6 /g def No HT: 15  /n def: 23 HT20 40M n def : 31 /ac VHT20 def: 145
    /*
    Use ¡°0¡± for rate "RATE_1Mbps_L" ( HT mode ¡°CCK¡± )
    Use ¡°1¡± for rate "RATE_2Mbps_L" ( HT mode ¡°CCK¡± )
    Use ¡°2¡± for rate "RATE_2Mbps_S" ( HT mode ¡°CCK¡± )
    Use ¡°3¡± for rate "RATE_5_5Mbps_L" ( HT mode ¡°CCK¡± )
    Use ¡°4¡± for rate "RATE_5_5Mbps_S" ( HT mode ¡°CCK¡± )
    Use ¡°5¡± for rate "RATE_11Mbps_L" ( HT mode ¡°CCK¡± )
    Use ¡°6¡± for rate "RATE_11Mbps_S" ( HT mode ¡°CCK¡± )
    Use ¡°8¡± for rate "RATE_6Mbps" ( HT mode ¡°No HT" )
    Use ¡°9¡± for rate "RATE_9Mbps" ( HT mode ¡°No HT" )
    Use ¡°10¡± for rate "RATE_12Mbps" ( HT mode ¡°No HT" )
    Use ¡°11¡± for rate "RATE_18Mbps" ( HT mode ¡°No HT" )
    Use ¡°12¡± for rate "RATE_24Mbps" ( HT mode ¡°No HT" )
    Use ¡°13¡± for rate "RATE_36Mbps" ( HT mode ¡°No HT" )
    Use ¡°14¡± for rate "RATE_48Mbps" ( HT mode ¡°No HT" )
    Use ¡°15¡± for rate "RATE_54Mbps" ( HT mode ¡°No HT" )
    Use ¡°16¡± for rate "RATE_MCS_0_20" ( HT mode ¡°HT20" )
    Use ¡°17¡± for rate "RATE_MCS_1_20" ( HT mode ¡°HT20" )
    Use ¡°18¡± for rate "RATE_MCS_2_20" ( HT mode ¡°HT20" )
    Use ¡°19¡± for rate "RATE_MCS_3_20" ( HT mode ¡°HT20" )
    Use ¡°20¡± for rate "RATE_MCS_4_20" ( HT mode ¡°HT20" )
    Use ¡°21¡± for rate "RATE_MCS_5_20" ( HT mode ¡°HT20" )
    Use ¡°22¡± for rate "RATE_MCS_6_20" ( HT mode ¡°HT20" )
    Use ¡°23¡± for rate "RATE_MCS_7_20" ( HT mode ¡°HT20" )
    Use ¡°32¡± for rate "RATE_MCS_8_20" ( HT mode ¡°HT20" )
    Use ¡°33¡± for rate "RATE_MCS_9_20" ( HT mode ¡°HT20" )
    Use ¡°34¡± for rate "RATE_MCS_10_20" ( HT mode ¡°HT20" )
    Use ¡°35¡± for rate "RATE_MCS_11_20" ( HT mode ¡°HT20" )
    Use ¡°36¡± for rate "RATE_MCS_12_20" ( HT mode ¡°HT20" )
    Use ¡°37¡± for rate "RATE_MCS_13_20" ( HT mode ¡°HT20" )
    Use ¡°38¡± for rate "RATE_MCS_14_20" ( HT mode ¡°HT20" )
    Use ¡°39¡± for rate "RATE_MCS_15_20" ( HT mode ¡°HT20" )
    Use ¡°48¡± for rate "RATE_MCS_16_20" ( HT mode set to ¡°HT20" )
    Use ¡°49¡± for rate "RATE_MCS_17_20" ( HT mode set to ¡°HT20" )
    Use ¡°50¡± for rate "RATE_MCS_18_20" ( HT mode set to ¡°HT20" )
    Use ¡°51¡± for rate "RATE_MCS_19_20" ( HT mode set to ¡°HT20" )
    Use ¡°52¡± for rate "RATE_MCS_20_20" ( HT mode set to ¡°HT20" )
    Use ¡°53¡± for rate "RATE_MCS_21_20" ( HT mode set to ¡°HT20" )
    Use ¡°54¡± for rate "RATE_MCS_22_20" ( HT mode set to ¡°HT20" )
    Use ¡°55¡± for rate "RATE_MCS_23_20" ( HT mode set to ¡°HT20" )
    Use ¡°64¡± for rate "RATE_AC_MCS_0_20" ( HT mode set to ¡°VHT20" )
    Use ¡°65¡± for rate "RATE_AC_MCS_1_20" ( HT mode set to ¡°VHT20" )
    Use ¡°66¡± for rate "RATE_AC_MCS_2_20" ( HT mode set to ¡°VHT20" )
    Use ¡°67¡± for rate "RATE_AC_MCS_3_20" ( HT mode set to ¡°VHT20" )
    Use ¡°68¡± for rate "RATE_AC_MCS_4_20" ( HT mode set to ¡°VHT20" )
    Use ¡°69¡± for rate "RATE_AC_MCS_5_20" ( HT mode set to ¡°VHT20" )
    Use ¡°70¡± for rate "RATE_AC_MCS_6_20" ( HT mode set to ¡°VHT20" )
    Use ¡°71¡± for rate "RATE_AC_MCS_7_20" ( HT mode set to ¡°VHT20" )
    Use ¡°72¡± for rate "RATE_AC_MCS_8_20" ( HT mode set to ¡°VHT20" )
    Use ¡°73¡± for rate "RATE_AC_MCS_9_20" ( HT mode ¡°VHT20" )
    Use ¡°100¡± for rate "RATE_AC_MCS_10_20" ( HT mode ¡°VHT20" )
    Use ¡°101¡± for rate "RATE_AC_MCS_11_20" ( HT mode ¡°VHT20" )
    Use ¡°102¡± for rate "RATE_AC_MCS_12_20" ( HT mode ¡°VHT20" )
    Use ¡°103¡± for rate "RATE_AC_MCS_13_20" ( HT mode set to ¡°VHT20" )
    Use ¡°104¡± for rate "RATE_AC_MCS_14_20" ( HT mode set to ¡°VHT20" )
    Use ¡°105¡± for rate "RATE_AC_MCS_15_20" ( HT mode set to ¡°VHT20" )
    Use ¡°106¡± for rate "RATE_AC_MCS_16_20" ( HT mode set to ¡°VHT20" )
    Use ¡°107¡± for rate "RATE_AC_MCS_17_20" ( HT mode ¡°VHT20" )
    Use ¡°108¡± for rate "RATE_AC_MCS_18_20" ( HT mode ¡°VHT20" )
    Use ¡°109¡± for rate "RATE_AC_MCS_19_20" ( HT mode ¡°VHT20" )
    Use ¡°136¡± for rate "RATE_AC_MCS_20_20" ( HT mode ¡°VHT20" )
    Use ¡°137¡± for rate "RATE_AC_MCS_21_20" ( HT mode ¡°VHT20" )
    Use ¡°138¡± for rate "RATE_AC_MCS_22_20" ( HT mode ¡°VHT20" )
    Use ¡°139¡± for rate "RATE_AC_MCS_23_20" ( HT mode ¡°VHT20" )
    Use ¡°140¡± for rate "RATE_AC_MCS_24_20" ( HT mode ¡°VHT20" )
    Use ¡°141¡± for rate "RATE_AC_MCS_25_20" ( HT mode ¡°VHT20" )
    Use ¡°142¡± for rate "RATE_AC_MCS_26_20" ( HT mode ¡°VHT20" )
    Use ¡°143¡± for rate "RATE_AC_MCS_27_20" ( HT mode ¡°VHT20" )
    Use ¡°144¡± for rate "RATE_AC_MCS_28_20" ( HT mode ¡°VHT20" )
    Use ¡°145¡± for rate "RATE_AC_MCS_29_20" ( HT mode ¡°VHT20" )
    Use ¡°24¡± for rate "RATE_MCS_0_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°25¡± for rate "RATE_MCS_1_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°26¡± for rate "RATE_MCS_2_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°27¡± for rate "RATE_MCS_3_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°28¡± for rate "RATE_MCS_4_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°29¡± for rate "RATE_MCS_5_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°30¡± for rate "RATE_MCS_6_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°31¡± for rate "RATE_MCS_7_40" ( HT mode set to ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°40¡± for rate "RATE_MCS_8_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°41¡± for rate "RATE_MCS_9_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°42¡± for rate "RATE_MCS_10_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°43¡± for rate "RATE_MCS_11_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°44¡± for rate "RATE_MCS_12_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°45¡± for rate "RATE_MCS_13_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°46¡± for rate "RATE_MCS_14_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°47¡± for rate "RATE_MCS_15_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°56¡± for rate "RATE_MCS_16_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°57¡± for rate "RATE_MCS_17_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°58¡± for rate "RATE_MCS_18_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°59¡± for rate "RATE_MCS_19_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°60¡± for rate "RATE_MCS_20_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°61¡± for rate "RATE_MCS_21_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°62¡± for rate "RATE_MCS_22_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°63¡± for rate "RATE_MCS_23_40" ( HT mode ¡°HT40+" or ¡°HT40-¡° )
    Use ¡°76¡± for rate "RATE_AC_MCS_0_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°77¡± for rate "RATE_AC_MCS_1_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°78¡± for rate "RATE_AC_MCS_2_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°79¡± for rate "RATE_AC_MCS_3_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°80¡± for rate "RATE_AC_MCS_4_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°81¡± for rate "RATE_AC_MCS_5_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°82¡± for rate "RATE_AC_MCS_6_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°83¡± for rate "RATE_AC_MCS_7_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°84¡± for rate "RATE_AC_MCS_8_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°85¡± for rate "RATE_AC_MCS_9_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°112¡± for rate "RATE_AC_MCS_10_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°113¡± for rate "RATE_AC_MCS_11_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°114¡± for rate "RATE_AC_MCS_12_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°115¡± for rate "RATE_AC_MCS_13_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°116¡± for rate "RATE_AC_MCS_14_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°117¡± for rate "RATE_AC_MCS_15_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°118¡± for rate "RATE_AC_MCS_16_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°119¡± for rate "RATE_AC_MCS_17_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°120¡± for rate "RATE_AC_MCS_18_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°121¡± for rate "RATE_AC_MCS_19_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°148¡± for rate "RATE_AC_MCS_20_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°149¡± for rate "RATE_AC_MCS_21_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°150¡± for rate "RATE_AC_MCS_22_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°151¡± for rate "RATE_AC_MCS_23_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°152¡± for rate "RATE_AC_MCS_24_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°153¡± for rate "RATE_AC_MCS_25_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°154¡± for rate "RATE_AC_MCS_26_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°155¡± for rate "RATE_AC_MCS_27_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°156¡± for rate "RATE_AC_MCS_28_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°157¡± for rate "RATE_AC_MCS_29_40" ( HT mode ¡°VHT40+"or¡°VHT40-¡° )
    Use ¡°88¡± for rate "RATE_AC_MCS_0_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°89¡± for rate "RATE_AC_MCS_1_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°90¡± for rate "RATE_AC_MCS_2_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°91¡± for rate "RATE_AC_MCS_3_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°92¡± for rate "RATE_AC_MCS_4_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°93¡± for rate "RATE_AC_MCS_5_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°94¡± for rate "RATE_AC_MCS_6_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°95¡± for rate "RATE_AC_MCS_7_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°96¡± for rate "RATE_AC_MCS_8_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°97¡± for rate "RATE_AC_MCS_9_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°124¡± for rate "RATE_AC_MCS_10_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°125¡± for rate "RATE_AC_MCS_11_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°126¡± for rate "RATE_AC_MCS_12_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°127¡± for rate "RATE_AC_MCS_13_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°128¡± for rate "RATE_AC_MCS_14_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°129¡± for rate "RATE_AC_MCS_15_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°130¡± for rate "RATE_AC_MCS_16_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°131¡± for rate "RATE_AC_MCS_1780" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°132¡± for rate "RATE_AC_MCS_18_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°133¡± for rate "RATE_AC_MCS_19_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°160¡± for rate "RATE_AC_MCS_20_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°161¡± for rate "RATE_AC_MCS_21_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°162¡± for rate "RATE_AC_MCS_22_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°163¡± for rate "RATE_AC_MCS_23_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°164¡± for rate "RATE_AC_MCS_24_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°165¡± for rate "RATE_AC_MCS_25_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°166¡± for rate "RATE_AC_MCS_26_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°167¡± for rate "RATE_AC_MCS_27_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°168¡± for rate "RATE_AC_MCS_28_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°169¡± for rate "RATE_AC_MCS_29_80" ( HT mode ¡°VHT80_x"; x[0:3]¡° )
    Use ¡°8¡± for rate "RATE_P_3Mbps_10" ( HT mode "DSRC10")
    Use ¡°9¡± for rate "RATE_P_4p5Mbps_10" ( HT mode "DSRC10")
    Use ¡°10¡± for rate "RATE_ P_6Mbps"_10 ( HT mode "DSRC10")
    Use ¡°11¡± for rate "RATE_ P_9Mbps_10" ( HT mode "DSRC10")
    Use ¡°12¡± for rate "RATE_ P_12Mbps_10" ( HT mode "DSRC10")
    Use ¡°13¡± for rate "RATE_ P_18Mbps_10" ( HT mode "DSRC10")
    Use ¡°14¡± for rate "RATE_ P_24Mbps_10" ( HT mode "DSRC10")
    Use ¡°15¡± for rate "RATE_ P_27Mbps_10" ( HT mode "DSRC10")
    */
    // const S_WLAN_PARAM_INFO gvDataRateInfo[] = {};

    //! Tx Pattern  txPattern def : ZEROS 0
    //! Set Short Guard setting
    //!  Set Short Guard setting shortGuard  def 0

    //! Set Aggregate Setting Number of aggregation frames. Enter value from 1-32/ agg def 1

    //! ifs def : 0
    //! Enter # of Packets numPackets def 0

    //! Set Enable/Disable the Scrambler scramblerOff def 0

    //!aifsn def 0 Enter the ASIFN timing parameter. ASIFN is Arbitration Inter Frame Spacing timing parameter. Enter the number of slots [0-252] between frame transmissions.
    //!pktLen0 def 1500 Enter Packet Size (in bytes)
    //! antenna def 0 Enter Tx Antenna
    /* txChain0  //! Set Tx Chain setting
    Use ¡°1¡± for TxChain0.
    Use ¡°2¡± for TxChain1.
    Use ¡°3¡± for TxChain01.
    Use ¡°4¡± for TxChain2.
    Use ¡°5¡± for TxChain02.
    Use ¡°6¡± for chain 1 and chain2 ¨C but not available from QRCT.
    Use ¡°7¡± for TxChain012.
    */
    const S_WLAN_PARAM_INFO gvTxChain0Info[] = {
        {"0",""},{"1","TxChain0"},{"2","TxChain1"},{"3","TxChain01"},{"4","TxChain2"},{"5","TxChain02"},{"6","for chain 1 and chain2 ¨C but not available from QRCT"},{"7","TxChain012"}
    };
    //! gainIdx def 9 Gain Index
    //! dacGain def 0 Set DAC Gain
    //! "paConfig" def 0 Enter PA CFG
    //! broadcast def 0 Select broadcast/Unicast
    const S_WLAN_PARAM_INFO gvBroadCaseInfo[] = {{"0","Unicast"},{"1","BroadCast"}};

    /* flags
    Enable/Disable LDPC
    QLIB_FTM_WLAN_TLV_AddParam( hResourceContext, "flag", ¡°26¡± );
    (0x18 + 0x00000002) = 26 (decimal) when LDPC is checked (Enabled).
    Note: The programming of LDPC should be combined with STBC and DPDmode.
    25.
    Enable/Disable STBC
    QLIB_FTM_WLAN_TLV_AddParam( hResourceContext, "flag", ¡°25¡± );
    (0x18 + 0x00000001) = 25 (decimal) when STBC is checked (Enabled).
    Note: The programming of STBC should be combined with LDPC and DPDmode.
    26.
    Enable/Disable DPDmode
    QLIB_FTM_WLAN_TLV_AddParam( hResourceContext, "flag", ¡°30¡± );
    (0x18 + 0x00000004) = 30 (decimal) when STBC is checked (Enabled).
    Note: The programming of DPDmode should be combined with LDPC and STBC
    */





    //! rxMode
    const S_WLAN_PARAM_INFO gvRxModeParamInfo[] = {{"0","All Frames"},
    {"1","Filter (match MAC only)"},
    }; 









}
