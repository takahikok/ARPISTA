#ifndef PLOTRAWWIDGET_H
#define PLOTRAWWIDGET_H
#include <qwt_plot.h>

class ChildWindow;
class QwtPlotCurve;

class PlotRawWidget : public QwtPlot
{
	Q_OBJECT
	friend class ChildWindow;
public:
	struct DATA
	{
	public:
		std::vector<double>* x;
		std::vector<double>* y;
	};

public:
	explicit PlotRawWidget(QWidget *parent = 0);
	void PlotCurve();

private:
	std::vector<QwtPlotCurve*> thisCurve;
	std::vector<DATA> data;

signals:

public slots:

};
#endif // PLOTRAWWIDGET_H
