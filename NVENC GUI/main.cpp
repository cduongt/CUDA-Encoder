#include "nvencgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NVENCGUI w;
	w.show();
	return a.exec();
}