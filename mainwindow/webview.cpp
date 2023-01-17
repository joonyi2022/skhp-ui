#include <QDir>
#include <QString>
#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QWidget>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>

#include "include/common.h"
#include "include/maincontrol.h"
#include "include/webview.h"



/****************************************************/

WebView::WebView() :
    m_file(new QFile())
{
    this->setWindowFlags(Qt::FramelessWindowHint);
	m_hide_flag = false;

    connect(this->page()->networkAccessManager(),
            SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )),
            this,
            SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & )));  
}

WebView::~WebView()
{

    if(m_file != NULL) {
        delete m_file;
        m_file = NULL;
    }
}

void WebView::bind( MainApp* ma)
{
    m_mainApp = ma;
}

void WebView::makeConnect()
{
 	connect(this->page()->currentFrame(), &QWebFrame::contentsSizeChanged,this,  &WebView::contentsSizeChangedHandle);
    connect(this, SIGNAL(loadFinished(bool)), SLOT(onLoadFinished(bool)));
	connect(this, &WebView::stopfreshTimer, m_mainApp, &MainApp::UpdateDisplayStopTimer);
	connect(this, &WebView::showFullLoadImageSig, m_mainApp, &MainApp::showFullLoadImage);
}

void WebView::handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    qDebug() << "handleSslErrors: ";
    foreach (QSslError e, errors)
    {
        qDebug() << "ssl error: " << e;
    }

    reply->ignoreSslErrors();
}

void WebView::contentsSizeChangedHandle(const QSize& size)
{
    //printf("HtmlView::contentsSizeChanged");
     qDebug() <<"contentsSizeChanged";
	 //system("echo full > /proc/epdc_fb/update_full");
	 //emit stopfreshTimer();

}

void WebView::onLoadFinished(bool okay)
{
    //printf("HtmlView::onLoadFinished \n");
    qDebug() <<"onLoadFinished";
	
	emit showFullLoadImageSig();

}


void WebView::set_hide_flag(bool flag)
{
 	m_hide_flag = flag;
}

void WebView::show_disconnect_charging_status()
{
	QRect rect;
	QString displayStr="";
	QPainter painter(this);

	//if(m_mainApp->MC_Get_Template_Mode() == LOCAL_TEMPLATE_MODE)return;
  
	QFont disp_font("Arial",15);
	painter.setFont(disp_font);
	painter.setPen(Qt::white);
	if(m_mainApp->MC_Get_Con_Status() && m_mainApp->MC_Get_Charging_Status()) return;

    if(!m_mainApp->MC_Get_Con_Status()){
		displayStr="No Network!\n";
    }
	if(!m_mainApp->MC_Get_Charging_Status()){
		displayStr+="No Charging!\n";
    }
	//if(!m_mainApp->MC_Get_Mqtt_Con_Status()){
	//	displayStr+="No Data!";
    //}
	if(m_mainApp->MC_Get_Device_Display_Width()== 2200) {
		rect = QRect(10, 1550, 200, 100);
	} else if(m_mainApp->MC_Get_Device_Display_Width()== 1600) {
		rect = QRect(10, 1100, 200, 100);
	}else{
		rect= QRect(0, 1540, 150, 60);
	}
	painter.fillRect(rect,Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter,displayStr);

}

void WebView::paintEvent(QPaintEvent *event)
{

    QWebView::paintEvent(event);
    QPainter painter(this);
	QRect rect;
	QRect rectIfinfor;
	QString f97HoriHostName = "/usr/local/epdc_9n7_h";

	qDebug() <<"paintEvent ";
	m_mainApp->MC_refresh_count_update();
    /*for production,no more debug info display*/
	#ifdef SKH_PRODUCTION
		//if(m_mainApp->MC_Get_Device_Display_Width()!= 2200) {
		//only for 9.7 inch
			show_disconnect_charging_status();
		//}
		return;
	#endif

	#ifdef SKH_HK_DEMO
		/*hk demo don't show battery and config*/
	    return;
	#endif

	#ifdef SKH_DEMO_DED_INFO
		/*demo bed info*/
		return;
	#endif
	//if(m_mainApp->MC_Get_Template_Mode() == LOCAL_TEMPLATE_MODE) {
	if(1){
	    QString batLevel = m_mainApp->getBatterLevel();
		QString version = m_mainApp->MC_Get_FwVersion();
		QString appversion = m_mainApp->MC_Get_UiAppVersion();
		#if 0
			QString mqtt_msg = m_mainApp->MC_Get_mqtt_msg();
			QString displayStr="\n"+mqtt_msg+ "\n"+version+" ui_app_version:"+app_version+"\n"+batLevel;
		#else
			QString displayStr="\n"+version+" ui_app_version:"+appversion+"\n"+batLevel;
		#endif

		qDebug() <<"paintEvent draw battery";
		qDebug() <<displayStr;

		QFont disp_font("Arial",15);
	    painter.setFont(disp_font);
	    painter.setPen(Qt::white);

		if(m_mainApp->MC_Get_Device_Display_Width()== 2200) {
			rect = QRect(500, 1350, 1200, 270);
		} else if(m_mainApp->MC_Get_Device_Display_Width()== 1600) {
			rect = QRect(200, 900, 1200, 270);
		} else if(m_mainApp->MC_Get_Device_Display_Width()== 1200) {
			rect = QRect(50, 1300, 1100, 270);
		} else {
		    #ifdef EINK_FACTORY_AGING_TESTING
				rect= QRect(50, 850, 700, 270);
			#else
				if(QFile::exists(f97HoriHostName)) {
					rect= QRect(50, 500, 700, 270);
				} else {
					rect= QRect(50, 850, 700, 270);
				}
			#endif
		}

	    painter.fillRect(rect,Qt::black);
	    painter.setPen(Qt::white);
	    painter.drawText(rect, Qt::AlignCenter,displayStr);
	#ifdef SKH_DEVICE_INFO_SHOW
		QString ifInfo = m_mainApp->MC_Get_DeviceInfo();
			  
        // qDebug() <<"ifconfig:"<<ifInfo;
		if(m_mainApp->MC_Get_Device_Display_Width()== 2200) {
	    	rectIfinfor = QRect(500, 200, 1200, 1150);
		}else if(m_mainApp->MC_Get_Device_Display_Width()== 1600) {
	    	rectIfinfor = QRect(200, 100, 1200, 800);
			
		}else if(m_mainApp->MC_Get_Device_Display_Width()== 1200) {
	    	
			rectIfinfor = QRect(50, 200, 1100, 1100);
		} else {
			rectIfinfor= QRect(50, 100, 700, 750);
			 
		}
		painter.fillRect(rectIfinfor,Qt::black);
	    painter.setPen(Qt::white);
	    painter.drawText(rectIfinfor, Qt::AlignLeft,ifInfo);
	#endif
		
    }
	
}


