/****************************************************************************

Copyright 2016 Peraso Technologies Inc. ALL RIGHTS RESERVED.

This Information is proprietary to Peraso Technologies Inc., Toronto, Ontario
(Peraso) and MAY NOT be copied by any method or incorporated into another
program without the express written consent of Peraso. This Information	or any
portion thereof remains the property of Peraso. The Information contained herein
is believed to be accurate and Peraso assumes no responsibility or liability for
its use in any way and conveys no license or title under any patent or copyright
and makes no representation or warranty that this Information is free from
patent or copyright infringement.



File: prs_version.h
Contains: $content$

$Id: //sw_peraso/trunk/utility/prs_download_flash/src/prs_version.h#2 $

@author: Bryan Huang
@date: 2016-3-3

***************************************************************************/

#ifndef __PRS_VERSION_H__
#define __PRS_VERSION_H__

#ifdef PRS_REV_FILE
#include "prs_revision.h"
#else
#define PRS_REV_NUM PRS_SVN_REV
#endif

///@brief version string
/// @details the length is limited to maximum 14 characters due to buffer constraint
/// <Version>.<rel_num>.<yymm>.<branch_level>.<svn_rev>
/// Where <branch_level> is:
/// 0 - trunk 1 - release branch 2, 3, 4 - patch releases from a release branch

#define PRS_BUILD_YYMM  "1612"
#define PRS_BUILD_VERSION_STRING "A.1." PRS_BUILD_YYMM

#define PRS_SPACER_COMPONENT_STRING " "
#define PRS_SPACER_SVN_REVISION_STRING "."

#define PRS_UMAC_COMPONENT_STRING      "Falcon UMAC"
#define PRS_FW_COMPONENT_STRING        "Falcon Firmware"
#define PRS_HOST_COMPONENT_STRING      "Falcon Host"
#define PRS_ATMEL_COMPONENT_STRING     "Peraso Atmel"
#define PRS_FLASH_BIN_COMPONENT_STRING "prs_flash_bin"



#define PRS_BUILD_YEAR ((int)PRS_BUILD_YYMM/100)
#define PRS_BUILD_MONTH ((int)PRS_BUILD_YYMM%100)

#define PRS_FORMAT_VERSION_INFO   "%s" PRS_SPACER_COMPONENT_STRING "%s" PRS_SPACER_SVN_REVISION_STRING "%s"


#define PRS_FORMAT_TAG_INFO         " %s"
#define PRS_FORMAT_EXTRA_INFO       "%s %s %s\n"



// PDB version defination.
// During release this should match the version defined in the base PDB templates (xml) files.
// these should reflect the values as defined in PRS_BUILD_YYMM, since the the macro expansion causes issues where these are used.
#define cPdb_DB_VER_TAG_DB_INCR     0
#define cPdb_DB_VER_TAG_FW_INCR     0
#define cPdb_DB_VER_TAG_MONTH       10
#define cPdb_DB_VER_TAG_YEAR        16


#endif // PRS_VERSION_H

