
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

	string strTime("");

	unsigned int iSiteNumber(0);
	_pSyn_Site->GetSiteNumber(iSiteNumber);

	//Lock.lockForWrite();

	_pSyn_Site->Run();
	//_pSyn_Site->ReadOTP();

	//Lock.unlock();


	_pSyn_Site->GetSiteInfo(_SiteInfo);
	emit send((void*)&_SiteInfo);

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