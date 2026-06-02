#ifndef UIChart_h__
#define UIChart_h__

#pragma once
#pragma warning(disable:4251)
#include <vector>
#include <string>

namespace UiLib
{
	class CChartViewUI;

	typedef enum LocationAlign
	{
		LOCATION_TOP,
		LOCATION_RIGHT,
		LOCATION_BOTTOM,
	}lalign;
	
	typedef enum ChartAlignment
	{
		CHART_HORIZONTAL,
		CHART_VERTICAL
	}calignment;

	class UILIB_API CChartFloatItem
	{
	public:
		CChartFloatItem(float _DataValue = NULL):m_iValue(_DataValue){};
		~CChartFloatItem(void){};

	public:
		void SetDataValue(float _DataValue){m_iValue = _DataValue;};
		float GetDataValue(){return m_iValue;};

	private:
		float m_iValue;
	};

	class UILIB_API CChartItem
	{
	public:
		CChartItem(double _DataValue = NULL):m_iValue(_DataValue){};
		~CChartItem(void){};

	public:
		void SetDataValue(double _DataValue){m_iValue = _DataValue;};
		double GetDataValue(){return m_iValue;};

	private:
		double m_iValue;
	};

	class UILIB_API CChartSeries
	{
	public:
		CChartSeries(LPCTSTR _LegendText,LPCTSTR _SeriesColorA,LPCTSTR _SeriesColorB = NULL);
		CChartSeries(LPCTSTR _LegendText,DWORD _SeriesColorA,DWORD _SeriesColorB);
		~CChartSeries(void);

	public:
		void SetLegendText(LPCTSTR _LegendText);
		LPCTSTR GetLegendText();
		void SetSeriesColor(LPCTSTR _SeriesColorA,LPCTSTR _SeriesColorB);
		void SetSeriesColor(DWORD _SeriesColorA,DWORD _SeriesColorB);
		DWORD GetSeriesColorA();
		DWORD GetSeriesColorB();
		
		bool AddSeriesData(CChartItem* _ChartItem);
		bool AddSeriesData(double _DataValue);
		bool SetSeriesData(int _Index,CChartItem* _ChartItem);
		bool SetSeriesData(int _Index,double _DataValue);
		bool InsertSeriesData(int _Index,CChartItem* _ChartItem);
		bool InsertSeriesData(int _Index,double _DataValue);
		bool RemoveSeriesDataAt(int _Index);
		void RemoveSeriesDataAll();

		CChartItem* GetSeriesData(int _Index);
		int  GetDataCount();

		void SetVisible(bool _Visible);
		bool GetVisible();

		void SetChartViewObj(CChartViewUI* _pChartViewUI);
		void Invalidate();

	private:
		bool				m_bIsVisible;
		UINT				m_uSeriesFondId;
		DWORD				m_dSeriesColorA;
		DWORD				m_dSeriesColorB;
		CDuiString			m_sLegendText;
		CChartViewUI*		m_pChartView;
		TStdPtrArray<CChartItem*>	m_rSeriesData;
	};

	class CZQChartViewUI;

	class UILIB_API ZQCChartXYAxis
	{
	public:
		ZQCChartXYAxis( std::vector<std::wstring>&,std::vector<std::wstring>& );
		~ZQCChartXYAxis( void );

	public:
		//! 自适应
		void DrawAxis(HDC hDC, const RECT& rcPaint,RECT& rcItem);
		void SetChartViewObj(CZQChartViewUI* _pChartViewUI);
		void Invalidate();

		//UINT GetXChartLabelsCount();//! 获取X轴字符显示数量

		UINT GetYChartLabelsCount();//! 获取Y轴字符显示 数量

	public:
		bool SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
// 		void AddLabel( LPCTSTR _DataLabel ); //! 添加X轴字符显示
// 		void AddYLabel( LPCTSTR _DataLabel ); //! 添加Y轴字符显示
// 		void RemoveLabel();
// 		void RemoveYLabel();

// 		void ResetTick(){}
// 
// 		void SetLegendMaxWidth(UINT _LegendMaxWidth){}
// 		UINT GetLegendMaxWidth(){return 0;}
// 		void SetLegendMaxHeight(UINT _LegendMaxHeight){}
// 		UINT GetLegendMaxHeight(){return 0;}
// 		void CalMaxAxisLabelWidth(HDC hDC,CChartSeries* _ChartSeries){}
// 		void CalMaxDataLabelWidth(HDC hDC,CChartSeries* _ChartSeries){}
// 		void CalAxisDataLimits(CChartSeries* _ChartSeries){}
// 		void SetMaxDataLabelWidth(UINT _DataLabelWidth){}
// 		UINT GetMaxDataLabelWidth(){ return 0;}
// 		UINT GetGroupCount(){return 1;}
	private:
		int			m_nLineWidth; //! 线宽度
		DWORD		m_uLineColor;//! 线颜色

		UINT		m_uXYAxisFontId; //！ 显示字体ID
		UINT		m_uXYAxisFontColor;//! 显示字体颜色

