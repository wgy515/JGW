#include "StdAfx.h"
#include <qrencode/CJGW_QRenCode.h>

#include "qrencode.h"
#include <string.h>
#include <stdlib.h>
static int casesensitive = 1;
static int eightbit = 0;
static int version = 0;
//static int size = 3;
static int margin = 5;
static int dpi = 72;
static int structured = 0;
static int rle = 0;
static int svg_path = 0;
static int micro = 0;
static QRecLevel level = QR_ECLEVEL_H;
static QRencodeMode hint = QR_MODE_8;
static unsigned char fg_color[4] = {0, 0, 0, 255};
static unsigned char bg_color[4] = {255, 255, 255, 255};

static int verbose = 0;

void InitQRCode(int lev,int nhint,int ver,int mic)
{
    version = ver;
    level = (QRecLevel)lev;
    hint = (QRencodeMode)nhint;
    micro = mic;
}

QRcode* EncodeQRCode(const unsigned char *intext, size_t length,int size /* = 3 */)
{
    QRcode *code;

    if(micro) {
        if(eightbit) {
            code = QRcode_encodeDataMQR(length, intext, version, level);
        } else {
            code = QRcode_encodeStringMQR((char *)intext, version, level, hint, casesensitive);
        }
    } else if(eightbit) {
        code = QRcode_encodeData(length, intext, version, level);
    } else {
        code = QRcode_encodeString((char *)intext, version, level, hint, casesensitive);
    }

    return code;
}

void FreeQRCode(QRcode* pQRcode)
{
    QRcode_free(pQRcode);
}