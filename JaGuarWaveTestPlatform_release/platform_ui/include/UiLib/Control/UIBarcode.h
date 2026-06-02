#pragma once
#include "Barcode.hpp"
namespace UiLib
{
    enum E_BARCODE_TYPE
    {
        BARCODE_TYPE_39 = 0,
        //! 
        BARCODE_TYPE_93,
        //! 数字、大写字母和控制字符组成的字符串，如ABC、ABC123
        BARCODE_TYPE_128A,
        //! 数字、大小写字母和字符组成的字符串，如Abc123、a-123（B）
        BARCODE_TYPE_128B,
        //! 双位数字组成的字符串，如1234、00008182。
        BARCODE_TYPE_128C,
        //! 
        BARCODE_TYPE_I2of5,
        //! 
        BARCODE_TYPE_QR
    };

    class UILIB_API CBarcodeUI : public CContainerUI
    {
    public:
        CBarcodeUI();
        virtual ~CBarcodeUI();
    public:
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        void PaintBkImage(HDC hDC);
        //! 条码宽高比值
        void SetBarcodeRatio(float fBarcodeRatio);
        float GetBarcodeRatio();

        void SetBarcodeWidth(int nBarcodeWidth);
        int GetBarcodeWidth() const;
        //! 
        void SetText(LPCTSTR pstrText);
        CDuiString GetText() const;

        void AutoAdjustWidthHeight();
        void SetAutoAdjustWidthHeight(bool bAutoAdjustWidthHeight);
        bool GetAutoAdjustWidthHeight() const;

        void SetBarcodeType(E_BARCODE_TYPE eBarcodeType);
        E_BARCODE_TYPE GetBarcodeType() const;
    private:
        bool mbAutoAdjustWidthHeight;

        int mnQRecLevel;
        int mnQRencodeMode;
        int mnQRecVersion;
        int mnQRecMicro;

        int mnBarcodeWidth;
        float mfBarcodeRatio;
        CDuiString mstrBarcode;
        E_BARCODE_TYPE meBarcodeType;
        Barcode39 mBarcode39;
        Barcode93 mBarcode93;
        BarcodeI2of5 mBarcodeI2of5;
        Barcode128 mBarcode128;
        BarcodeQR mBarcodeQR;
    };
}