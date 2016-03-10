
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
			rc = _pSyn_Site->ExecuteTestStep("Calibrate");
			emit send(iSiteNumber, "Calibrate");

			Syn_DutTestResult * TestResult = NULL;

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("AcqImgNoFinger");
			emit send(iSiteNumber, "AcqImgNoFinger");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PeggedPixelsTest");
			emit send(iSiteNumber, "PeggedPixelsTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("FlooredPixelsTest");
			emit send(iSiteNumber, "FlooredPixelsTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("DRdyTest");
			emit send(iSiteNumber, "DRdyTest");

			emit send(iSiteNumber);
		}
	}
	else if (2 == _iFlag)
	{
		if (rc == 0)
		{
			rc = _pSyn_Site->ExecuteTestStep("AcqImgFinger");
			emit send(iSiteNumber, "AcqImgFinger");

			Syn_DutTestResult * TestResult = NULL;
			
			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("SNRTest");
			emit send(iSiteNumber, "SNRTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PixelTest");
			emit send(iSiteNumber, "PixelTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("SharpnessTest");
			emit send(iSiteNumber, "SharpnessTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("Imperfections");
			emit send(iSiteNumber, "Imperfections");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("ConsecutivePixels");
			emit send(iSiteNumber, "ConsecutivePixels");

			/*rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("PixelPatchTest");
			emit send(iSiteNumber, "PixelPatchTest");*/

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("WoVarTest");
			emit send(iSiteNumber, "WoVarTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("CurrentTest");
			emit send(iSiteNumber, "CurrentTest");

			rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("RxStandardDev");
			emit send(iSiteNumber, "RxStandardDev");

			/*rc = _pSyn_Site->GetTestResult(TestResult);
			rc = _pSyn_Site->ExecuteTestStep("OTPWriteMainSector");
			emit send(iSiteNumber, "OTPWriteMainSector");*/

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