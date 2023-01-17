
/********************************************************
Let consider a channel with three fields representing temperature,
humidity and pressure. 
Let us assume that the various API keys and channel numbers 
are initialised as follows:

QString RDKey = "XXXXXXXXXXXXX"
QString WRKey = "XXXXXXXXXXXXXXX"
QString CHKey = "XXXXXXXXXXXXXXX"
Qstring CHNum = "xxxxx"

To send a feed, we need to perform the POST method
with the steps that follow, using the 
URL https://api.thingspeak.com/update.json and
the following JSON data as request payload:

{
"api_key":"XXXXXXX XXXXXXXX",
"field1":"15",
"field2":"22",
"field3":"900"
}


*********************************************************/
#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include <QUrlQuery>
#include <QUrl>
#include <QDebug>
#include <QVariant>
#include "include/common.h"
#include "include/maincontrol.h"
#include "include/com_https.h"



ComHttpsThread::ComHttpsThread(QObject *in)
    : QThread(in)
{

	QString WRKey = "XXXXXXXXXXXXXXX";
	QString CHKey = "XXXXXXXXXXXXXXX";
	QString CHNum = "xxxxx";

}

ComHttpsThread::~ComHttpsThread()
{
   
}


void ComHttpsThread::makeConnect()
{
	//connect(m_restclient, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply *)));
	
}

void ComHttpsThread::replyFinished(QNetworkReply * reply)
{
	QJsonDocument jsdoc;
	QString strCmd;
//	QString test = "replayfinished";


	if(reply->error() == QNetworkReply::NoError) {

			jsdoc = QJsonDocument::fromJson(reply->readAll());
			//QByteArray data = reply->readAll();
			//qDebug(data);


			QJsonObject jsobj = jsdoc.object();

			//get command
			if (jsobj.contains("command")) {  // 包含指定的 key
	            QJsonValue value = jsobj.value("command");  // 获取指定 key 对应的 value
	            if (value.isString()) {  // 判断 value 是否为字符串
	                strCmd = value.toString();  // 将 value 转化为字符串
	                qDebug() << "command : " << strCmd;
	            }
	        }
			//qDebug() << "status:"<< jsobj["status"].toInt();

            if(QString::compare(strCmd,"update_firmware") == 0) {
				//update firmware command received, decode url first.
				
                if (jsobj.contains("content")) {
            		QJsonValue contentdata = jsobj.value("content");
           		 	if (contentdata.isObject()) {  // data is object
                		QJsonObject dataobj = contentdata.toObject();
               			 if (dataobj.contains("version")) {
                    		  QJsonValue versionVal = dataobj.value("version");
                  			  if (versionVal.isString()) {
                       			 QString strVer = versionVal.toString();
                       			 qDebug() << "version: " << strVer;
                    		  }
                		 }
               		 	 if (dataobj.contains("url")) {
                    		QJsonValue urlVal = dataobj.value("url");
                   			 if (urlVal.isString()) {
                        		QString strDownload = urlVal.toString();
                           		qDebug() << "url: " << strDownload;
                         	}
                		}
						 if (dataobj.contains("checksum")) {
                    		QJsonValue ChecksumVal = dataobj.value("checksum");
                   			 if (ChecksumVal.isString()) {
                        		QString strChecksum = ChecksumVal.toString();
                           		qDebug() << "checksum: " << strChecksum;
                         	}
                		}
           		 	}
			 }
            }
            
		} else {
			 qDebug()<<"Error at Reply"<<endl;
			 QByteArray data = reply->readAll();
			 qDebug(data);
		}

		reply->deleteLater();

}



void ComHttpsThread::response_q_num( QString num)
{
	QNetworkRequest request;
	QUrlQuery querystr;
	m_restclient->moveToThread(this);
    connect(m_restclient, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	querystr.addQueryItem("message_id",num );
	
	QString hname =	m_mainApp->MC_Get_HTTP_Host_Name();
	qDebug() <<"run:"<<hname<<endl;
	QString pingStrUrl = "http://"+hname+"/api/v1/devices/receive_message";
	QUrl url(pingStrUrl);
	
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->post(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"response_q_num pingStrUrl="<<pingStrUrl<<endl;
		

}


void ComHttpsThread::regesiterDevice()
{
    

	QNetworkRequest request;
	QUrlQuery querystr;
	QString m_mac_address;
    QString m_ip_address;

    m_mac_address = m_mainApp->MC_Get_Device_MAC();
    m_ip_address = m_mainApp->MC_Get_Device_Ip_Addr();
	m_restclient = new QNetworkAccessManager();
	m_restclient->moveToThread(this);
	connect(m_restclient, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	querystr.addQueryItem("uuid",m_mac_address );
	qDebug()<<"uuid="<<m_mac_address<<endl;
	querystr.addQueryItem("ip",m_ip_address);
	qDebug()<<"ip="<<m_ip_address<<endl;
	querystr.addQueryItem("version",app_version);
	QString hname =	m_mainApp->MC_Get_HTTP_Host_Name();
	qDebug() <<"run:"<<hname<<endl;
	QString pingStrUrl = "http://"+hname+"/api/v1/devices/ping";
	QUrl url(pingStrUrl);
	
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->post(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"pingStrUrl="<<pingStrUrl<<endl;
		

}

void ComHttpsThread::bind( MainApp* ma)
{
    m_mainApp = ma;
}

void ComHttpsThread::run()
{

    /*regesiter device*/
	#ifdef CS_TESTING
	/*do nothing*/
	#else
	regesiterDevice();
	#endif

	//new thread running
    exec();
}

void ComHttpsThread::stopRunning()
{
    m_isRunning = 0;
}


