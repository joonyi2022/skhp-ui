#ifndef COM_QMQTT_H
#define COM_QMQTT_H

#include <QThread>
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <qmqtt.h>

class MainApp;
class FwUpgradeThread;

class Publisher : public QMQTT::Client
{
    Q_OBJECT
public:
    explicit Publisher(const QHostAddress& host ,
                       const quint16 port ,
                       QObject* parent);
    virtual ~Publisher();

    QTimer _timer;
    quint16 _number;

public slots:
    void onConnected();
    void onTimeout();
    void onDisconnected();
};

class Subscriber : public QMQTT::Client
{
    Q_OBJECT
public:

 //   explicit Subscriber(const QHostAddress& host,
 //                       const quint16 port,
 //                       QObject* parent);
    explicit Subscriber(const QString& host,
                    const quint16 port,
                    const bool ssl,
                    const bool ignoreSelfSigned ,
                    QObject* parent);
    virtual ~Subscriber();
    QTextStream _qout;
    void bind( MainApp* ma);
    bool conStatus();
    void cmdDecode(QString data);
signals:
    void updateDisplaySignal(QString);
    void reloadTemplateSignal();
    void downloadfwSignal();

public slots:
    void onDisconnected();
    void onConnected();
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message& message);

private:
    MainApp* m_mainApp;
    QString m_template_url;
    QString m_template_data;
    bool m_connection_status;
};
#if 1


class ComQmqttThread : public QThread
{
	Q_OBJECT  

public:
	ComQmqttThread(QObject *in = NULL);   
	virtual ~ComQmqttThread();
	void makeConnect();
    void run();
    void stopRunning();
    void bind( MainApp* ma, FwUpgradeThread* fwugth);
    bool isConnectedMain();    
    bool isConnectedBk();

public slots:


protected:
 

private:
    
    MainApp*                m_mainApp;
    FwUpgradeThread*        m_FwUpgradeTsk;

    int m_isRunning;
    Subscriber *psubscriber;
	Subscriber *psubscriberBkServer;
};
#endif
#endif //COM_QMQTT_H
