#include "domparser.h"

DomParser::DomParser(QIODevice *device, QTreeWidgetItem *tree)
{
	treeWidget = tree;
	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument doc;

	if (!doc.setContent(device, true, &errorStr, &errorLine,
			&errorColumn)) {
		QMessageBox::warning(0, QObject::tr("DOM Parser"),
					 QObject::tr("Parse error at line %1, "
						 "column %2:\n%3")
					 .arg(errorLine)
					 .arg(errorColumn)
					 .arg(errorStr));
		return;
	}

	QDomElement root = doc.documentElement();

	if (root.tagName() != "ARPISTASettings")
		return;

	for (QDomNode node = root.firstChild();
	     !node.isNull();
	     node = node.nextSibling()) {
		if (node.toElement().tagName() == "entry")
			parseEntry(node.toElement(), 0);

	}
}

void DomParser::parseEntry(const QDomElement &element,
			   QTreeWidgetItem *parent)
{
	QTreeWidgetItem *item;

	if (parent)
		item = new QTreeWidgetItem(parent);
	else
		item = new QTreeWidgetItem(treeWidget);

	item->setText(0, element.attribute("term"));
	for (QDomNode node = element.firstChild();
	     !node.isNull();
	     node = node.nextSibling()) {
		if (node.toElement().tagName() == "entry") {
			parseEntry(node.toElement(), item);
		} else if (node.toElement().tagName() == "page") {
			for (QDomNode childNode = node.firstChild();
			     !childNode.isNull();
			     childNode = childNode.nextSibling()) {
				if (childNode.nodeType() == QDomNode::TextNode) {
					QString page = childNode.toText().data();
					QString allPages = item->text(1);

					if (!allPages.isEmpty())
						allPages += ", ";

					allPages += page;
					item->setText(1, allPages);
					break;
				}
			}
		}
	}
}
