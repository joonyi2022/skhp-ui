#include <QDir>
#include <QFile>
#include <QWidget>
#include <QtNetwork>
#include <QTextStream>
#include <QDebug>
#include "include/localserver.h"
Server::Server()
{
    m_server = 0;
} 
Server:: ~Server()
{
    if (m_server) 
    {
        delete m_server;
    }
}
int Server::init(const QString & servername)
{
    // 如果已经有一个实例在运行了就返回0
    if (isServerRun(servername)) {
        return 0;
    }
    m_server = new QLocalServer;
    // 先移除原来存在的,如果不移除那么如果
    // servername已经存在就会listen失败
    QLocalServer::removeServer(servername);
    // 进行监听
    m_server->listen(servername);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    return 1;
}

void Server::newConnection()
{
    QLocalSocket *newsocket = m_server->nextPendingConnection(); 
    connect(newsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}  
// 可以读数据了
void  Server::readyRead()
{
    // 取得是哪个localsocket可以读数据了
    QLocalSocket *local = static_cast<QLocalSocket *>(sender()); 
    if (!local)
        return;
    QTextStream in(local);
    QString     readMsg;
    // 读出数据
    readMsg = in.readAll();
    // 发送收到数据信号
    emit newMessage(readMsg);
}

// 判断是否有一个同名的服务器在运行
int Server::isServerRun(const QString & servername)
{
    // 用一个localsocket去连一下,如果能连上就说明
    // 有一个在运行了
    QLocalSocket ls;
    ls.connectToServer(servername);
    if (ls.waitForConnected(1000)){
        // 说明已经在运行了 
        ls.disconnectFromServer();
        ls.close();
        return 1;
    }
    return 0;
}


