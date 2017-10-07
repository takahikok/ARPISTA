#ifndef ADCCHANNELCONTROLWIDGET_H
#define ADCCHANNELCONTROLWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "tkadcconst.h"

namespace Ui {
class ADCChannelControlWidget;
}

class ADCChannelControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ADCChannelControlWidget(QWidget *parent = 0);
	~ADCChannelControlWidget();
	bool SetExpansionState(bool visible);
	bool GetExpansionState();
	bool SwitchExpansionState();
	void SetParentWidgetItem(QListWidgetItem *parent);
	QListWidgetItem* GetParentWidgetItem();
	void SetFreezeControls(bool const flag);
	bool GetFreezeControls();
	void SetCurrentV_div(TKADCCONST::VDIV const v_div);
	void SetCurrentCoupling();
	void SetCurrentCutOff();

private slots:
	void switchExpansionStateSlot();

private:
	Ui::ADCChannelControlWidget *ui;
	bool is_visible_expansion = false;
	QListWidgetItem* parent_widget_item = nullptr;
	bool is_freeze_controls = false;
};

#endif // ADCCHANNELCONTROLWIDGET_H
