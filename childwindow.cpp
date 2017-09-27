#include "childwindow.h"

#include <QMdiSubWindow>

#include "childwidget.h"
#include <QFileInfo>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>

ChildWindow::ChildWindow(QWidget *parent) :
  QMdiSubWindow(parent)
{
    childWidget = new ChildWidget(this);
    this->setWidget(childWidget);
    thisShot = new TKSHOT();
}

ChildWindow::~ChildWindow()
{
	childWidget->~ChildWidget();
}

void ChildWindow::OpenShot(std::string file_path)
{
	shot_profile_file_name = file_path;

	TKQXMLUtility::LoadXMLFile(shot_profile_file_name.c_str(),
				   shot_profile_dom);

	if (shot_profile_dom.documentElement().childNodes().isEmpty())
		return;

	QFileInfo finfo(shot_profile_file_name.c_str());


	// 1st
	for (QDomNode shotNode = shot_profile_dom.documentElement().childNodes().at(0);
	     !shotNode.isNull();
	     shotNode = shotNode.nextSibling()) {
		if (shotNode.toElement().tagName() == "ShotInformation") {
			for (QDomNode shotInformationNode = shotNode.childNodes().at(0);
			     !shotInformationNode.isNull();
			     shotInformationNode = shotInformationNode.nextSibling()) {
				if (shotInformationNode.toElement().tagName() == "ShotDate") {
//					QMessageBox::about(this, "ShotDate", shotInformationNode.firstChild().nodeValue());
				} else if  (shotInformationNode.toElement().tagName() == "RawData") {
					for (QDomNode rawDataNode = shotInformationNode.childNodes().at(0);
					     !rawDataNode.isNull();
					     rawDataNode = rawDataNode.nextSibling()) {
						if (rawDataNode.toElement().tagName() == "FileName")
//							QMessageBox::about(this, "FileName", rawDataNode.firstChild().nodeValue());
						thisShot->AppendDataFile((finfo.absoluteDir().absolutePath().toStdString() + "/" + rawDataNode.firstChild().nodeValue().toStdString()).c_str());
//						QMessageBox::about(this, "FileName", (finfo.absoluteDir().absolutePath().toStdString() + "/" + rawDataNode.firstChild().nodeValue().toStdString()).c_str());
						//QMessageBox::about(this, "FileName", std::to_string(QFileInfo((finfo.absoluteDir().absolutePath().toStdString() + "/" + rawDataNode.firstChild().nodeValue().toStdString() + ".WVF").c_str()).size()).c_str());
					}
				}
			}

		}
//		QMessageBox::about(this, "About this demo", shotNode.toElement().tagName());
	}

	for (int i = 0; i < thisShot->GetADCNumber(); i++)
		thisShot->LoadDataPoints(i);
	return;


}

void ChildWindow::Plot()
{
	int total_trace = 0;
	for (int i = 0; i < thisShot->GetADCNumber(); i++)
			total_trace += thisShot->GetTraceTotalNumber(i);

	this->childWidget->CreateChildWidget(total_trace);

	for (int i = 0, adc_ch = 0, adc_id = 0; i < total_trace; i++, adc_ch++) {
		if (adc_ch == thisShot->GetTraceTotalNumber(adc_id)) {
			adc_id++;
			adc_ch = 0;
		}

//		int i = 0, adc_ch = 0, adc_id = 0;
//		PlotRawWidget::DATA data_;
//		data_.x = &(thisShot->GetDataPoints(adc_id)[0]);
//		data_.y = &(thisShot->GetDataPoints(adc_id)[adc_ch + 1]);
//		data_.x = (thisShot->GetDataPointsPtr(adc_id)->at(0));
//		data_.y = (thisShot->GetDataPointsPtr(adc_id)[adc_ch + 1]);
//		this->childWidget->plotRawWidget[i]->data.clear();
//		this->childWidget->plotRawWidget[i]->data.push_back(data_);

		this->childWidget->plotRawWidget[i]->thisCurve.push_back(new QwtPlotCurve("sin"));
		this->childWidget->plotRawWidget[i]->thisCurve[0]->setPen(QPen(Qt::red));
		this->childWidget->plotRawWidget[i]->thisCurve[0]->attach(this->childWidget->plotRawWidget[i]);

//		thisCurve[0]->setSamples(data[0].x->data(), data[0].y->data(), static_cast<int>(data[0].y->size()));
		this->childWidget->plotRawWidget[i]->thisCurve[0]->setSamples(thisShot->GetDataPoints(adc_id)[0].data(), thisShot->GetDataPoints(adc_id)[adc_ch + 1].data(), static_cast<unsigned int>(thisShot->GetDataPoints(adc_id)[adc_ch + 1].size()));
	}
}