		UINT		m_nChartWidth;//! 绘图界面宽
		UINT		m_nChartHeight;//! 绘图界面高度

		std::vector<std::wstring>&	m_rXChartLabels;

		std::vector<std::wstring>&	m_rYChartLabels;
		/*
		_____________________
		|  ________________  |
		| |m_rcChartPadding| |
		| |________________| |
		|____________________|
		*/
		RECT		m_rcChartPadding; //! 图例的内边距
// 		UINT		m_uOriginLeftSpacing;//! 坐标原点距离左边间距
// 		UINT		m_uOriginBottonSpacing; //! 坐标原点距离底部间距

		UINT		m_uXYAxisLineSize;//! 刻度线条大小
		UINT		m_uXYAxisLineColor;//! 刻度线条颜色

		bool		m_bVisibleBaseLine;//! 是否显示基线
		UINT		m_uBaseLineSize;//! 基线大小
		UINT		m_uBaseLineColor; //! 基线颜色

// 		UINT		m_uXAxisCount;	//! X轴刻度分割 等分 默认按照5份等分
// 		UINT		m_uYAxisCount;	//! Y轴刻度分割 等分 默认按照5份等分

		UINT		m_uXMajorScaleLineSize; //! X轴主刻度线大小
		UINT		m_uXMinorScaleLineSize; //! X轴次刻度线大小


// 		UINT		m_uXAxisChartMax; //! X轴显示最大的数值（最大的数值+ 最小的数值）
// 		UINT		m_uYAxisChartMax; //! Y轴显示最大的数值（最大的数值+ 最小的数值）

		//float		m_fXPerAxis; //!   每一個坐标点代表的数值
		float		m_fYPerAxis;//!  每一個坐标点代表的数值

 		//UINT		m_uXPerChartAxis; //! 每一隔刻度代表的坐标值 X
 		UINT		m_uYPerChartAxis;//! 每一隔刻度代表的坐标值 Y

		//UINT		m_uXPerScalePoints; //! 每隔刻度多少个点

		CZQChartViewUI*		m_pChartView;
		CDuiString	m_strXUint;
		CDuiString	m_strYUint;
	};

	class UILIB_API CChartXYAxis
	{
	public:
		CChartXYAxis(void);
		~CChartXYAxis(void);

	public:
		void SetTickLimis(int _MinTick,int _MaxTick,UINT _StepTick);
		void SetMinTick(int _MinTick);
		void SetMaxTick(int _MaxTick);
		void SetStepTick(int _StepTick = 0);
		int  GetMinTick();
		int  GetMaxTick();
		UINT GetSetpTick();
		void ResetTick();

		void AddLabel(LPCTSTR _DataLabel);
		void AddCustomTick(int _TickValue);
		void SetVisibleCoustomTick(bool _Visible);
		bool GetVisibleCoustomTick();

		void SetXYAxisFontId(int _FontId);
		int  GetXYAxisFontId();
		void SetXYAxisLineSize(UINT _LineSize);
		UINT GetXYAxisLineSize();
		void SetLegendMaxWidth(UINT _LegendMaxWidth);
		UINT GetLegendMaxWidth();
		void SetLegendMaxHeight(UINT _LegendMaxHeight);
		UINT GetLegendMaxHeight();
		int  GetMaxGraphWidth();
		int  GetMaxGraphHeight();
		void SetMaxAxisLabelWidth(UINT _AxisLabelWidth);
		UINT GetMaxAxisLabelWidth();
		void SetMaxAxisLabelHeight(UINT _AxisLabelHeight);
		UINT GetMaxAxisLabelHeight();
		void SetMaxDataLabelWidth(UINT _DataLabelWidth);
		UINT GetMaxDataLabelWidth();
		void SetLegendLocation(lalign _LegendLocation);
		lalign GetLegendLocation();
		void SetXYAxisLineColor(DWORD _LineColor);
		void SetXYAxisLineColor(LPCTSTR _LineColor);
		DWORD GetXYAxisLineColor();
		void CalLegendSize(HDC hDC);
		void CalMaxAxisLabelWidth(HDC hDC,CChartSeries* _ChartSeries);
		void CalMaxDataLabelWidth(HDC hDC,CChartSeries* _ChartSeries);
		void CalAxisDataLimits(CChartSeries* _ChartSeries);
		void RemoveAllDataLabel();
		UINT GetGroupCount();
		UINT GetTickCount();
		DWORD AddColor(DWORD _SrcValue,DWORD _AddValue);
		DWORD CutColor(DWORD _SrcValue,DWORD _CurValue);
		void SetVisibleBaseLine(bool _Visible);
		bool GetVisibleBaseLine();

		void SetChartViewObj(CChartViewUI* _pChartViewUI);
		void Invalidate();
		void DrawAxis(HDC hDC, const RECT& rcPaint,RECT& rcItem);

