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
    // ����Ѿ���һ��ʵ���������˾ͷ���0
    if (isServerRun(servername)) {
        return 0;
    }
    m_server = new QLocalServer;
    // ���Ƴ�ԭ�����ڵ�,������Ƴ���ô���
    // servername�Ѿ����ھͻ�listenʧ��
    QLocalServer::removeServer(servername);
    // ���м���
    m_server->listen(servername);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    return 1;
}

void Server::newConnection()
{
    QLocalSocket *newsocket = m_server->nextPendingConnection(); 
    connect(newsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}  
// ���Զ�������
void  Server::readyRead()
{
    // ȡ�����ĸ�localsocket���Զ�������
    QLocalSocket *local = static_cast<QLocalSocket *>(sender()); 
    if (!local)
        return;
    QTextStream in(local);
    QString     readMsg;
    // ��������
    readMsg = in.readAll();
    // �����յ������ź�
    emit newMessage(readMsg);
}

// �ж��Ƿ���һ��ͬ���ķ�����������
int Server::isServerRun(const QString & servername)
{
    // ��һ��localsocketȥ��һ��,��������Ͼ�˵��
    // ��һ����������
    QLocalSocket ls;
    ls.connectToServer(servername);
    if (ls.waitForConnected(1000)){
        // ˵���Ѿ��������� 
        ls.disconnectFromServer();
        ls.close();
        return 1;
    }
    return 0;
}


