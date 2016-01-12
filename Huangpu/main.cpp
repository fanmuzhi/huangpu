#include "fps_testexecutive.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FPS_TestExecutive w;
	w.show();


	//el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	//el::Configurations conf("log.conf");
	//el::Loggers::reconfigureAllLoggers(conf);
	////el::Helpers::installPreRollOutCallback(rolloutHandler);

	//for (int i = 0; i < 100000; ++i)
	//{
	//	LOG(INFO) << "Test==========================================================";
	//}

	//el::Helpers::uninstallPreRollOutCallback();

	return a.exec();

}
