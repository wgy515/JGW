//
// xxx.h
//
// $Id: xxx.h#3 $
//
// Library: 
// Package: 
// Module:  
//
// TDO:
//
// Copyright (c) 2021-2021
//
// SPDX-License-Identifier:	
//
#pragma once
#include <string>
namespace JGW
{
    class CTSE_ControlMsgBase
    {
    public:
        virtual void CloseControlMsg() = 0;
        /// 

        virtual bool ReadControlMsg(std::string& strRead) = 0;
        /// 读取log，无数据返回false

        virtual bool WriteControlMsg(const std::string& strWrite) = 0;
    };
}