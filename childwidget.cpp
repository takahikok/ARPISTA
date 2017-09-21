#include "childwidget.h"
#include "ui_childwidget.h"

ChildWidget::ChildWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChildWidget)
{
	// init child widget
	ui->setupUi(this);

	// tab_1

	// tab_1 vertical layout
	QVBoxLayout *verticalLayout;
	verticalLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

	// raw plots
	for (int i = 0; i < 3; i++) {
		plotRawWidget.push_back(new PlotRawWidget(ui->scrollAreaWidgetContents));
		plotRawWidget[i]->setObjectName(QStringLiteral("widget"));
		verticalLayout->addWidget(plotRawWidget[i]);
	}
}

ChildWidget::~ChildWidget()
{
	delete ui;
}
