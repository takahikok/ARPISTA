#ifndef CHILDWIDGET_H
#define CHILDWIDGET_H

#include <QWidget>
#include "plotrawwidget.h"
#include <vector>

namespace Ui {
class ChildWidget;
}

class ChildWindow;

class ChildWidget : public QWidget
{
	Q_OBJECT
	friend class ChildWindow;

public:
	explicit ChildWidget(QWidget *parent = 0);
	~ChildWidget();
	void CreateChildWidget(int widget_number);

private:
	Ui::ChildWidget *ui;
	std::vector<PlotRawWidget*> plotRawWidget;
};

#endif // CHILDWIDGET_H
