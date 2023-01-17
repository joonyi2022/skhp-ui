#include <QApplication>
#include <QMessageBox>
#include <QtGlobal>
#include <QtWidgets>
#include <QLabel>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QList>

#include "include/maincontrol.h"
#include "include/localserver.h"

//log file size 30M
#define LIMITED_LOG_FILE_SIZE 30000000
//#define LOCAL_SERVER

#define USER_DATA_MAX_FILE_NUM 10

#if 1
void myMessageHandler1(QtMsgType type, const QMessageLogContext & , const QString & str)
{

	QString txt;
	QString strCmd = "cp -f /usr/local/log.txt /run/log.txt";
	QFile logFile("/usr/local/log.txt");
	QFile outFile("/run/log.txt");
	const char * msg = str.toStdString().c_str();
	
	switch (type) {
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
	break;
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
	break;
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
		abort();
	}

	if(outFile.exists()){
		if(outFile.size()>LIMITED_LOG_FILE_SIZE){
			outFile.remove();
	    }
		
	} else {
		if(logFile.exists()){
			system(strCmd.toLocal8Bit().data());
		} 
	}
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
	outFile.close();
}


void myMessageHandler(QtMsgType type, const QMessageLogContext &context , const QString & msg)
{
	QString text;
	static QMutex mutex;
	mutex.lock();
	QFile logFile("/usr/local/log.txt");
	QFile outFile("/run/log.txt");
	QString strCmd = "cp -f /usr/local/log.txt /run/log.txt";
 
	switch(type) {
	 	case QtDebugMsg:
	 		text = QString("Debug: ");
	 		break;
	    case QtWarningMsg:
			text = QString("Warning: ");
		 	break;
		case QtCriticalMsg:
			text = QString("Critical:");
			break;
		case QtFatalMsg:
		 	text = QString("Fatal: ");
		 	break;
  		default:
			text = QString("Debug: ");
  	}
    
	//QString context_info = QString("F2:(%1) L:(%2)").arg(QString(context.file)).arg(context.line); // F文件L行数
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
	QString strMessage = QString("%1 \t%2").arg(strDateTime).arg(msg);

    if(outFile.exists()){
		if(outFile.size()>LIMITED_LOG_FILE_SIZE){
			outFile.remove();
		}
    } else if(logFile.exists()){
   		system(strCmd.toLocal8Bit().data());
    }

	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << strMessage << endl;
	outFile.close();
	mutex.unlock();
}

#else

QList<QString> m_dnuserDatafileList;
QString logfilename="";

/* List all zip files. */
QList<QString> dnUserDataZipFileList()
{
    QString datadir = "/usr/local/log";
    QDir mDir(datadir);
    QString zipfilename;
	
    m_dnuserDatafileList.clear();
    foreach(QFileInfo item, mDir.entryInfoList(QDir::Files,
            QDir::Time |
            QDir::Reversed ))
    {
        if(item.exists()) {
            QString suffix = item.suffix();
            if(QString::compare(suffix, QString("gz"), Qt::CaseInsensitive) == 0) {
                zipfilename = datadir + item.fileName();
                m_dnuserDatafileList.append(zipfilename);
               // printf("userDataZipFileList::gzfile = %s\n", zipfilename.toLocal8Bit().data());
            }
        }
    }
    return m_dnuserDatafileList;
}

/* Generate user data to zip file. */
void userDataToZipFile(QString filename)
{
	QProcess* m_zip_file;

    if(m_zip_file == NULL) {
        m_zip_file = new QProcess;
    }
    QFile tmpfile;
    //QString fileDataName = userDataFileName();
    QString fileDataName = filename;
    if(true == tmpfile.exists(fileDataName)) {
        QString cmdline = QString("gzib -f ") + fileDataName;
        m_zip_file->start(cmdline.toLocal8Bit().data());
       // printf("FileUtil::userDataToZipFile start. \n");
        if (!m_zip_file->waitForFinished())
            return;
    }
    return ;
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context , const QString & msg)
{
  QString text;
 static QMutex mutex;
 mutex.lock();
 
 switch(type) {
 	case QtDebugMsg:
 		text = QString("Debug: ");
 	break;
    case QtWarningMsg:
		 text = QString("Warning: ");
	 break;
	case QtCriticalMsg:
		 text = QString("Critical:");
		break;
	 case QtFatalMsg:
	 	text = QString("Fatal: ");
	 break;
  default:
	text = QString("Debug: ");
  }
    
	QString context_info = QString("F2:(%1) L:(%2)").arg(QString(context.file)).arg(context.line); // F文件L行数
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
	QString strMessage = QString("%1 %2 \t%3 \t%4").arg(text).arg(context_info).arg(strDateTime).arg(msg);
	 // unlock
	 QDir dir("/usr/local/log/");
if (!dir.exists())QDir().mkdir("/usr/local/log/");

     if(QString(logfilename).isEmpty()){
		 logfilename=QString("/usr/local/log/")+strDateTime+QString("-log.txt");
     }
	 QFile outFile(logfilename);
	if(outFile.exists()&&(outFile.size()>LIMITED_LOG_FILE_SIZE)){
		//outFile.remove();
		//zip log file;
		 /* zip current file and create a new file */
        userDataToZipFile(logfilename);
        dnUserDataZipFileList();
        int num = m_dnuserDatafileList.size();
        if(num > USER_DATA_MAX_FILE_NUM) {
            QString cmdline = QString("rm -f ") + m_dnuserDatafileList.at(0);
            system(cmdline.toLocal8Bit().data());
        }
		logfilename="";
	}
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << strMessage << endl;
	outFile.close();
	 mutex.unlock();
}

#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainApp* myapp = new MainApp();
	qInstallMessageHandler(myMessageHandler);
	
	#ifdef LOCAL_SERVER

		QLabel		 text("test");
		Server		 s;
		if (!s.init("/tmp/mtr")){
			// 初使化失败, 说明已经有一个在运行了
			qDebug() << "local server run at /tmp/mtr!"<<endl;
			//QMessageBox::information(&text, "info", "There is already exist one!"); 
			//return 1;
		} else {
			//QObject::connect(&s, SIGNAL(newMessage(const QString &)),&text, SLOT(setText(const QString &)));
			//text.show();
			//QMessageBox::information(&text, "info", "local server ready at /tmp/mtr!"); 
			qDebug() << "local server ready at /tmp/mtr!"<<endl;
			QObject::connect(&s, SIGNAL(newMessage(const QString &)), myapp, SLOT(UpdateMeetingRoomDisplay(const QString &)));
		}
			
	#endif
	
    myapp->start();
    return app.exec();
}


