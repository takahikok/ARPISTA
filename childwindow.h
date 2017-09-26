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
#include "tkshotinfo.h"

namespace Ui {
class ChildWindow;
}

class ChildWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	explicit ChildWindow(QWidget *parent = 0);
	~ChildWindow();
	void OpenShot(std::string file_name);
	void Plot();

private:
	ChildWidget *childWidget;
	std::string shot_profile_file_name;
	QDomDocument shot_profile_dom;
	TKSHOT* thisShot;


};

#endif // CHILDWINDOW_H
