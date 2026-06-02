#include "StdAfx.h"
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
namespace JGW
{
	CCJGW_CHiPerfTimer::CCJGW_CHiPerfTimer(void)
	{
		if (QueryPerformanceFrequency(&freq) == false)
		{
			//!throw "QueryPerformanceFrequency";
		}
		InitTime();
	}


	CCJGW_CHiPerfTimer::~CCJGW_CHiPerfTimer(void)
	{
	}

	void CCJGW_CHiPerfTimer::InitTime()
	{
		startTime.QuadPart = 0;
		stopTime.QuadPart = 0;
	}

	// Start the timer

	void CCJGW_CHiPerfTimer::Start()
	{
		QueryPerformanceCounter(&startTime);
	}

	// Stop the timer

	void CCJGW_CHiPerfTimer::Stop()
	{
		QueryPerformanceCounter(&stopTime);
	}

	double CCJGW_CHiPerfTimer::GetTimeMs()
	{
		return ((double)(stopTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart) *1000;
	}
}


