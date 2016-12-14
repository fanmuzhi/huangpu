#pragma once
#include "Syn_FingerprintTest.h"

class Ts_SharpnessTest : public Syn_FingerprintTest
{
public:
	Ts_SharpnessTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SharpnessTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();


	void get_sensor_matrices(AcqImgNoFingerResult* pFrames, SNRInfo *pInfo, SNRResults* pResult, float nofinger[MAXROW][MAXCOL], float finger[MAXROW][MAXCOL], int numRows, int numCols);

	void get_sharpness(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessResults* pSharpness);

	int find_min(float* pArray, int size);

	int find_max(float* pArray, int size);

	//differential along x and y - > Gx and Gy
	//gradient() will populate Gx and Gy matrices.
	void gradient(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int Gx[MAXROW][MAXCOL], int Gy[MAXROW][MAXCOL], SharpnessResults* pSharpness);
};

