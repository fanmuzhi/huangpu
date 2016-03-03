#ifndef SYN_THREAD_H
#define SYN_THREAD_H

//local
#include "Syn_Site.h"

//Qt
#include <QThread>
//std
#include <string>

//Q_DECLARE_METATYPE(Syn_SiteInfo)

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

	void send(unsigned int iSiteNumber);

protected:

	void run();

private:
	
	bool _stopped;

	Syn_Site *_pSyn_Site;

	int _iFlag;
};

#endif // SYN_THREAD_H