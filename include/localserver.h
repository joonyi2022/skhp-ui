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
    // 有新的连接来了
    void newConnection();
 
    // 可以读数据了
    void readyRead();

private:
    // 判断是否有一个同名的服务器在运行
    int isServerRun(const QString & servername);

signals:
    void newMessage(const QString &msg);
private:
    QLocalServer *m_server;
};
#endif // SERVER_H

