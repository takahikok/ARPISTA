#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "domparser.h"
#include "childwindow.h"
#include "adcchannelcontrolwidget.h"
#include "tkshotinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void Init(QTreeWidgetItem* settings);

private slots:
	ChildWindow* createChild();
	ChildWindow* createChildandOpenLocal();
	void maximizeSubWindows();
	void tileSubWindowsVertically();
	void tileSubWindowsHorizontally();
	void test();

private:
	Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
