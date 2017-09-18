#include "childwidget.h"
#include "ui_childwidget.h"

ChildWidget::ChildWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChildWidget)
{
	ui->setupUi(this);
}

ChildWidget::~ChildWidget()
{
	delete ui;
}
