#include "fps_testexecutive.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FPS_TestExecutive w;
	w.show();
	return a.exec();
}
