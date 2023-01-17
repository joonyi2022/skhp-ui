#ifndef COM_HTTPS_H
#define COM_HTTPS_H

#include <QThread>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QDebug>

class MainApp;

class ComHttpsThread : public QThread
{
	Q_OBJECT  

public:
	ComHttpsThread(QObject *in = NULL);   
	~ComHttpsThread();
	void makeConnect();
    void regesiterDevice();
    void bind( MainApp* ma);
    void run();
    void stopRunning();
    void response_q_num( QString num);

public slots:

void replyFinished(QNetworkReply * reply);

signals:
	 void finished(QNetworkReply*);
     void useRemoteTemplateSignal();
     
protected:
  

private:
 
    MainApp*                m_mainApp;
    int m_isRunning;
    QNetworkAccessManager *m_restclient;

};

#endif //COM_HTTPS_H
