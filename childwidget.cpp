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
}

ChildWidget::~ChildWidget()
{
	delete ui;
}

void ChildWidget::CreateChildWidget(int widget_number)
{
//	for (iterator iter : plotRawWidget)
//		delete iter;
	for (int i = 0; i < widget_number; i++) {
		plotRawWidget.push_back(new PlotRawWidget(ui->scrollAreaWidgetContents));
		plotRawWidget[i]->setObjectName(QStringLiteral("widget"));
		//ui->tab_1->addWidget(plotRawWidget[i]);
//		QVBoxLayout *verticalLayout_3;
		//ui->verticalLayout_3 = new QVBoxLayout(ui->scrollAreaWidgetContents);
		//ui->verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
		ui->verticalLayout_3->addWidget(plotRawWidget[i]);
	}

}
