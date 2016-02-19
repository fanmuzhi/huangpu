#ifndef SYN_THREADDEBUG_H
#define SYN_THREADDEBUG_H

/*
fingerprint debug,delete at end
*/
//local
#include "Syn_Site.h"

//Qt
#include <QThread>
//std
#include <string>

#include "windows.h"

//Q_DECLARE_METATYPE(Syn_SiteInfo)

enum debugType{ calibrateType,getImageType};

class Syn_ThreadDebug : public QThread
{
	Q_OBJECT

public:
	Syn_ThreadDebug()
	{
		_run = false;
		_pSyn_Site = NULL;
	};
	~Syn_ThreadDebug()
	{};

	void SetSite(Syn_Site *ipSyn_Site)
	{
		_pSyn_Site = ipSyn_Site;
	};

	void SetRunTag(bool runTag)
	{
		_run = runTag;
	};

	//debug
	debugType _dbgType;

signals:

	void send(void * TestResultValue);

protected:

	void run()
	{
		if (NULL == _pSyn_Site)
		{
			return;
		}

		if (calibrateType == _dbgType)
		{
			_pSyn_Site->ExecuteScript(3);

			while (true)
			{
				//sleep(200);
				SiteState oState;
				_pSyn_Site->GetState(oState);
				if (TestDataReady == oState)
				{
					_pSyn_Site->GetSiteInfo(_SiteInfo);
					emit send((void*)&_SiteInfo);
					break;
				}
			}
		}
		else
		{
			while (_run)
			{
				_pSyn_Site->GetFingerprintImage();

				_pSyn_Site->GetSiteInfo(_SiteInfo);
				emit send((void*)&_SiteInfo);
			}
		}

		
	};

private:

	bool _run;

	Syn_Site *_pSyn_Site;

	Syn_SiteInfo _SiteInfo;
};

#endif // SYN_THREADDEBUG_H