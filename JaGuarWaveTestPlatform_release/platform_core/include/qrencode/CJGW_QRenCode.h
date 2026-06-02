#pragma once

typedef struct {
    int version;         ///< version of the symbol
    int width;           ///< width of the symbol
    unsigned char *data; ///< symbol data
} QRcode;

/**
* Encoding mode.
*/
//typedef enum {
//    QR_MODE_NUL = -1,   ///< Terminator (NUL character). Internal use only
//    QR_MODE_NUM = 0,    ///< Numeric mode
//    QR_MODE_AN,         ///< Alphabet-numeric mode
//    QR_MODE_8,          ///< 8-bit data mode
//    QR_MODE_KANJI,      ///< Kanji (shift-jis) mode
//    QR_MODE_STRUCTURE,  ///< Internal use only
//    QR_MODE_ECI,        ///< ECI mode
//    QR_MODE_FNC1FIRST,  ///< FNC1, first position
//    QR_MODE_FNC1SECOND, ///< FNC1, second position
//} QRencodeMode;

/**
* Level of error correction.
*/
//typedef enum {
//    QR_ECLEVEL_L = 0, ///< lowest
//    QR_ECLEVEL_M,
//    QR_ECLEVEL_Q,
//    QR_ECLEVEL_H      ///< highest
//} QRecLevel;

#if defined(__cplusplus)
extern "C" {
#endif

    //! (QRecLevel)level:def 0,(QRencodeMode)hint: def 2   version def = 0
    //! micro:def 0 £¬4¸öÎ¢ÐÍ°æ±¾ (mirco = 1,version < 4)
    extern void InitQRCode(int level,int hint,int version,int micro);
    //! size def:3
    extern QRcode* EncodeQRCode(const unsigned char *intext, size_t length,int size);
    //! 
    extern void FreeQRCode(QRcode* pQRcode);
#if defined(__cplusplus)
}
#endif

#ifndef JGW_QRENCODE_LIB_EXPORTS
#   define JGW_QRENCODE_LIB_IMPORTS
#endif

#ifdef JGW_QRENCODE_LIB_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"qrencode_d.lib")
#   else
#       pragma comment(lib,"qrencode.lib")
#   endif
#endif