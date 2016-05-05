#ifndef SYN_THREAD_H
#define SYN_THREAD_H

//local
#include "Syn_Site.h"

//Qt
#include <QThread>
//std
#include <string>

class Syn_Thread : public QThread
{
	Q_OBJECT

public:
	Syn_Thread();
	~Syn_Thread();

	void SetSite(Syn_Site *ipSyn_Site);

	void SetStopTag(bool stoptag);

	void SetFlag(int iFlag){ _iFlag = iFlag; };

signals:

	void send(unsigned int iSiteNumber, const Syn_DutTestResult *pDutResult);

	void send(unsigned int iSiteNumber, const QString strTestStepName, const QString strPassResults);

protected:

	void run();

private:
	
	bool _stopped;

	Syn_Site *_pSyn_Site;

	int _iFlag;
};

#endif // SYN_THREAD_H