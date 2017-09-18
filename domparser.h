#ifndef DOMPARSER_H
#define DOMPARSER_H
#include <qxml.h>
#include <qdom.h>
#include <QTreeWidget>
#include <QMessageBox>
class DomParser
{
public:
	DomParser(QDomDocument &doc, QTreeWidgetItem *tree);
private:
	void parseEntry(const QDomElement &element,
			QTreeWidgetItem *parent);

	QTreeWidgetItem *treeWidget;
};
class TKQXMLUtility
{
public:
	static void LoadXMLFile(const QString& file_path, QDomDocument &doc);
	static void SaveXMLFile(const QString& file_path, QDomDocument &doc);
};
#endif // DOMPARSER_H
