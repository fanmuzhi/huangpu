
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

	std::vector<std::string> listOfTestStepName;
	_pSyn_Site->GetTestStepList(listOfTestStepName);
	if (0 == listOfTestStepName.size())
		return;

	unsigned int iWaitStimulusPosition(0);
	for (size_t i = 0; i < listOfTestStepName.size(); i++)
	{
		if (std::string("WaitStimulus") == listOfTestStepName[i])
		{
			iWaitStimulusPosition = i;
			break;
		}
	}

	Syn_DutTestResult *pTestResult = NULL;
	if (1 == _iFlag)
	{
		rc = _pSyn_Site->Open();
		if (rc == 0)
		{
			for (size_t t = 0; t < iWaitStimulusPosition; t++)
			{
				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
				_pSyn_Site->GetTestResult(pTestResult);
				if (Syn_ExceptionCode::Syn_OK != rc || NULL == pTestResult)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Failed");
					emit send(iSiteNumber, pTestResult);
					return;
				}
				else
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]));
				}
			}
			emit send(iSiteNumber, pTestResult);
		}
	}
	else if (2 == _iFlag)
	{
		if (rc == 0)
		{
			Syn_Site::SiteState oState;
			_pSyn_Site->GetState(oState);
			if (Syn_Site::Error == oState)
			{
				return;
			}

			for (size_t t = iWaitStimulusPosition+1; t < listOfTestStepName.size(); t++)
			{
				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
				_pSyn_Site->GetTestResult(pTestResult);
				if (Syn_ExceptionCode::Syn_OK != rc || NULL == pTestResult)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Failed");
					emit send(iSiteNumber, pTestResult);
					return;
				}
				else
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]));
				}
			}
			emit send(iSiteNumber, pTestResult);
		}
	}
	else
	{
		rc = _pSyn_Site->Open();
		if (rc == 0)
		{
			for (size_t t = 0; t < listOfTestStepName.size(); t++)
			{
				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
				_pSyn_Site->GetTestResult(pTestResult);
				if (Syn_ExceptionCode::Syn_OK != rc || NULL == pTestResult)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Failed");
					emit send(iSiteNumber, pTestResult);
					return;
				}
				else
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), QString::fromStdString(pTestResult->_mapTestPassInfo[listOfTestStepName[t]]));
				}
			}
			emit send(iSiteNumber, pTestResult);
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