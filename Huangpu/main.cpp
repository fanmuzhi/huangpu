#include "fps_testexecutive.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon("images/Synaptics.ico"));
	
	FPS_TestExecutive w;
	w.show();

	return a.exec();
}
