#include "mainwindow.h"
#include <QApplication>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "domparser.h"

#include <qxml.h>
#include <qdom.h>
#include <QTreeWidget>
#include <QMessageBox>

void Init(auto& w);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;

	QDomDocument doc;
	TKQXMLUtility::LoadXMLFile("C:\\Users\\user\\Documents\\ARPISTA\\settings.xml",
			      doc);
	QTreeWidgetItem settings;
	DomParser(doc, &settings);

//	::Init(doc);

	w.Init(&settings);
	w.show();

	TKQXMLUtility::SaveXMLFile("C:\\Users\\user\\Documents\\ARPISTA\\settings_gitignore.xml",
			      doc);
	return a.exec();
}

void Init(auto& doc)
{

	return;
}
