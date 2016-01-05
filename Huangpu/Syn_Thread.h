#ifndef SYN_THREAD_H
#define SYN_THREAD_H

//local
#include "Syn_Site.h"

//Qt
#include <QThread>

//std
#include <string>

struct Syn_St
{
	QString qValue;
};
Q_DECLARE_METATYPE(Syn_St)

class Syn_Thread : public QThread
{
	Q_OBJECT

public:
	Syn_Thread();
	~Syn_Thread();

	void SetSite(Syn_Site *ipSyn_Site);

	void SetStopTag(bool stoptag);

signals:

	//void send(QString strValue);
	//void send(Syn_St Value);
	void send(void * Value);

protected:

	void run();

private:
	
	bool _stopped;

	Syn_Site *_pSyn_Site;

	Syn_St _SynSt;

};

#endif // SYN_THREAD_H
