#ifndef DOMPARSER_H
#define DOMPARSER_H
#include <qxml.h>
#include <qdom.h>
#include <QTreeWidget>
#include <QMessageBox>
class DomParser
{
public:
	DomParser(QIODevice *device, QTreeWidgetItem *tree);
private:
	void parseEntry(const QDomElement &element,
			QTreeWidgetItem *parent);

	QTreeWidgetItem *treeWidget;
};
#endif // DOMPARSER_H
