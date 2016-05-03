#ifndef SYN_MINITHREAD_H
#define SYN_MINITHREAD_H

//local
#include "Syn_Site.h"

//Qt
#include <QThread>
//std
#include <string>

//Q_DECLARE_METATYPE(Syn_SiteInfo)

class Syn_MiniThread : public QThread
{
	Q_OBJECT

public:
	Syn_MiniThread() :_stopped(true){};
	~Syn_MiniThread(){};

	void SetSite(Syn_Site *ipSyn_Site)
	{
		if (NULL != ipSyn_Site)
			_pSyn_Site = ipSyn_Site;
	}

	void SetStopTag(bool stoptag)
	{
		_stopped = stoptag;
	}

signals:

	void sendImageInTime(unsigned int iSiteNumber);

protected:

	void run()
	{
		uint32_t rc(0);
		if (NULL == _pSyn_Site)
			return;

		unsigned int iSiteNumber(0);
		_pSyn_Site->GetSiteNumber(iSiteNumber);

		std::vector<std::string> listOfTestStepName;
		_pSyn_Site->GetTestStepList(listOfTestStepName);
		if (0 == listOfTestStepName.size())
			return;

		unsigned int iWaitStimulusPosition(0);
		bool find(false);
		for (size_t i = 0; i < listOfTestStepName.size(); i++)
		{
			if (std::string("WaitStimulus") == listOfTestStepName[i])
			{
				find = true;
				iWaitStimulusPosition = i;
				break;
			}
		}
		if (!find)
			return;

		Syn_DutTestResult *pResults = NULL;
		while (!_stopped)
		{
			Syn_Site::SiteState oState;
			_pSyn_Site->GetState(oState);
			if (Syn_Site::Error != oState)
			{
				rc = _pSyn_Site->ExecuteTestStep(listOfTestStepName[iWaitStimulusPosition]);
				_pSyn_Site->GetTestResult(pResults);
				emit sendImageInTime(iSiteNumber);
			}
		}
	}

private:

	bool _stopped;

	Syn_Site *_pSyn_Site;
};

#endif // SYN_MINITHREAD_H