/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/SubSysVFS2.0/SubSysVFS_Core/QSEQ_CORE/Core/QSEQ_Debug.h#3 $
 * $DateTime: 2018/02/05 13:40:25 $
 *
 * DESCRIPTION: Definition for debug information
 ******************************************************************************
 *
 * Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#define QSEQ_MEMORY_LEAK_CHECK
#if 0
#ifdef _DEBUG

//#define QSEQ_MEMORY_LEAK_CHECK

// http://support.microsoft.com/kb/140858
// http://www.codeguru.com/forum/showthread.php?t=312742

#ifdef QSEQ_MEMORY_LEAK_CHECK
#pragma message ( "******Including QSEQ_Debug.h..." )
#define MYDEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//allocations to be of _CLIENT_BLOCK type
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new MYDEBUG_NEW
#endif
#endif
#endif
