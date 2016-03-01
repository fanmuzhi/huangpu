#include "Ts_AcqImgNoFinger.h"


Ts_AcqImgNoFinger::Ts_AcqImgNoFinger(string &strName, string &strArgs,  Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_AcqImgNoFinger::~Ts_AcqImgNoFinger()
{
}


void Ts_AcqImgNoFinger::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	//parse args

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (!_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("ImageAcqPatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
}


void Ts_AcqImgNoFinger::Execute()
{
	//image to discard

	//acqire user-specified number of images

}


void Ts_AcqImgNoFinger::ProcessData()
{

}


void Ts_AcqImgNoFinger::CleanUp()
{

}