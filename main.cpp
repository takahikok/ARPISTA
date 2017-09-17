#include "mainwindow.h"
#include <QApplication>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "domparser.h"

void Init(auto* w);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;

	QFile file("C:\\Users\\user\\Documents\\ARPISTA\\settings.xml");
	file.open(QFile::ReadOnly | QFile::Text);
	QTreeWidgetItem settings;// = new QTreeWidgetItem();
	DomParser(&file, &settings);

	::Init(&settings);

	w.Init(&settings);
	w.show();

	return a.exec();
}

void Init(auto* w)
{
//	QString filePath = QFileDialog::getOpenFileName(this,
//			    tr("Open XML"), "/", tr("XML Files (*.xml)"));


	return;
}
