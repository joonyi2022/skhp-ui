
/****************************************
fw upgrade thread

******************************************/
#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <QJsonValue>
#include <QByteArray>


#include <QDebug>
#include <QFile>

#include "include/common.h"
#include "include/maincontrol.h"
#include "include/fw_upgrade.h"



FwUpgradeThread::FwUpgradeThread(QObject *in)
    : QThread(in)
{
   qDebug()<<"FwUpgradeThread thread created"<<endl;
   
  
}

FwUpgradeThread::~FwUpgradeThread()
{
   
}
void FwUpgradeThread::replyFinished(QNetworkReply * reply)
{

	if(reply->error() == QNetworkReply::NoError) {
			QByteArray data = reply->readAll();
			QString Sdata = data;
			qDebug(data);
	} else {
			 qDebug()<<"Error at Reply"<<endl;
			 QByteArray data = reply->readAll();
			 qDebug(data);
	}
	reply->deleteLater();

}




void FwUpgradeThread::bind( MainApp* ma)
{
    m_mainApp = ma;
}

void FwUpgradeThread::makeConnect()
{

	//connect(m_restclient, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply *)));
}

void FwUpgradeThread::downLoadFW()
{
#if 0
	QNetworkRequest request;
	
	QString cmdline = QString("rm -rf ") + QString("/opt/news/") + strLine;
	            printf("delCurBook::cmdline = %s\n", cmdline.toLocal8Bit().data());
	            system(cmdline.toLocal8Bit().data());
	QUrl url("http://skh.labcross.com:3000/api/v1/device/ping");
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	//m_restclient->get(request);
	system("wget url");
	http://39dc8752.ngrok.io/update_ui/update_skh_ui.V1.1.6" -o log.txt
	"wget -T 180 http://34a30dfd.ngrok.io/update_ui/update_skh_ui.V1.1.6 -O ui_app_v1.6 -o upgrade_log.txt"
		"md5sum myFile> ui_app_md5sum"
	system(checksum)
     md5sum ./ui_app_v1.6 > md5sumresult
	qDebug()<<url;
	#endif
}

void FwUpgradeThread::run()
{

	//new thread running
	 qDebug()<<"fw upgrade start"<<endl;
	
	
    exec();
}


