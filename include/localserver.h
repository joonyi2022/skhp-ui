////////////////////////////////////////////////////////////////////////////////
// server.h
/////////////////////////////////////////////////////////////////////////////////
#ifndef SERVER_H
#define SERVER_H

#include <QLocalServer>
#include <QLocalSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    Server();

    ~Server();

    int init(const QString & servername);

private slots:
    // ���µ���������
    void newConnection();
 
    // ���Զ�������
    void readyRead();

private:
    // �ж��Ƿ���һ��ͬ���ķ�����������
    int isServerRun(const QString & servername);

signals:
    void newMessage(const QString &msg);
private:
    QLocalServer *m_server;
};
#endif // SERVER_H

