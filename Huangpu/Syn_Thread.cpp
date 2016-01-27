
//local
#include "Syn_Thread.h"

#include <QReadWriteLock>

QReadWriteLock Lock(QReadWriteLock::Recursive);

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

	/*unsigned int iSiteNumber(0);
	_pSyn_Site->GetSiteNumber(iSiteNumber);

	_pSyn_Site->Run();

	_pSyn_Site->GetSiteInfo(_SiteInfo);
	emit send((void*)&_SiteInfo);

	_stopped = true;*/

	//fingerprint debug,delete at end
	while (!_stopped)
	{
		_pSyn_Site->GetFingerprintImage();

		_pSyn_Site->GetSiteInfo(_SiteInfo);
		emit send((void*)&_SiteInfo);
	}

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