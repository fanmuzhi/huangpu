#pragma once
#include "Syn_FingerprintTest.h"
class Ts_SNRTest : public Syn_FingerprintTest
{
public:
	Ts_SNRTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SNRTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();


	int get_signal_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol, int regions, SNRResults* results);
	
	float get_noise_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol, int regions, SNRResults* results);

	float find_noise(SNRResults * pRes, int S_ROW, int S_COL, int regions);

	void find_bins(SNRResults *pRes, int S_ROW, int S_COL, int bins[HISTOGRAM], int regions);

	int sum(int arr[HISTOGRAM], int firstindex, int lastindex);

	void SNR_Fill_Log(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRResults* pResults, int numRows, int numCols, int numFrames);

	float find_mean(float* arr, int S_COL);

	void get_span(int span[MAXFRAMES], int *min, int *max, int numFrames);
};

