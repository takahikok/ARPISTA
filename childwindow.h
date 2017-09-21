#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>

#include "childwidget.h"
#include <string>
#include <qxml.h>
#include <qdom.h>
#include "domparser.h"

namespace Ui {
class ChildWindow;
}

class ChildWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	explicit ChildWindow(QWidget *parent = 0);
	~ChildWindow();

private:
	ChildWidget *childWidget;
	std::string shot_profile_file_name;
	QDomDocument shot_profile_dom;
};

#endif // CHILDWINDOW_H
