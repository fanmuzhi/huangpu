
//local
#include "Syn_Thread.h"

#include <QReadWriteLock>

//QReadWriteLock Lock;

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

	//Lock.lockForRead();
	//Lock.tryLockForWrite();

	string strTime("");
	//while (!_stopped)
	//{
	//	/*_pSyn_Site->TestSet();

	//	_pSyn_Site->TestGetValue(strTime);

	//	QString qstrTime(QString::fromStdString(strTime));
	//	//emit send(qstrTime);
	//	
	//	_SynSt.qValue = qstrTime;

	//	emit send(&_SynSt);*/

	//	/*_pSyn_Site->Run();

	//	_pSyn_Site->GetOTPTestInfo(_TestInfo);
	//	emit send(&_TestInfo);*/
	//}

	_pSyn_Site->Run();

	_pSyn_Site->GetOTPTestInfo(_TestInfo);
	emit send(&_TestInfo);

	_stopped = true;

	//Lock.unlock();

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