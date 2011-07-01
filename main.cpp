#include "wc3mousecapture.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WC3MouseCapture w;
	return a.exec();
}
