#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::Init(QTreeWidgetItem* settings)
{
	//listWidget_Log
	{
		ui->listWidget_Log->addItem("<font color=\"red\">Init</font>");
	}

	//treeWidget_Instruments
	{
		QStringList list;//QTreeWidgetのヘッダ名が入る

		list << "Device" << "Enabled";

		ui->treeWidget_Instruments->setColumnCount(2);//列の数をセット
		ui->treeWidget_Instruments->setHeaderLabels(list);//先ほどのlistをヘッダ名にセット

		QTreeWidgetItem *item = new QTreeWidgetItem;//セットするアイテム
		QTreeWidgetItem *child = new QTreeWidgetItem;//itemの子となるアイテム

		item->setText(0,"item");//0列目に"item"というテキストセット
		item->setText(1,"1");//1列目に"1"というテキストセット
		child->setText(0, "child item");//0列目に"child item"というテキストセット
		item->addChild(child);//itemの子にchildをセット

		ui->treeWidget_Instruments->insertTopLevelItem(0, item);//QTreeView０行目にitemをセット
	}

	//treeWidget_Property
	{
		QStringList list;//QTreeWidgetのヘッダ名が入る

		list << "Key" << "Value";

		ui->treeWidget_Property->setColumnCount(2);//列の数をセット
		ui->treeWidget_Property->setHeaderLabels(list);//先ほどのlistをヘッダ名にセット

//		QTreeWidgetItem *item = new QTreeWidgetItem;//セットするアイテム
		QTreeWidgetItem *item = settings->clone();
//		QTreeWidgetItem *child = new QTreeWidgetItem;//itemの子となるアイテム

//		item->setText(0,"item");//0列目に"item"というテキストセット
//		item->setText(1,"1");//1列目に"1"というテキストセット
//		child->setText(0, "child item");//0列目に"child item"というテキストセット
//		item->addChild(child);//itemの子にchildをセット

		ui->treeWidget_Property->insertTopLevelItem(0, item);//QTreeView０行目にitemをセット
	}
	return;
}

ChildWindow* MainWindow::createChild()
{
	ChildWindow *childWindow = new ChildWindow(ui->mdiArea);
	childWindow->setAttribute(Qt::WA_DeleteOnClose);
	childWindow->setWindowState(Qt::WindowMaximized);
	childWindow->show();
	return childWindow;
}

ChildWindow* MainWindow::createChildandOpenLocal()
{
	QString selFilter = "shot information(*.txt)";
	QString caption = "open local file";
	QString dir = "C:/Qt/Qt5.3.2";
	QString filter = "all(*.*);;shot information(*.xml);;ソース(*.h *.cpp)";
	QString fileName = QFileDialog::getOpenFileName(
				this, caption, dir, filter, &selFilter,
				QFileDialog::DontUseCustomDirectoryIcons
				);

	if (fileName.isEmpty())
		return nullptr;

	ChildWindow *childWindow = MainWindow::createChild();
	return childWindow;
}

void MainWindow::maximizeSubWindows()
{
	if (ui->mdiArea->subWindowList().isEmpty())
	    return;

	for (QMdiSubWindow *childWindow : ui->mdiArea->subWindowList()) {
		childWindow->setWindowState(Qt::WindowMaximized);
	}
}

void MainWindow::tileSubWindowsVertically()
{
	if (ui->mdiArea->subWindowList().isEmpty())
	    return;

	QPoint position(0, 0);
	for (QMdiSubWindow *childWindow : ui->mdiArea->subWindowList()) {
	    QRect rect(0, 0, ui->mdiArea->width(),
		       ui->mdiArea->height() / ui->mdiArea->subWindowList().count());
	    childWindow->setGeometry(rect);
	    childWindow->move(position);
	    position.setY(position.y() + childWindow->height());
	}
}

void MainWindow::tileSubWindowsHorizontally()
{
	if (ui->mdiArea->subWindowList().isEmpty())
	    return;
	QPoint position(0, 0);
	for (QMdiSubWindow *childWindow : ui->mdiArea->subWindowList()) {
	    QRect rect(0, 0, ui->mdiArea->width() / ui->mdiArea->subWindowList().count(),
		       ui->mdiArea->height());
	    childWindow->setGeometry(rect);
	    childWindow->move(position);
	    position.setX(position.x() + childWindow->width());
	}
}

void MainWindow::test()
{

}
