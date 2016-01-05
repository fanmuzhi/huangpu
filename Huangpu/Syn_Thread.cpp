
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

	string strTime("");
	while (!_stopped)
	{
		_pSyn_Site->TestSet();

		_pSyn_Site->TestGetValue(strTime);

		QString qstrTime(QString::fromStdString(strTime));
		//emit send(qstrTime);
		
		_SynSt.qValue = qstrTime;

		emit send(&_SynSt);
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