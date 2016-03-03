
//local
#include "Syn_Thread.h"

Syn_Thread::Syn_Thread()
: QThread()
, _stopped(true)
, _pSyn_Site(NULL)
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
	switch (_iFlag)
	{
		case 1:
			rc = _pSyn_Site->Open();
			if (rc == 0)
			{
				rc = _pSyn_Site->ExecuteTestStep("Calibrate");
				Syn_DutTestResult * TestResult = NULL;
				rc = _pSyn_Site->GetTestResult(TestResult);
				rc = _pSyn_Site->ExecuteTestStep("AcqImgNoFinger");
				if (rc == 0)
				{
					emit send(iSiteNumber);
				}
			}

		case 2:
			/*rc = _pSyn_Site->Open();
			if (rc == 0)
			{
				rc = _pSyn_Site->ExecuteTestStep("AcqImgFinger");
				if (rc == 0)
				{
					emit send(iSiteNumber);
				}
			}*/

			rc = _pSyn_Site->ExecuteTestStep("AcqImgFinger");
			//Syn_DutTestResult * TestResult = NULL;
			//rc = _pSyn_Site->GetTestResult(TestResult);
			//rc = _pSyn_Site->ExecuteTestStep("AcqImgNoFinger");
			if (rc == 0)
			{
				emit send(iSiteNumber);
			}

		default:
			if (rc == 0)
			{
				rc = _pSyn_Site->ExecuteTestStep("Calibrate");
			}
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