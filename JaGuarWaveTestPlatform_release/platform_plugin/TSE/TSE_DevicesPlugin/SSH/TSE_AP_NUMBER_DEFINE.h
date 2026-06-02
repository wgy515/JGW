#pragma once

namespace JGW
{
    struct s_ap_info 
    {
        char sn[32];           //! 以 '\0'为结束符
        char mac[32];          //! 以 '\0'为结束符, "B6:4C:84:36:DC:3D"
        char ssid_60G[32];     //! 以 '\0'为结束符, "Leopard_60G_36DC3D"
        char password_60G[32]; //! 以 '\0'为结束符, "123456789"
        char reserve[896];     //预留字段，ap.bin总文件大小1KB
    };
}