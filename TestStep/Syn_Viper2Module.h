#pragma once
#include "Syn_ViperModule.h"
class Syn_Viper2Module : public Syn_ViperModule
{
public:
	Syn_Viper2Module();
	virtual ~Syn_Viper2Module();

	void ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames);

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);


private:
	//CDM16 conversion	
	static float m_matrix[144][144];
};

