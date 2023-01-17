#ifndef FW_UPGRADE_H
#define FW_UPGRADE_H

#include <QThread>
#include <QObject>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QTimer>

class MainApp;

class FwUpgradeThread : public QThread
{
	Q_OBJECT  

public:
	FwUpgradeThread(QObject *in = NULL);   
	~FwUpgradeThread();
	void makeConnect();
    void run();
    void bind( MainApp* ma);

public slots:
    void downLoadFW();
    void replyFinished(QNetworkReply * reply);


signals:
	 void finished(QNetworkReply*);

protected:
  
private:
    QNetworkAccessManager *m_restclient;
    MainApp*                m_mainApp;

};

#endif //FW_UPGRADE_H

