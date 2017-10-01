#include "adcchannelcontrolwidget.h"
#include "ui_adcchannelcontrolwidget.h"
#include <QMessageBox>

ADCChannelControlWidget::ADCChannelControlWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ADCChannelControlWidget)
{
	ui->setupUi(this);
	SetExpansionState(false);
}

ADCChannelControlWidget::~ADCChannelControlWidget()
{
	delete ui;
}

bool ADCChannelControlWidget::SetExpansionState(bool visible)
{
	is_visible_expansion = visible;
	ui->comboBox_2->setVisible(visible);
	ui->comboBox_3->setVisible(visible);
	switch (visible) {
	case true:
		ui->toolButton->setArrowType(Qt::UpArrow);
		break;
	case false:
		ui->toolButton->setArrowType(Qt::DownArrow);
		break;
	}
//	this->updateGeometry();
	this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	this->resize(this->width(), ui->horizontalLayout->sizeHint().height()+ui->horizontalLayout_3->sizeHint().height());
//	this->update();
//	QMessageBox::about(this, "About this demo", QString::number(this->size().width())+","+QString::number(this->size().height()));
//	QMessageBox::about(this, "About this demo", QString::number(this->sizeHint().width())+","+QString::number(this->sizeHint().height()));
	if (parent_widget_item != nullptr) {
//		parent_widget_item->setSizeHint(QSize(this->width(), this->height()));
		parent_widget_item->setSizeHint(QSize(parent_widget_item->sizeHint().width(), this->sizeHint().height()));
		parentWidget()->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
		parentWidget()->update();
	}
	return visible;
}

bool ADCChannelControlWidget::GetExpansionState()
{
	return is_visible_expansion;
}

bool ADCChannelControlWidget::SwitchExpansionState()
{
	return SetExpansionState(!GetExpansionState());
}

void ADCChannelControlWidget::SetParentWidgetItem(QListWidgetItem* parent)
{
	parent_widget_item = parent;
}

QListWidgetItem* ADCChannelControlWidget::GetParentWidgetItem()
{
	return parent_widget_item;
}

void ADCChannelControlWidget::switchExpansionStateSlot()
{
	this->SwitchExpansionState();
}
