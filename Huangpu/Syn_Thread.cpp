
//local
#include "Syn_Thread.h"

Syn_Thread::Syn_Thread()
: QThread()
, _stopped(true)
, _pSyn_Site(NULL)
, _bFinished(true)
, _strPreTestStepName("")
{
}

Syn_Thread::~Syn_Thread()
{
}

void Syn_Thread::run()
{
	if (NULL == _pSyn_Site)
		return;

	unsigned int iSiteNumber(0);
	_pSyn_Site->GetSiteNumber(iSiteNumber);

	uint32_t rc(0);

	if (1 == _iFlag)
	{
		rc = _pSyn_Site->Open();
		if (rc == 0)
		{
			Syn_DutTestResult * TestResult = NULL;

			rc = _pSyn_Site->ExecuteTestStep("InitializationStep");
			if (Syn_ExceptionCode::Syn_OK != rc)
			{
				emit send(iSiteNumber, "InitializationStep", "Fail");
				_pSyn_Site->Close();
				return;
			}
			else
			{
				emit send(iSiteNumber, "InitializationStep", "Pass");
			}

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PixelPatchTest");
			if (NULL != TestResult)
			{
				emit send(iSiteNumber, "PixelPatchTest", TestResult->_pixelPatchResults.m_bPass ? "Pass" : "Fail");
			}
			else
			{
				emit send(iSiteNumber, "PixelPatchTest", "Fail");
			}

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("WoVarTest");
			if (NULL != TestResult)
			{
				emit send(iSiteNumber, "WoVarTest", TestResult->_woVarResults.m_bPass ? "Pass" : "Fail");
			}
			else
			{
				emit send(iSiteNumber, "WoVarTest", "Fail");
			}

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("Calibrate");
			if (NULL != TestResult)
				emit send(iSiteNumber, "Calibrate", TestResult->_calibrationResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "Calibrate", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("AcqImgNoFinger");
			if (NULL != TestResult)
				emit send(iSiteNumber, "AcqImgNoFinger", "Pass");
			else
				emit send(iSiteNumber, "AcqImgNoFinger", "Fail");

			emit send(iSiteNumber);
		}
	}
	else if (2 == _iFlag)
	{
		if (rc == 0)
		{
			Syn_DutTestResult * TestResult = NULL;

			rc = _pSyn_Site->ExecuteTestStep("AcqImgFinger");
			emit send(iSiteNumber, "AcqImgFinger","Pass");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PeggedPixelsTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "PeggedPixelsTest", TestResult->_peggedPixelsResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "PeggedPixelsTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("FlooredPixelsTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "FlooredPixelsTest", TestResult->_flooredPixelsResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "FlooredPixelsTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("DRdyTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "DRdyTest", TestResult->_DRdyResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "DRdyTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PixelTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "PixelTest", TestResult->_pixelResults.bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "PixelTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("ConsecutivePixels");
			if (NULL != TestResult)
				emit send(iSiteNumber, "ConsecutivePixels", TestResult->_consecutivePixelsResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "ConsecutivePixels", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("CurrentTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "CurrentTest", TestResult->_currentResults.bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "CurrentTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("SNRTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "SNRTest", TestResult->_snrResults.bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "SNRTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("SharpnessTest");
			if (NULL != TestResult)
				emit send(iSiteNumber, "SharpnessTest", TestResult->_SharpnessResults.bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "SharpnessTest", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("RxStandardDev");
			if (NULL != TestResult)
				emit send(iSiteNumber, "RxStandardDev", TestResult->_RxStandardDevResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "RxStandardDev", "Fail");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("Imperfections");
			if (NULL != TestResult)
				emit send(iSiteNumber, "Imperfections", TestResult->_imperfectionsTestResults.m_bPass ? "Pass" : "Fail");
			else
				emit send(iSiteNumber, "Imperfections", "Fail");
			
			/*rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("OTPWriteMainSector");
			emit send(iSiteNumber, "OTPWriteMainSector");*/

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("FinalizationStep");
			if (NULL != TestResult)
				emit send(iSiteNumber, "FinalizationStep", "Pass");
			else
				emit send(iSiteNumber, "FinalizationStep", "Fail");

			emit send(iSiteNumber);

		}
	}
	else
	{
		//rc = _pSyn_Site->ExecuteTestStep("Calibrate");
	}

	_stopped = true;
}

void Syn_Thread::SetSite(Syn_Site *ipSyn_Site)
{
	if (NULL != ipSyn_Site)
		_pSyn_Site = ipSyn_Site;
}

void Syn_Thread::SetStopTag(bool stoptag)
{
	_stopped = stoptag;
}