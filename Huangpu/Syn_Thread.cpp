
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
				if (0!=t)
					_pSyn_Site->GetTestResult(pTestResult);

				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[t]);
				if (Syn_ExceptionCode::Syn_OK != rc)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Failed");
					emit send(iSiteNumber);
					return;
				}
				else
				{
					if (0==t)
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
				if (Syn_ExceptionCode::Syn_OK != rc)
				{
					emit send(iSiteNumber, QString::fromStdString(listOfTestStepName[t]), "Failed");
					emit send(iSiteNumber);
					return;
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