	private:
		bool				m_bVisibelCustomTick;
		bool				m_bVisibleBaseLine;
		int					m_iMaxGraphWidth;
		int 				m_iMaxGraphHeight;
		int					m_iMinTick;
		int					m_iMaxTick;
		int					m_iMinTickEx;
		int					m_iMaxTickEx;
		UINT				m_uStepTick;
		UINT				m_uCountLabel;
		UINT				m_uMaxAxisLabelWidth;
		UINT				m_uMaxAxisLabelHeight;
		UINT				m_uMaxDataLabelWidth;
		UINT				m_uXYAxisFontId;
		UINT				m_uXYAxisLineSize;
		UINT				m_uLegendMaxWidth;
		UINT				m_uLegendMaxHeight;
		int					m_iAxissCale;
		int					m_iMinBaseLine;
		int					m_iMaxBaseLine;
		double				m_dDataScale;
		lalign				m_uLegendLocation;
		lalign				m_uLabelLocation;
		calignment			m_uAlignment;
		DWORD				m_dXYAxisLineColor;
		RECT				m_rcChartPadding;
		CDuiString			m_sLegendTitel;
		CChartViewUI*		m_pChartView;
		TStdValArray<int>	m_rDataAxis;
		TStdValArray<int>	m_rDataCustomAxis;
		TStdValArray<CDuiString,LPCTSTR>	m_rChartLabels;
	};

	class UILIB_API CChartViewUI : public CHorizontalLayoutUI
	{
	public:
		CChartViewUI(void);
		~CChartViewUI(void);

		LPCTSTR GetClass() const;
		LPVOID  GetInterface(LPCTSTR pstrName);
		void	DoPaint(HDC hDC, const RECT& rcPaint);

	public:
		void AddLabel(LPCTSTR _DataLabel);
		void AddSeries(CChartSeries* _ChartSeries);
		CChartSeries* GetSeries(LPCTSTR _LegendText);
		CChartSeries* GetSeries(int nIndex);
		CDuiString GetSeriesName(int nIndex);
		void SetVisibleSeries(LPCTSTR _LegendText,bool _Visible);
		bool GetVisibleSeries(LPCTSTR _LegendText);
		void CalLegendSize(CChartSeries* _ChartSeries );
		CChartXYAxis& GetXYAxis();
		//ZQCChartXYAxis& GetXYAxis();
		void RemoveSeries(int _Index);
		void RemoveSeries(CChartSeries* _ChartSeries);
		void RemoveAllData();
		UINT GetGroupCount();
		UINT GetSeriesCount();
	private:
		CChartXYAxis						m_cXYAxis;
		//ZQCChartXYAxis						m_cXYAxis;
		TStdPtrArray<CChartSeries*>			m_rChartSeries;
	};

	class UILIB_API CZQChartViewUI : public CHorizontalLayoutUI
	{
	public:
		CZQChartViewUI(void);
		~CZQChartViewUI(void);

		LPCTSTR GetClass() const;
		LPVOID  GetInterface(LPCTSTR pstrName);
		void	DoPaint(HDC hDC, const RECT& rcPaint);

		//! series
		void	AddSeriesData(float _DataValue);
		float	GetSeriesData( int nIndex );
		UINT	GetSeriesCount();
		void	ClearSeriesData();
		//！获取显示起点
		UINT	GetSeriesShowStartIndex();
		void	SetSeriesShowStartIndex( UINT nIndex = 0 );
		void	MoveSeriesShowStartIndex( bool bLeft );
		void	ResetSeriesShowStartIndex();

		void	AddLabel( LPCTSTR _DataLabel ); //! 添加X轴字符显示
		void	AddYLabel( LPCTSTR _DataLabel ); //! 添加Y轴字符显示
		void	RemoveLabel();
		void	RemoveYLabel();

		void	SetXTwoPointsInterval( UINT xTwoPointsInterval );
		UINT	GetXTwoPointsInterval();//! 获取X轴，前一个点与后一个点之间的间距

		void	SetYMajorScaleUnit( float dData ); //! 设置Y轴主刻度代表的单位
		float	GetYMajorScaleUnit( );//! 获取Y轴主刻度代表的单位

		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		UINT	GetXPerScalePoints();//!获取每隔刻度多少个点
		void	SetXScoreCount( UINT xScoreCount );//! 设置X轴等分的数量
		UINT	GetXScoreCount(); //! 获取等分数
		UINT	GetMaxXPointsCount(); //! 获取当前坐标最大的点数

		bool	GetVisibleGridlines();

		void	SetVisibleGridlines( bool bVisibleGridlines );

	private:
		bool								m_bVisibleGridlines;
		float								m_fMajorScaleUnit;
		//! float								m_fPerXChartDataCount;
		UINT								m_dShowStartIndex;
		UINT								m_uXTwoPointsInterval; //! X轴 两个点之间的间隔
		std::vector<std::wstring>			m_rXChartLabels;
		std::vector<std::wstring>			m_rYChartLabels;
		std::vector<float>					m_rSeriesData;
		ZQCChartXYAxis						m_cXYAxis;
		UINT								m_uXPerScalePoints; //! 每隔刻度多少个点
		UINT								m_xScoreCount;
		RECT								m_sScoreRect;
		UINT								m_uMaxXPointsCount;
		UINT								m_uMaxOldXPointsCount;
	};
}

#endif // UIChart_h__
