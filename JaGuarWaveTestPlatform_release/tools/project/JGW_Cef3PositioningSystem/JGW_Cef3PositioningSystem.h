#pragma once

#include "resource.h"
#include <string>
//! DXCS BLE通讯TX端口
#define DXCS_BLE_TX_UDP_SOCKET_PORT 9000
//! DXCS BLE 通讯RX端口 === 本机端口
#define LOCAL_UDP_SOCKET_PORT 9100
//! 
#define DXCS_RECEIVE_BUFFER_MAX_LENGTH (1024 * 10)

typedef void (*pOnRecvBuffer)(const std::string& strRecvBuffer);
//! 无效纬度信息
#define INVALID_LATITUDE -99999.0f
//! 无效纬度信息
#define INVALID_LONGITUDE -99999.0f
//! 无效距离信息
#define INVALID_DISTANCE -1.0f
//! 无效偏航角
//#define INVALID_YAW -99999.0f
//! 无效偏航角
#define INVALID_YAW_F 99999.0f
//!altitude1
#define INVALI_ALTITUDE -99999.0f
// 定位设备GATT UUID
#define DXCS_GATT_UUID "5833ff01-9b8b-5191-6142-22a4536ef123"

//! 更新经纬度时间间隔
#define UPDATE_LNGLAT_TIMERS 20
//! 更新偏航角时间间隔
#define UPDATE_YAW_TIMERS 5
//! 更新RMQ消息时间间隔
#define UPDATE_RABBITMQ_MSG 25
// 更新时延单位50MS
#define UPDATE_DELAY_UNIT_MSEC 100

typedef struct
{
	/**
	* 计算两点方向偏航角 第二点到第一点偏航角
	*/
	double mCalcRevAzimuth;
	/**
	* 计算两点的俯仰角===根据海拔高度差
	*/
	double mCalcPitch;
	/**
	* 计算两点的俯仰角===根据海拔高度差
	*/
	double mCalcRevPitch;
	/**
	* 计算两点之间距离
	*/
	double mCalcDistance;
	/**
	* 计算两点的偏航角,第一点到第二点偏航角
	*/
	double mCalcAzimuth;
} S_VINCENTY_AzimuthPitch;

/// <summary>
/// LongitudeLatitude经纬度
/// </summary>
#define GNSS_MODULE_LONGITUDELATITUDE 0x21
/// <summary>
/// world magnetic model https://blog.csdn.net/weixin_40525909/article/details/105809756
/// https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#declination
/// 磁偏角
/// </summary>
#define GNSS_MODULE_WMM 0x23
/// <summary>
/// GNSS原始的数据
/// </summary>
#define GNSS_MODULE_LONGITUDELATITUDE_RAW 0x24
/// <summary>
/// yaw, pitch and roll
/// </summary>
#define MOTION_MODULE_MFX  0x41
/// <summary>
/// X axis / Y axis gravity axis  pitch_3x-roll_3x-phi_3x / theta-psi-phi _3x
/// </summary>
#define MOTION_MODULE_MTL 0x42
/// <summary>
/// altitude
/// </summary>
#define MOTION_MODULE_ALTITUDE  0x43
/// <summary>
/// 电压毫伏
/// </summary>
#define MOTION_MODULE_VOLTAGEMV  0x44
/// <summary>
/// 地磁重新校准
/// </summary>
#define MOTION_MODULE_MAGCALIBRATE  0x45
/// <summary>
/// pressure calibrate 海平面气压校准
/// </summary>
#define MOTION_MODULE_PRESSURECALIBRATE  0x46
/// <summary>
/// 设置海平面气压值
/// </summary>
#define MOTION_MODULE_SETTING_SEALEVEL_PRESSURE 0x47



/********************* RabbitMQ *****************************************/
/**
*  RabbitMQ exchange
*/
#define LOCATION_EXCHANGE "location-exchange"
/**
* RabbitMQ user name
*/
#define USER_NAME_RABBITMQ "admin"
/**
* RabbitMQ password
*/
#define  PASSWORD_RABBITMQ "jgw_loc1"
/**
* RabbitMQ host 1.116.200.229
*/
#define HOST_RABBITMQ  "39.107.247.200"
/**
* RabbitMQ routingKey == location-routingkey
* 房间ID
*/
#define ROUTINGKEY_RABBITMQ "hola"

typedef enum
{
	TAG_RMQ_UNKNOWN = -1,
	/**
	* 标识A
	*/
	TAG_RMQ_A = 0,
	/**
	* 标识B
	*/
	TAG_RMQ_B,
	/**
	* 标识观察模式
	*/
	TAG_RMQ_VIEW
} E_TAG_RMQ;

typedef enum
{
	/**
	* 纬度、经度
	*/
	LATITUDELONGITUDE_TYPE = 0,
	/**
	* 海拔高度
	*/
	ALTITUDE_TYPE,
	/**
	* 欧拉角
	*/
	EULERANGLE_TYPE,

	/*********************简写************************************************/
	/**
	* LATITUDELONGITUDE_TYPE 简写 : 未使用
	*/
	LLT,
	/**
	* ALTITUDE_TYPE 简写 : 未使用
	*/
	AT,
	/**
	* EULERANGLE_TYPE 简写 : 未使用
	*/
	ET,
	/**
	* 未知类型
	*/
	UNKNOWN_TYPE
}E_RabbitMQDataType;

