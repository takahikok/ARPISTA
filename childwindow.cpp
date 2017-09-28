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

//	QElapsedTimer et;
//	et.start();
	for (unsigned int i = 0; i < thisShot->GetADCNumber(); i++) {
		unsigned int every = thisShot->GetBlockSize(i) / 4000;
		thisShot->LoadDataPoints(i, every);
	}

//	qint64 t = et.elapsed();
//	QMessageBox::about(this, "About this demo", QString::number(t));
	return;


}

void ChildWindow::Plot()
{
//	QElapsedTimer et;
//	et.start();
	int total_trace = 0;
	for (unsigned int i = 0; i < thisShot->GetADCNumber(); i++)
			total_trace += thisShot->GetTraceTotalNumber(i);

	this->childWidget->CreateChildWidget(total_trace);


	for (unsigned int trace_id = 0, adc_id = 0; adc_id < thisShot->GetADCNumber(); adc_id++) {
		for (unsigned int ch_id = 0; ch_id < thisShot->GetTraceTotalNumber(adc_id); trace_id++, ch_id++) {
			std::vector<QwtPlotCurve*>& this_curve = this->childWidget->plotRawWidget[trace_id]->thisCurve;
			this_curve.push_back(new QwtPlotCurve("sin"));
			this_curve[0]->setPen(QPen(Qt::red));
			this_curve[0]->attach(this->childWidget->plotRawWidget[trace_id]);
			this_curve[0]->setSamples(thisShot->GetDataPoints(adc_id)[0].data(),
					thisShot->GetDataPoints(adc_id)[ch_id + 1].data(),
					static_cast<unsigned int>(thisShot->GetDataPoints(adc_id)[ch_id + 1].size()));

		}
	}
//	qint64 t = et.elapsed();
//	QMessageBox::about(this, "About this demo", QString::number(t));
}


