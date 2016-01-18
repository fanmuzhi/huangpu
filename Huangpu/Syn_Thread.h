#ifndef SYN_THREAD_H
#define SYN_THREAD_H

//local
#include "Syn_Site.h"

//Qt
#include <QThread>
//std
#include <string>

//Q_DECLARE_METATYPE(Syn_OTPTestInfo)

class Syn_Thread : public QThread
{
	Q_OBJECT

public:
	Syn_Thread();
	~Syn_Thread();

	void SetSite(Syn_Site *ipSyn_Site);

	void SetStopTag(bool stoptag);

signals:

	void send(void * TestResultValue);

protected:

	void run();

private:
	
	bool _stopped;

	Syn_Site *_pSyn_Site;

	Syn_SiteInfo		_SiteInfo;
	Syn_DutTestInfo		_TestInfo;
	Syn_DutTestResult	_TestResult;
};

#endif // SYN_THREAD_H