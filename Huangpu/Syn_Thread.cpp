
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

	/*if (1 == _iFlag)
	{
		rc = _pSyn_Site->Open();
		if (rc == 0)
		{
			Syn_DutTestResult * TestResult = NULL;

			rc = _pSyn_Site->ExecuteTestStep("InitializationStep");
			if (Syn_ExceptionCode::Syn_OK != rc)
			{
				emit send(iSiteNumber, "InitializationStep", "Fail");
				emit send(iSiteNumber);
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

			Syn_Site::SiteState oState;
			_pSyn_Site->GetState(oState);
			if (Syn_Site::Error == oState)
			{
				return;
			}

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
			
			//rc = _pSyn_Site->GetTestResult(TestResult);
			//rc = _pSyn_Site->ExecuteTestStep("OTPWriteMainSector");
			//emit send(iSiteNumber, "OTPWriteMainSector");

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
	}*/



	std::vector<std::string> listOfTestStepName;
	_pSyn_Site->GetTestStepList(listOfTestStepName);
	if (0 == listOfTestStepName.size())
		return;

	unsigned int imageNoFingerPos(0);
	unsigned int imageFingerPos(0);
	for (size_t i = 0; i < listOfTestStepName.size(); i++)
	{
		if (std::string("AcqImgNoFinger") == listOfTestStepName[i])
		{
			imageNoFingerPos = i;
		}
		else if (std::string("AcqImgFinger") == listOfTestStepName[i])
		{
			imageFingerPos = i;
		}
	}
	if (0 == imageNoFingerPos || 0 == imageFingerPos)
		return;

	if (1 == _iFlag)
	{
		Syn_DutTestResult *pTestResult = NULL;

		rc = _pSyn_Site->Open();
		if (rc == 0)
		{
			for (size_t t = 0; t <= imageNoFingerPos; t++)
			{
				if (0 == t)
				{
					rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
					if (Syn_ExceptionCode::Syn_OK != rc)
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Fail");
						emit send(iSiteNumber);
						return;
					}
					else
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Pass");
					}
				}
				else if (t!=imageNoFingerPos)
				{
					_pSyn_Site->GetTestResult(pTestResult);

					rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
					if (Syn_ExceptionCode::Syn_TestStepConfigError == rc)
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "NULL");
						emit send(iSiteNumber);
						return;
					}
					else if (Syn_ExceptionCode::Syn_OK != rc)
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Fail");
					}
					else
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]));
					}
				}
				else
				{
					_pSyn_Site->GetTestResult(pTestResult);
					rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]) );
				}
			}

			emit send(iSiteNumber);
		}
	}
	else if (2 == _iFlag)
	{
		Syn_DutTestResult *pTestResult = NULL;
		if (rc == 0)
		{
			Syn_Site::SiteState oState;
			_pSyn_Site->GetState(oState);
			if (Syn_Site::Error == oState)
			{
				return;
			}

			for (size_t t = imageFingerPos; t < listOfTestStepName.size(); t++)
			{

				if (t != imageFingerPos)
					_pSyn_Site->GetTestResult(pTestResult);
					
				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
				if (Syn_ExceptionCode::Syn_TestStepConfigError == rc)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "NULL");
					emit send(iSiteNumber);
					return;
				}
				else if (Syn_ExceptionCode::Syn_OK != rc)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Fail");
				}
				else
				{
					if (t == imageFingerPos)
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Pass");
					}
					else
					{
						emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]));
					}
				}

			}

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