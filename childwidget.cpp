#include "childwidget.h"
#include "ui_childwidget.h"

ChildWidget::ChildWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChildWidget)
{
	ui->setupUi(this);

	QVBoxLayout *verticalLayout_3;
	verticalLayout_3 = new QVBoxLayout(ui->scrollAreaWidgetContents);

	widget = new PlotRawWidget(ui->scrollAreaWidgetContents);
	widget->setObjectName(QStringLiteral("widget"));
	verticalLayout_3->addWidget(widget);
}

ChildWidget::~ChildWidget()
{
	delete ui;
}
