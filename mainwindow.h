#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "domparser.h"

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
	void on_action_New_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
