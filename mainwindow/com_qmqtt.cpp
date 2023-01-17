/*
 * example.cpp - qmqtt example
 *
 * Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of mqttc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <qmqtt.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include <QUrlQuery>
#include <QUrl>
#include <QDebug>
#include <QVariant>

#include "include/maincontrol.h"
#include "include/fw_upgrade.h"
#include "include/com_qmqtt.h"


const QHostAddress EXAMPLE_HOST = QHostAddress("13.229.83.140");
//const QString hostname = QString("skh.labcross.com");
//const QString hostname = QString("example.org");
const QString hostname = QString("192.168.0.117");

//const QHostAddress EXAMPLE_HOST = QHostAddress("test.mosquitto.org");
const quint16 EXAMPLE_PORT = 1883;
//const quint16 EXAMPLE_PORT =  21759;

 

const QString EXAMPLE_TOPIC = "/device/123456";

Publisher:: Publisher(const QHostAddress& host = EXAMPLE_HOST,
                   const quint16 port = EXAMPLE_PORT,
                   QObject* parent = NULL)
    : QMQTT::Client(host, port, parent)
    , _number(0)
{
    connect(this, &Publisher::connected, this, &Publisher::onConnected); 
	connect(&_timer, &QTimer::timeout, this, &Publisher::onTimeout);
	connect(this, &Publisher::disconnected, this, &Publisher::onDisconnected);
}
Publisher::~Publisher() {}

void Publisher::onConnected()
{

}

void Publisher::onTimeout()
{
    QMQTT::Message message(_number, EXAMPLE_TOPIC,
                           QString("Number is %1").arg(_number).toUtf8());
    publish(message);
    _number++;
    if(_number >= 10)
    {
        _timer.stop();
        disconnectFromHost();
    }
}

void Publisher::onDisconnected()
{
    QTimer::singleShot(0, qApp, &QCoreApplication::quit);
}
#if 0
Subscriber::Subscriber(const QHostAddress& host = EXAMPLE_HOST,
                    const quint16 port = EXAMPLE_PORT,
                    QObject* parent = NULL)
    : QMQTT::Client(host, port, parent)
    , _qout(stdout)
{
    //setHostname(hostname);
    connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
    connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
    connect(this, &Subscriber::received, this, &Subscriber::onReceived);
	
	
}
#endif

Subscriber::Subscriber(const QString& host ,
                    const quint16 port ,
                    const bool ssl ,
                    const bool ignoreSelfSigned ,
                    QObject* parent)
    : QMQTT::Client(host, port,ssl, true,NULL)
    , _qout(stdout)
{
    //setHostname(hostname);
    connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
    connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
    connect(this, &Subscriber::received, this, &Subscriber::onReceived);
	connect(this, &Subscriber::disconnected, this, &Subscriber::onDisconnected);
	
	m_connection_status = false;
}

Subscriber::~Subscriber() {}

void Subscriber::onDisconnected()
{
 
    qDebug() << "disconnected to server" << endl;
	m_connection_status = false;

}


void Subscriber::onConnected()
{
    QString m_mac_address ;
    qDebug() << "connected to server" << endl;
   
	m_mac_address = m_mainApp->MC_Get_Device_MAC();
	#if 1
	QString toppic = "messages/to_device/"+m_mac_address;
	#else
	QString toppic = "messages/#";
	#endif
	toppic = toppic.simplified();
	qDebug() <<"run: toppic= "<<toppic<<endl;
    subscribe(toppic, 0);
	m_connection_status = true;

}

void Subscriber::bind( MainApp* ma)
{
    m_mainApp = ma;
}
bool Subscriber::conStatus()
{
    return m_connection_status;
}


void Subscriber::onSubscribed(const QString& topic )
{
    qDebug() << "subscribed " << topic << endl;

}

void Subscriber::onReceived(const QMQTT::Message& message)
{
	QString data = QString::fromUtf8(message.payload());
    qDebug() << "publish received: \"" << data << "\"" << endl;
	#if 0
   // system("/usr/local/aplay 111.wav -x 127");
    QString dwloadstr= QString("|download");
    QString reloadstr= QString("|reload");
	cmdDecode(data);

   	if(QString::compare(data.left(9),dwloadstr) == 0){
		//emit reloadTemplateSignal();
		//down load the fw.
		emit downloadfwSignal();
   	} 
    else if(QString::compare(data.left(7),reloadstr) == 0)
    {
        //reload template
       	emit reloadTemplateSignal();
    }
	else
   	{
    	emit updateDisplaySignal(data);
    	
   	}
#endif

   emit updateDisplaySignal(data);
	
}
#if 1
ComQmqttThread::ComQmqttThread(QObject *in)
    : QThread(in)
{
  qDebug() <<"ComQmqttThread created";
  psubscriber = NULL;
  psubscriberBkServer = NULL;
  
}
ComQmqttThread::~ComQmqttThread()
{
    
}
void ComQmqttThread::makeConnect()
{
	
}
bool ComQmqttThread::isConnectedMain()
{
   if(psubscriber != NULL){
    	return psubscriber->conStatus();
   	}else return false;
}
bool ComQmqttThread::isConnectedBk()
{
	QString hbkname= m_mainApp->MC_Get_MQTT_Backup_Host_Name();
	
    if(!hbkname.isEmpty()){
		if(psubscriberBkServer !=NULL){
	    	return psubscriberBkServer->isConnectedToHost();
		}else return false;
    }else{
    	return false;
    }
}

void ComQmqttThread::run()
{
    qDebug() <<"subscriber start running now,set clean sesion to ture";

	QString hname = m_mainApp->MC_Get_MQTT_Host_Name();
	QString hbkname= m_mainApp->MC_Get_MQTT_Backup_Host_Name();
	QString hport =m_mainApp->MC_Get_MQTT_Host_Port();
	QString hMacAddr= m_mainApp->MC_Get_Device_MAC();
	qDebug() <<"run:MQTT hostname: "<<hname<<"port"<<hport<<endl;
    quint16 portNum = hport.toInt();
	Subscriber subscriber(hname,portNum,m_mainApp->MC_Get_Https_Flag(),true,NULL);
	Subscriber subscriberBkServer(hbkname,portNum,m_mainApp->MC_Get_Https_Flag(),true,NULL);
	subscriber.setHostName(hname);
	subscriber.setCleanSession(true);
	subscriber.setPort(portNum);
	subscriber.bind(m_mainApp);
	subscriber.setAutoReconnect(true);
	subscriber.setAutoReconnectInterval(30);
	subscriber.setClientId(hMacAddr);
	subscriber.setKeepAlive(90);

	// QString hname =	m_mainApp->MC_Get_MQTT_Host_Name();
	//qDebug() <<"run :"<<hname<<endl;
	
	//subscriber.setHostName(hname);
	connect(&subscriber, &Subscriber::updateDisplaySignal, m_mainApp, &MainApp::MC_Decode_MQTT_Cmd);
	connect(&subscriber, &Subscriber::reloadTemplateSignal, m_mainApp, &MainApp::reloadTemplate);
	connect(&subscriber, &Subscriber::downloadfwSignal, m_FwUpgradeTsk, &FwUpgradeThread::downLoadFW);
	subscriber.moveToThread(this);
    subscriber.connectToHost(); 

	if(!hbkname.isEmpty()){
		//Subscriber subscriberBkServer(hname,portNum,false,true,NULL);
		subscriberBkServer.setHostName(hbkname);
		subscriberBkServer.setPort(portNum);
		subscriberBkServer.bind(m_mainApp);
		subscriberBkServer.setAutoReconnect(true);
		subscriberBkServer.setAutoReconnectInterval(30);
		subscriberBkServer.setClientId(hMacAddr);
		subscriberBkServer.setKeepAlive(90);
		

		// QString hname =	m_mainApp->MC_Get_MQTT_Host_Name();
		//qDebug() <<"run :"<<hname<<endl;
		
		//subscriber.setHostName(hname);
		connect(&subscriberBkServer, &Subscriber::updateDisplaySignal, m_mainApp, &MainApp::MC_Decode_MQTT_Cmd);
		connect(&subscriberBkServer, &Subscriber::reloadTemplateSignal, m_mainApp, &MainApp::reloadTemplate);
		connect(&subscriberBkServer, &Subscriber::downloadfwSignal, m_FwUpgradeTsk, &FwUpgradeThread::downLoadFW);
		subscriberBkServer.moveToThread(this);
	    subscriberBkServer.connectToHost(); 
    }
	psubscriber = &subscriber;
	psubscriberBkServer = &subscriberBkServer;
    exec();

}

void ComQmqttThread::bind( MainApp* ma, FwUpgradeThread* fwugth)
{
    m_mainApp = ma;
	m_FwUpgradeTsk = fwugth;
}

void ComQmqttThread::stopRunning()
{
   
}
#endif

