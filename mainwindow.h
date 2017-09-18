#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "domparser.h"
#include "childwindow.h"

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
	void createChild();
private:
	Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
