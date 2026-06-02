// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_MESDataBase.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include "CJGW_MESDataBaseAdo.h"

namespace JGW
{
    CCJGW_MESDataBaseAdo* gpAdo = NULL;
    CCJGW_MESDataBaseImpl* GetMESDataBaseAdo()
    {
        if (NULL == gpAdo) gpAdo = new CCJGW_MESDataBaseAdo;
        return gpAdo;
    }

    void DestoryMESDataBaseAdo()
    {
        if (gpAdo)
        {
            delete gpAdo;
            gpAdo = NULL;
        }
    }
}