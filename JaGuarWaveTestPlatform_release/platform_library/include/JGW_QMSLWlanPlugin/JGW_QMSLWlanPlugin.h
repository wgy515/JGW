#include "JGW_QMSLWlanPlugin_Define.h"

#ifdef JGW_QMSLWLANPLUGIN_EXTERN_
extern "C" 
{
#endif
    //! 
    JGW_QMSLWLANPLUGIN_API void InitWlanConfigParam(const char* szParamNodeName);
    //! 更新TX配置参数
    JGW_QMSLWLANPLUGIN_API void UpdateWlanConfigParam(const char* szParamName,const char* szParamValue);
    //! 开启TX
    JGW_QMSLWLANPLUGIN_API bool SetWlanTxOn();
    //! 关闭TX
    JGW_QMSLWLANPLUGIN_API void SetWlanTxOff();
    //! 开启RX
    JGW_QMSLWLANPLUGIN_API bool SetWlanRxOn();
    //! 获取WLAN测试结果并关闭RX
    JGW_QMSLWLANPLUGIN_API bool GetWlanRxResult(float* per,float* perPower,int* rssi);
#ifdef JGW_QMSLWLANPLUGIN_EXTERN_
}
#endif
