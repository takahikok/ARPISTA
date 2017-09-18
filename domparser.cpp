#include "domparser.h"

DomParser::DomParser(QDomDocument &doc, QTreeWidgetItem *tree)
{
	treeWidget = tree;

	QDomElement root = doc.documentElement();

	if (root.tagName() != "ARPISTASettings")
		return;

	for (QDomNode node = root.firstChild();
	     !node.isNull();
	     node = node.nextSibling()) {
		if (node.toElement().tagName() == "entry")
			parseEntry(node.toElement(), tree);

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

void TKQXMLUtility::LoadXMLFile(const QString& file_path, QDomDocument& doc)
{
	QFile file(file_path);

	file.open(QFile::ReadOnly | QFile::Text);
	doc.setContent(&file, true);
	return;
}

void TKQXMLUtility::SaveXMLFile(const QString& file_path, QDomDocument& doc)
{
	QFile file(file_path);

	if (file.open(QFile::WriteOnly| QFile::Truncate)) {
		const int indent_space_width = 4;
		QTextStream out(&file);
		doc.save(out, indent_space_width);
	}
	return;
}
