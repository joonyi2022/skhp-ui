
#include <QDir>
#include <QFile>
#include <QWidget>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#include <QQueue>
#include <QTime>
#include <QSslConfiguration>
#include "include/common.h"
#include "include/webview.h"
#include "include/maincontrol.h"
#include "include/com_https.h"
#include "include/com_qmqtt.h"
#include "include/fw_upgrade.h"
#include "include/localserver.h"


#define DEFAULT_TEMPLATE_NAME_HIDDEN     "sample_template_bk.html"
#define DEFAULT_TEMPLATE_NAME            "sample_template.html"

#define DEFAULT_133_INACTIVE_TEMPLATE        "/usr/local/template_inactive_off/133_inactive_off.html"
#ifdef CS_TESTING
#define DEFAULT_HTTP_HOST_NAME           "127.0.0.1"
#else
//#define DEFAULT_HTTP_HOST_NAME           "SKHEPAPVPAPP00.shses.shs.com.sg"
#define DEFAULT_STAGING_HTTP_HOST_NAME           "10.128.15.21"
#define DEFAULT_PRODUCT_HTTP_HOST_NAME           "10.128.9.51"

#endif

#define DEFAULT_SLEEP_TIME             "20:00"
#define DEFAULT_SLEEP_HOURS            "10"
#define DEFAULT_AUTO_WAKEUP_INTERVAL   "5"
#define DEFAULT_AUTO_SLEEP_INTERVAL    "15"
#define DEFAULT_DISCONNECT_THRESHOLD    "24" /*24*5=120 MINS*/





#define DEFAULT_STAGING_MQTT_HOST_NAME           "10.128.15.21"
#define DEFAULT_PRODUCT_MQTT_HOST_NAME           "10.128.9.55"
#define DEFAULT_PRODUCT_MQTT_BK_HOST_NAME        "10.128.9.56"
#define DEFAULT_MQTT_HOST_PORT                   "1883"

#define MAX_MQTT_QUEUE                    5
#define DEFAULT_DEVICE_MAC_ADDRESS       "01:02:03:04:05:06"
#define DEFAULT_DEVICE_IP_ADDRESS        "xx"

#define DEFAULT_DEVICE_MAC_ADDRESS       "01:02:03:04:05:06"
#define DEFAULT_DEVICE_IP_ADDRESS        "xx"

#define TIMER_INTERVAL         			 900  /*ms*/
#define TIMER_BLINKING_INTERVAL          600  /*ms*/
#define REFRESH_NUMER                    6
#define TIMER_INTERVAL_NETWORK_CHECK     7000 /*1000MS*/

#ifdef SKH_HK_DEMO
#define DEMO_DISPLAY_INTERVLAL           30000 

#else
#define DEMO_DISPLAY_INTERVLAL           15000 

#endif
#define DEMO_REFRESH_INTERVAL            1
#define DEMO_SHOW_BATTERY_INTERVAL       20000
#define MAX_SOUND_COUNT                  3

#define BLINK_INTERVAL                   1000
#define SHUT_OFF_REFRESH_DELAY           3000
#define DEVICE_INFO_REFRESH_DELAY        8000
#define TIMER_RREFLESH_DELAY             4000
#define TIMER_RREFLESH_SHORT_DELAY       700
#ifdef AUTO_SHUTOFF
/*1 mins for wifi wake and sleep usage*/
#define RUNTIME_TIMER_MINS               1 
#define WEB_PING_INTERVAL                4

#else
/*if no auto shutoff, then 5 mins for pin*/
#define RUNTIME_TIMER_MINS               5   
#endif


#define QUEUE_NUM_BUFFER                 5
#define QUEUE_NUMBER_DELAY               5000
#define FIRST_LOADING_REFRESH_INTERVAL   700
#define LOADING_REFRESH_INTERVAL         1900
#define HTTP_REPLY_TIMEOUT               120000         
#define FULL_LOADING_DELAY               1000
#define TEMPLATE_133_CHANGE_DELAY        700

 

MainApp::MainApp()
{
  MC_Get_Device_Display_Size();

  this->setGeometry(0,0,g_screen_width,g_screen_height);
  //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
  setWindowState( Qt::WindowFullScreen );
  #ifdef SKH_DEVICE_INFO_SHOW
    m_webView = new WebView();
    m_webView->bind(this);
    m_webView->makeConnect();
    m_webView->setGeometry(0,0,g_screen_width,g_screen_height);
    connect(&_demoTimer, &QTimer::timeout, this, &MainApp::onDemoTimeout);
    connect(&_timerFeedWatchDog, &QTimer::timeout, this, &MainApp::onWatchDogTimeout);

    QSslConfiguration sslconf = QSslConfiguration::defaultConfiguration();
    QList<QSslCertificate> cert_list = sslconf.caCertificates();
    QList<QSslCertificate> cert_new = QSslCertificate::fromData("CaCertificates");
    cert_list += cert_new;
   
    sslconf.setCaCertificates(cert_list);
    sslconf.setProtocol(QSsl::AnyProtocol);
    QSslConfiguration::setDefaultConfiguration(sslconf);
 
  #else
    #ifndef SKH_POWER_OFF_SHOW
      m_webView = new WebView();

      #ifdef SKH_HK_DEMO
        m_webViewHiden = new WebView();
      #endif

      m_ComHttpsTsk = new ComHttpsThread();
      m_ComQmqttTsk = new ComQmqttThread();
      m_FwUpgradeTsk = new FwUpgradeThread();  
      m_webView->bind(this);

      #ifdef SKH_HK_DEMO
	      m_webViewHiden->bind(this);
      #endif

      m_ComQmqttTsk->bind(this,m_FwUpgradeTsk);
      m_ComHttpsTsk->bind(this);
      m_FwUpgradeTsk->bind(this);
      m_webView->makeConnect();

      #ifdef SKH_HK_DEMO
	      m_webViewHiden->makeConnect();
      #endif

      m_ComHttpsTsk->makeConnect();
      m_ComQmqttTsk->makeConnect();
      m_FwUpgradeTsk->makeConnect();
      m_webView->setGeometry(0,0,g_screen_width,g_screen_height);

      #ifdef SKH_HK_DEMO
	      m_webViewHiden->setGeometry(0,0,g_screen_width,g_screen_height);
      #endif

      m_num =1000;
      _number = 0;
      m_http_host_name = DEFAULT_STAGING_HTTP_HOST_NAME;
      m_mqtt_host_name = DEFAULT_STAGING_MQTT_HOST_NAME;
      m_mqtt_bk_host_name = "";
      m_auto_sleep_time = DEFAULT_SLEEP_TIME;
      m_auto_sleep_hours = DEFAULT_SLEEP_HOURS;

      m_auto_sleep_interval = DEFAULT_AUTO_SLEEP_INTERVAL;
      m_auto_wakeup_interval= DEFAULT_AUTO_WAKEUP_INTERVAL;
      m_dischonnect_threshold = DEFAULT_DISCONNECT_THRESHOLD;
      m_mqtt_host_port = DEFAULT_MQTT_HOST_PORT;
      m_mac_address = DEFAULT_DEVICE_MAC_ADDRESS;
      m_ip_address = DEFAULT_DEVICE_IP_ADDRESS;
      m_mqtt_msg = "no msg";
  
      demo_call_number = 1000;
      demo_docname_index = 0;
      m_sound_count = 0;
      m_dingdong_sound_count = 0;
      m_max_count = MAX_SOUND_COUNT;
      m_speaker_vol = "127";
      m_speaker_beep = "1";
      m_call_action = "1";
      m_device_status = "normal";
      m_updatewin_data_para = "00000";
      m_template_changed = false;
      m_device_register = false;
      m_showing_number = false;
      m_current_blank_number = false;
      m_bat_charging_status = true;
      m_queue_number_time = "01";
      m_pre_msg_id = "0";
      m_showfullscreen = 0;
      m_disconnect_count = 0;
      m_fullrefresh97_count = 0;
      m_shutdown_133_template = "";
      m_default_133_template = "";
      m_memory_percentage = "45";
      m_cpu_percentage = "0.5";
      
      #ifdef SKH_HK_DEMO
        m_exchange = 1;
      #endif

      m_blinking_interval = BLINK_INTERVAL;//ms
      m_fwVersionStr = "";
      m_shutdown_template = DEFAULT_133_INACTIVE_TEMPLATE;
      m_shutdown_timer_count = 120;//120minutes
      m_commqtt_tsk_runing = false;
      m_web_con = false;
      m_https_exsit=false;
      m_template_change_refresh_interval = 5000;//ms
      m_content_chang_refresh_interval = 700;//ms
      connect(&_timerClrCache, &QTimer::timeout, this, &MainApp::onTimeClrCacheout);
      connect(&_timerFeedWatchDog, &QTimer::timeout, this, &MainApp::onWatchDogTimeout);
      connect(&_timer, &QTimer::timeout, this, &MainApp::onTimeout);
      connect(&_timerBlinking, &QTimer::timeout, this, &MainApp::blinkingTimeout);
      connect(&_timerNetworkCheck, &QTimer::timeout, this, &MainApp::onNetworkCheckTimeout);
      connect(&_demoTimer, &QTimer::timeout, this, &MainApp::onDemoTimeout);
      connect(&_demoTimerBattery, &QTimer::timeout, this, &MainApp::onDemoBatTimeout);
      connect(&_runtimeTimer, &QTimer::timeout, this, &MainApp::onRuntimeTimeout);
      connect(&_shutdownTimer, &QTimer::timeout, this, &MainApp::onShutdownTimeout);
      connect(&_timerReflesh, &QTimer::timeout, this, &MainApp::onRefleshTimeout);
      connect(&_timerJustReflesh, &QTimer::timeout, this, &MainApp::onJustRefleshTimeout);
      connect(&_queueNumTimer, &QTimer::timeout, this, &MainApp::onQueueNumTimerTimeout);
      connect(&_timerHttpReply, &QTimer::timeout, this, &MainApp::onHttpReplyTimerTimeout);
      connect(&_timerReflesh97, &QTimer::timeout, this, &MainApp::onRefleshTimeout97);
      connect(&_timerReflesh133, &QTimer::timeout, this, &MainApp::onRefleshTimeout133);
      connect(&_timerFullLoading, &QTimer::timeout, this, &MainApp::onFullLoadTimeout);
      connect(this, &MainApp::replyQnumberSignal, m_ComHttpsTsk, &ComHttpsThread::response_q_num);
	
      demo_docname<<"Dr Lou Ai Ren"<<"Dr Tan Chong Kee"<<"Dr Lee I Wuen"<<"Dr Lim Sheow Lei"<<"Dr Wong Wai Loong"<<"Dr Soh Lay Tin";
    #else
      m_webView = new WebView();
      m_webView->bind(this);
      m_webView->makeConnect();
      m_webView->setGeometry(0,0,g_screen_width,g_screen_height);
      connect(&_demoTimer, &QTimer::timeout, this, &MainApp::onDemoTimeout);
    #endif  // SKH_POWER_OFF_SHOW
  #endif  // SKH_DEVICE_INFO_SHOW

  m_restclient = new QNetworkAccessManager();
  m_mqttreply = new QNetworkAccessManager();
  //QSslConfiguration sslconfig;
  //sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
  //sslconfig.setProtocol(QSsl::TlsV1);
  //sslconfig.setPeerVerifyMode(QSslSocket:VerifyNone);
  //m_restclient->moveToThread(this);
  connect(m_restclient, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::DirectConnection);
  connect(m_mqttreply, SIGNAL(finished(QNetworkReply*)), this, SLOT(mqttReplyFinished(QNetworkReply*)),Qt::DirectConnection);
  connect(m_restclient, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & ))); 
  qDebug() << "ssl app_version:"<< app_version<<endl;

}

MainApp::~MainApp()
{

  if(m_webView != NULL) {
    delete m_webView;
    m_webView = NULL;
  }

	#ifdef SKH_HK_DEMO
	  if(m_webViewHiden != NULL) {
      delete m_webViewHiden;
      m_webViewHiden = NULL;
    }
	#endif
}

void MainApp::start()
{
	MC_Check_Https_Exsit();
  MC_Read_VersionNumber();
	MC_Read_UiAppVersionNumber();
	MC_Read_Device_Config();
	MC_Read_Disconnect_Threshold_Config();
  
  #ifdef OPEN_WATCHDOG
    MC_Start_Watchdog_Timer();
  #endif
  
  #ifdef AUTO_SHUTOFF
	  MC_Read_Auto_Sleep_Wakeup_Config();
  #endif

  #ifdef SKH_DEVICE_INFO_SHOW
    MC_LoadDeviceInfo();
  #else
    #ifndef SKH_POWER_OFF_SHOW
	    MC_Read_Host_Name();
	    MC_Read_Device_MAC();
      MC_Read_Life_Time_Data();
	    readBatterVol();
	    readCPUInfo();
	    readMemInfo();

	    if(!m_screen_13n3_exit){
		    MC_Picture_Mode();
	    }
	
      #ifndef LOCAL_SERVER
        if(MC_Check_Host_Network(0) == true) {
		      MC_Read_Device_Ip_Addr();
   		    qDebug() << "network is good 2\r\n";
    	    LoadDefaultTemplate();
	        // m_ComHttpsTsk->start();
	        // regesiterDevice();
	        // m_commqtt_tsk_runing = true;
	        //  m_ComQmqttTsk->start();
   	    } else {
   	      qDebug() << "network is not ready \r\n";
		      regesiterDevice();
		      MC_LoadLocalTemplate();
		      /*start a network checking timer*/
        }

      #else
        qDebug() << "meeting room display \r\n";
        MC_LoadDefaultMeetingRoomTemplate();
      #endif  // LOCAL_SERVER

    #else
      #if SKH_WHITE_SHOW
        MC_LoadWhiteTemplate();
      #elif SKH_BLACK_SHOW
        MC_LoadBlackTemplate();
      #else
        MC_LoadShutOffTemplate();
      #endif

    #endif // SKH_POWER_OFF_SHOW
  #endif  // SKH_DEVICE_INFO_SHOW
}

//   faster mode:echo mode 1 > /proc/epdc_fb/update_mode
//   picture mode:echo mode 0 > /proc/epdc_fb/update_mode

void MainApp::MC_Start_Ping()
{
	QString cmdline;
  cmdline = "ping -w 2 "+ m_http_host_name +" -c 2 > /tmp/ping";
  qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
  system(cmdline.toLocal8Bit().data());
}

void MainApp::Full_reflesh()
{
 	system("echo full > /proc/epdc_fb/update_full");
	m_refresh_count++;
}

void MainApp::MC_Picture_Mode()
{
	system("echo mode 0 > /proc/epdc_fb/update_mode");
}

void MainApp::MC_Speed_Mode()
{
	system("echo mode 1 > /proc/epdc_fb/update_mode");
}

void MainApp::onQueueNumTimerTimeout()
{
  _queueNumTimer.stop();
  MC_Mqtt_CMD_Handler(m_mqtt_queue.dequeue());
}

void MainApp::onHttpReplyTimerTimeout()
{
   qDebug() << "on onHttpReplyTimerTimeout test";
   _timerHttpReply.stop();

	if(m_web_con==true){
 		m_web_con=false;
		qDebug() << "show no network" ;
 	}
	m_webView->update();
	m_disconnect_count++;
	qDebug() << "show no network with mins" <<QString::number(m_disconnect_count*5);
	int dischonnect_threshold= m_dischonnect_threshold.toInt();
	if(m_disconnect_count>=dischonnect_threshold){
		//if no response continually for 2 hours, just reboot.
		qDebug() << "disconnect too long, reboot!";
		system("reboot");
		m_disconnect_count = 0;
	}
}	  


void MainApp::onJustRefleshTimeout()
{
  qDebug() << "on onJustRefleshTimeout ";
  _timerJustReflesh.stop();
  if(!m_screen_13n3_exit){
  	Full_reflesh();
  } else {
    // for 13.3 display
  	m_webView->update();
  } 
}

void MainApp::onRefleshTimeout()
{
  qDebug() << "on onRefleshTimeout test";
  _timerReflesh.stop();
  Full_reflesh();
  if(m_commqtt_tsk_runing == false) {
	  if(m_template_mode ==REMOTE_TEMPLATE_MODE ){
		  m_commqtt_tsk_runing = true;
  		m_ComQmqttTsk->start();
		  qDebug() << "on mqtt start";
    }
  }
}

void MainApp::onFullLoadTimeout()
{
	qDebug() << "onFullLoadTimeout";

	_timerFullLoading.stop();
	
	
	 if(m_template_changed) {
		qDebug()<< "template changed";
		m_webView->showFullScreen();

		if(m_screen_13n3_exit){
		  // for 13.3 display
			/*wait to show out*/
			//QThread::msleep(400);
			//QString params2 = QString("updateWindowDisplay('%1')").arg();
			qDebug()<<m_updatewin_data_para;
			qDebug()<< "133 update data";
			m_webView->page()->mainFrame()->evaluateJavaScript(m_updatewin_data_para);

			 _timerReflesh133.stop();
			//_timerReflesh.start(m_template_change_refresh_interval);
			//_timerReflesh133.start(TEMPLATE_133_CHANGE_DELAY);
			//use config file to dynamic change refresh time
			_timerReflesh133.start(m_template_change_refresh_interval);
			
			#if 0
			  if(m_max_count >0){
				  _timerBlinking.start(m_blinking_interval);
				  m_dingdong_sound_count = 1;
			  } else {
				  m_showing_number = false;
			  }
			#endif
		} else {
		  //for 9.7 bed info
		  /*wait to show out*/
		  qDebug()<<m_updatewin_data_para;
		  qDebug()<< "97 update data";
		  m_webView->page()->mainFrame()->evaluateJavaScript(m_updatewin_data_para);
		  m_showing_number = false;
			_timerReflesh97.stop();
			//_timerReflesh.start(m_template_change_refresh_interval);
			_timerReflesh97.start(m_template_change_refresh_interval);
		}

		qDebug()<< "template changed end";
		m_template_changed = false;

	} else {
    if( m_showfullscreen ==1){
		  qDebug()<< "show full screen";
		  m_webView->showFullScreen();
		  /*wait to show out*/
		  //QThread::msleep(100);
		  m_showfullscreen= 0;
		  m_showing_number = false;
		  //Full_reflesh();
		  qDebug()<< "reflesh";
		}
	}
}


void MainApp::onRefleshTimeout133()
{
  qDebug() << "on onRefleshTimeout133 full refresh ;";
  _timerReflesh133.stop();
  Full_reflesh();
  if(m_max_count >0) {
    _timerBlinking.start(m_blinking_interval);
  	
    if(QString::compare(m_speaker_beep,"1") == 0) {
  	 	//if speaker beep 1, then need to sound
  	 	m_dingdong_sound_count = 1;
  		qDebug() << "blinking and sound;";
  	 } else {
  	   qDebug() << "blinking but no sound ;";
  	 }
   } else {
  	 m_showing_number = false;
  	 qDebug() << "no blinking and sound ,just full refresh ;";
   }
}

void MainApp::onRefleshTimeout97()
{
  qDebug() << "on onRefleshTimeout97 full refresh times: " + m_fullrefresh97_count;
  _timerReflesh97.stop();
  if(m_fullrefresh97_count == 0){
    m_fullrefresh97_count++;
    Full_reflesh();
    _timerReflesh97.start(1000);
  } else if(m_fullrefresh97_count == 1) {
    m_fullrefresh97_count=0;
    Full_reflesh();
  }
}

void MainApp::onDemoTimeout()
{
  #ifdef SKH_DEVICE_INFO_SHOW
    MC_Read_DeviceInfo();
    readBatterLevel();
    m_webView->update();
    Full_reflesh();
  #else
    #ifndef SKH_POWER_OFF_SHOW
      qDebug() << "on onDemoTimeout";
      m_bat_show = false;
      // _demoTimerBattery.start(DEMO_SHOW_BATTERY_INTERVAL);
      if(MC_Check_Host_Network(0) == true) {
        MC_Read_Device_Ip_Addr();
        MC_LoadRomoteTemplate();
      } else {
        regesiterDevice();
        //get_last_config();
        #ifndef SKH_PRODUCTION
          /*no more queue number display*/
          UpdateDisplayDemo();
        #endif
      }
    #else 
      _demoTimer.stop();
      Full_reflesh();
    #endif  // SKH_POWER_OFF_SHOW
  #endif  // SKH_DEVICE_INFO_SHOW
}


void MainApp::onShutdownTimeout()
{
    qDebug() << "onShutdownTimeout";
	m_template_url = m_shutdown_template;
	_shutdownTimer.stop();
	MC_LoadInactiveTemplate();
	_timerReflesh.start(TIMER_RREFLESH_DELAY);
	m_showing_number = false;

}


void MainApp::onDemoBatTimeout()
{
  qDebug() << "on onDemoBatTimeout";
	m_bat_show = true;
   m_webView->update();
	_demoTimerBattery.stop();
}

void MainApp::onRuntimeTimeout()
{
  #ifdef AUTO_SHUTOFF
    QTime currTime = QTime::currentTime();
	  int curHour = currTime.hour() ;
	  int curMin = currTime.minute() ;
	  int sleep_interval= m_auto_sleep_interval.toInt();

    if(m_bat_charging_status)
	  {
		  //for charing, only display charging UI
		
	  } else {
    	qDebug() << "curMin:"<<QString::number(curMin);
    	m_runtime_mins += RUNTIME_TIMER_MINS;

    	if((m_device_register == false) || 
    	   (m_web_con == false) ||
    	   (curMin%WEB_PING_INTERVAL==0)
    	   ){
    	  qDebug() << "_runtimeTimer added:"<<QString::number(m_runtime_mins);
    		MC_Write_Life_Time_Data();
    		readBatterVol();
    		readCPUInfo();
    		readMemInfo();
    		regesiterDevice();
    	}

    	if(m_device_register == true && m_web_con == true){
    		qDebug() << "sleep_interval:"<<QString::number(sleep_interval);
    		if((curMin+1)%sleep_interval==0){
    			MC_Device_Auto_Sleep();
    		}
    	}
    }
  #else
	  m_runtime_mins += RUNTIME_TIMER_MINS;
	  MC_Write_Life_Time_Data();
	  readBatterVol();
	  readCPUInfo();
	  readMemInfo();
	  regesiterDevice();
  #endif  // AUTO_SHUTOFF
}

bool MainApp::showBat()
{
  qDebug() << "show bat";
  return m_bat_show;
}

void MainApp::readCPUInfo(void)
{

  #if 0
	  QFile cpuInfoLine("/run/cpu_used");
    if(cpuInfoLine.exists()){
		  cpuInfoLine.remove();
    } 
	  QString ipCmd ="top -bn1 | sed -n '/Cpu/p' | awk '{print $2}' | sed 's/..,//' > /run/cpu_used";
	  qDebug() <<"ipCmd:"<<ipCmd<<endl;
    system(ipCmd.toLocal8Bit().data());
	  cpuInfoLine.open(QIODevice::ReadOnly | QIODevice::Text);
	  m_cpu_percentage.clear();
    m_cpu_percentage = cpuInfoLine.readAll();
	  m_cpu_percentage = m_cpu_percentage.simplified();
    cpuInfoLine.close();
	#endif

	return;
	
}

void MainApp::readMemInfo(void)
{
  #if 0
	  QFile memInfoLine("/run/mem_used");
    if(memInfoLine.exists()){
		  memInfoLine.remove();
    } 
	  QString ipCmd ="free | grep Mem | awk '{print $3/$2 * 100.0}' > /run/mem_used";
	  qDebug() <<"ipCmd:"<<ipCmd<<endl;
    system(ipCmd.toLocal8Bit().data());
	  memInfoLine.open(QIODevice::ReadOnly | QIODevice::Text);
	  m_memory_percentage.clear();
    m_memory_percentage = memInfoLine.readAll();
	  m_memory_percentage = m_memory_percentage.simplified();
    memInfoLine.close();
	#endif
	
  return;
}

void MainApp::readBatterVol()
{
	QFile batteryLine("/sys/class/power_supply/imx6-bat/voltage_now");
	if(batteryLine.exists()){
		batteryLine.open(QIODevice::ReadOnly | QIODevice::Text);
		m_bat_voltage.clear();
	  m_bat_voltage = batteryLine.readAll();
	  batteryLine.close();
	}
	qDebug() << "read bat vol";
	QFile powersupplyLine("/sys/class/power_supply/imx6-bat/uevent");
	if(powersupplyLine.exists()){
		powersupplyLine.open(QIODevice::ReadOnly | QIODevice::Text);
		m_batStr.clear();
		m_batStr = powersupplyLine.readAll();
		powersupplyLine.close();
	}
	qDebug() << "read charging status";

  /*No charging ; Charging*/
  bool charging_stat =!m_batStr.contains("charging", Qt::CaseSensitive); 
  if(m_bat_charging_status != charging_stat) {
		m_bat_charging_status = charging_stat;
		m_webView->update();
	  _timerJustReflesh.start(TIMER_RREFLESH_SHORT_DELAY);
  }   
}

void MainApp::readBatterLevel()
{
   
 	#ifdef SKH_DEMO_BATTERY
    QFile batteryLine("/run/battery");
    // system("cat /sys/class/power_supply/imx6-bat/uevent");
    if(batteryLine.exists()){
		  system("cat -n /sys/class/power_supply/imx6-bat/uevent >/run/battery");
    } else {
    	system("cat -n /sys/class/power_supply/imx6-bat/uevent >/run/battery");
    }
	  system("cat /run/battery");
	  batteryLine.open(QIODevice::ReadOnly | QIODevice::Text);
	  m_batStr.clear();
    m_batStr = batteryLine.readAll();
    batteryLine.close();
	  batteryLine.remove();
	#else
	  QFile batteryLine("/sys/class/power_supply/imx6-bat/uevent");
	  if(batteryLine.exists()){
		  batteryLine.open(QIODevice::ReadOnly | QIODevice::Text);
		  m_batStr.clear();
	    m_batStr = batteryLine.readAll();
	    batteryLine.close();
	  }
	  qDebug() << "read bat uevent";
	#endif
}


QString MainApp::getBatterLevel()
{
  return    m_batStr ; 
}

QString MainApp::MC_Get_DeviceInfo(void)
{
   return    m_ifconfigStr;
}

QString MainApp::MC_Get_FwVersion(void)
{
   return    m_fwVersionStr;
}
QString MainApp::MC_Get_UiAppVersion(void)
{
   return    m_uiAppVersionStr;
}

void MainApp::onTimeout()
{
  qDebug() << "on timeout then blinking";
  _timer.stop();
	
  #ifdef SKH_HK_DEMO
	  if(!m_screen_13n3_exit){
	    Full_reflesh();
	    return;
	  }
	#endif
	
  #ifdef SKH_HK_BUS_STOP
	  if(m_screen_13n3_exit){
	    Full_reflesh();
	    return;
	  }
	#endif
	
  //just cal blinking java api
	QString params1 = QString("blinkMainContent(%1)").arg(m_blinking_interval);
	qDebug()<<params1 <<endl;
  m_webView->page()->mainFrame()->evaluateJavaScript(params1);
	
  #ifdef SKH_HK_DEMO
	  _timerBlinking.start(m_blinking_interval<<1);
	#endif
}

void MainApp::MC_LoadRomoteTemplate(void)
{
	_demoTimer.stop();
	_timer.stop();
	_timerReflesh.stop();
	get_last_config();
	
	LoadDefaultTemplate();
	if(!m_screen_13n3_exit){
		triger_p2_resend();
	}
  // m_ComQmqttTsk->start();
}

void MainApp::onNetworkCheckTimeout()
{
	_timerNetworkCheck.stop();
  if(MC_Check_Host_Network(0) == true){
   	qDebug()<< "network is good 1\r\n";
		_demoTimer.stop();
		_timer.stop();
		//_timerNetworkCheck.stop();
    LoadDefaultTemplate();
	  //m_ComHttpsTsk->start();
	  regesiterDevice();
		m_commqtt_tsk_runing = true;
	  m_ComQmqttTsk->start();
  } else {
    regesiterDevice();
  } 
}

void MainApp::onWatchDogTimeout()
{
  /*feed watchdog*/
  QString cmdline ="echo 1 >/dev/watchdog";
	//qDebug() << "feed watchdog\n";
  // qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
  system(cmdline.toLocal8Bit().data());
}

void MainApp::onTimeClrCacheout()
{   
  QString cmdline ="echo 3 > /proc/sys/vm/drop_caches";
	qDebug() << "clear cache\n";
  qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
  system(cmdline.toLocal8Bit().data());
}

void MainApp::blinkingTimeout()
{
  qDebug() << "blinking Timeout";
	QString cmdline;
	/*play sound*/
  /*only display one time sound*/
	if(m_dingdong_sound_count>0) {	
   	cmdline = "aplay -x "+ m_speaker_vol+" /etc/189.wav &";
  	qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
		system(cmdline.toLocal8Bit().data());
		m_dingdong_sound_count =0;
	}
   
	if(m_sound_count == 0){
		QString params1 = "blinkMainContent("+ QString::number(m_blinking_interval)+","+ QString::number(m_max_count)+")";
	  qDebug() << params1;
		m_webView->page()->mainFrame()->evaluateJavaScript(params1);						
  } else if((m_sound_count%2) != 0){
	  // system(cmdline.toLocal8Bit().data());
  }

	m_sound_count++;

	if(m_sound_count >= m_max_count*2) {
		_timerBlinking.stop();
		m_sound_count = 0;
		m_webView->update();
		if(!m_mqtt_queue.isEmpty()){
			_queueNumTimer.start(QUEUE_NUMBER_DELAY);
			//MC_Mqtt_CMD_Handler(m_mqtt_queue.dequeue());
		} else {
			m_showing_number = false;
		}
	}
}

void MainApp::showFullLoadImage()
{
  _timerFullLoading.stop();
  _timerFullLoading.start(FULL_LOADING_DELAY);
  qDebug()<< "_timerFullLoading 500ms extend loading time";
  return;
}


void MainApp::UpdateDisplayStopTimer()
{
  _timer.stop();
  qDebug() <<"stop timer and refresh screen";
  m_webView->update();
}

void MainApp::reloadTemplate()
{
	qDebug() <<"reload tempate";
	m_webView->reload();
	Full_reflesh();
}

void MainApp::UpdateDisplayDemo()
{
  #ifdef SKH_DEMO_DED_INFO
    //refresh screen
    qDebug()<<"bed demo full refresh";
	  Full_reflesh();
    _timer.start(10000);
    return;
  #endif

  if(g_screen_width == EINK_SCREEN133_WIDTH){
 	  #ifdef SKH_HK_BUS_STOP
      if(m_exchange){
  	    m_webView->hide();
  	    m_webViewHiden->show();
	      m_exchange = 0;
	      qDebug() << "show hidden\n";
      } else {
        m_webViewHiden->hide();
  	    m_webView->show();
  	    m_exchange = 1;
	      qDebug() << "show webview\n";
      }
      _timer.start(700);
    #else
      QString docname = demo_docname.at(demo_docname_index);
      QString calling_num= QString::number(demo_call_number);

      QString params2 = QString("updateWindowDisplay('%1','A%2')").arg(docname).arg(calling_num);
	    qDebug() << params2;
	    m_webView->page()->mainFrame()->evaluateJavaScript(params2);

	    demo_docname_index= (++demo_docname_index)%6;
	    demo_call_number = (++demo_call_number)%10000;
      if((demo_call_number%DEMO_REFRESH_INTERVAL) == 0) {
	      //refresh screen
	      Full_reflesh();
      }
	    readBatterLevel();
	    m_webView->update();
	    _timer.start(TIMER_INTERVAL);
    #endif  // SKH_HK_BUS_STOP
 	} else {
    #ifdef SKH_HK_DEMO
      if(m_exchange){
  	    m_webView->hide();
  	    m_webViewHiden->show();
	      m_exchange = 0;
	      qDebug() << "show hidden\n";
      } else {
        m_webViewHiden->hide();
  	    m_webView->show();
  	    m_exchange = 1;
	      qDebug() << "show webview\n";
      }
      _timer.start(700);
    #else
      #ifdef EINK_FACTORY_AGING_TESTING 
        QString docname = demo_docname.at(demo_docname_index);
        QString calling_num= QString::number(demo_call_number);
        QString params2 = QString("updateWindowDisplay('%1','A%2')").arg(docname).arg(calling_num);
	      qDebug() << params2;
	      m_webView->page()->mainFrame()->evaluateJavaScript(params2);
	      demo_docname_index= (++demo_docname_index)%6;
	      demo_call_number = (++demo_call_number)%10000;

	      if((demo_call_number%DEMO_REFRESH_INTERVAL) == 0) {
		      //refresh screen
		      Full_reflesh();
	      }

	      readBatterLevel();
	      m_webView->update();
	      _timer.start(TIMER_INTERVAL);
      #else
        readBatterLevel();
        m_webView->update();
        Full_reflesh();
      #endif  // EINK_FACTORY_AGING_TESTING
    #endif  // SKH_HK_DEMO
	   
	}
}

void MainApp::UpdateDisplay()
{
  return;
}

void MainApp::handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
  qDebug() << "handleSslErrors: ";
  foreach (QSslError e, errors)
  {
    qDebug() << "ssl error: " << e;
  }
  reply->ignoreSslErrors();
}

void MainApp::mqttReplyFinished(QNetworkReply * reply)
{
	QJsonDocument jsdoc;
	QString strCmd;
  // QString test = "replayfinished";

	if(reply->error() == QNetworkReply::NoError) {
    qDebug()<<"mqttReplyFinished:success at reply"<<endl;            
	} else {
		 qDebug()<<"mqttReplyFinished: Error at Reply"<<endl;
	}

	reply->deleteLater();
}


void MainApp::replyFinished(QNetworkReply * reply)
{
	QJsonDocument jsdoc;
	QString strCmd;
  // QString test = "replayfinished";
	
	_timerHttpReply.stop();

  qDebug() << "replyFinished " ;
	//qDebug() << "reply data:"<<data;
	
	if(reply->error() == QNetworkReply::NoError) {
		QByteArray data = reply->readAll();
		qDebug() << "reply data:"<<data;

    //if(!reply->bytesAvailable())return;
    jsdoc = QJsonDocument::fromJson(data);
		QJsonObject jsobj = jsdoc.object();
			
    if(m_device_register == false){
			m_device_register = true;
			if (jsobj.contains("content")){
				QJsonValue contentdata = jsobj.value("content");
				qDebug() << "reply status " ;
				if (contentdata.isObject()) {  // data is object
	        QJsonObject dataobj = contentdata.toObject();
	        if (dataobj.contains("device")) {
						QJsonValue devicedata = dataobj.value("device");
						if(devicedata.isObject()) {
							QJsonObject deviceobj = devicedata.toObject();
							if (deviceobj.contains("updated_at")){	 
				        QJsonValue updateVal = deviceobj.value("updated_at");
				        QString strDateTime = updateVal.toString();
								strDateTime.remove(strDateTime.size()-6,6);
								QString cmdline;
								cmdline = "date -s '"+ strDateTime+"'";
								qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
								system(cmdline.toLocal8Bit().data());
							}
						}   		  
	        }
        }
			} 
    }
			
		if(m_web_con ==false){
			m_web_con=true;
			m_webView->update();
			_timerJustReflesh.start(TIMER_RREFLESH_SHORT_DELAY);
			qDebug() << "remove no network " ;
			m_disconnect_count = 0;
		}
			
		//get command
		if (jsobj.contains("command")) {  // \B0\FC\BA\ACָ\B6\A8\B5\C4 key
	    QJsonValue value = jsobj.value("command");  // \BB\F1ȡָ\B6\A8 key \B6\D4Ӧ\B5\C4 value
	    if (value.isString()) {  // \C5ж\CF value \CAǷ\F1Ϊ\D7ַ\FB\B4\AE
	      strCmd = value.toString();  // \BD\AB value ת\BB\AFΪ\D7ַ\FB\B4\AE
	      qDebug() << "command : " << strCmd;
	    }
	  }

    if(QString::compare(strCmd,"update_config") == 0){
			cmd_update_config_handle(jsobj);	
		} else if(QString::compare(strCmd,"update_firmware") == 0) {
			//update firmware command received, decode url first.				
      if (jsobj.contains("content")) {
        QJsonValue contentdata = jsobj.value("content");
        if (contentdata.isObject()) {  // data is object
        	QJsonObject dataobj = contentdata.toObject();
          if (dataobj.contains("version")) {
            QJsonValue versionVal = dataobj.value("version");
            if (versionVal.isString()) {
              QString strVer = versionVal.toString();
              qDebug() << "version: " << strVer;
            }
          }
          if (dataobj.contains("url")) {
          	QJsonValue urlVal = dataobj.value("url");
            if (urlVal.isString()) {
              QString strDownload = urlVal.toString();
              qDebug() << "url: " << strDownload;
            }
          }
					if (dataobj.contains("checksum")) {
          	QJsonValue ChecksumVal = dataobj.value("checksum");
            if (ChecksumVal.isString()) {
              QString strChecksum = ChecksumVal.toString();
              qDebug() << "checksum: " << strChecksum;
            }
          }
        }
			}
    }
            
	} else {
		qDebug()<<"Error at Reply"<<endl;
		// QByteArray data = reply->readAll();
		// qDebug(data);
		// if(m_device_register == true){
		if(m_web_con==true){
	 		m_web_con=false;
			m_webView->update();
			_timerJustReflesh.start(TIMER_RREFLESH_SHORT_DELAY);
			m_disconnect_count++;
			qDebug() << "show no network with mins" <<QString::number(m_disconnect_count*5);
			int dischonnect_threshold= m_dischonnect_threshold.toInt();
			if(m_disconnect_count>=dischonnect_threshold){
				//if no response continually for 2 hours, just reboot.
				qDebug() << "disconnect too long, reboot!";
				system("reboot");
				m_disconnect_count = 0;
			}
		}
			// }
	}

	reply->deleteLater();

}


void MainApp::response_mqtt_ack( QString num)
{
	QNetworkRequest request;
	QUrlQuery querystr;
	QSslConfiguration sslconfig;
	sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
	sslconfig.setProtocol(QSsl::TlsV1_2);
	sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);

	querystr.addQueryItem("message_id",num );
	QString pingStrUrl;
	if(m_https_exsit){
		pingStrUrl = "https://"+m_http_host_name+"/api/v1/devices/receive_mqtt";
	}else {
		pingStrUrl = "http://"+m_http_host_name+"/api/v1/devices/receive_mqtt";
	}

	QUrl url(pingStrUrl);
	
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_mqttreply->setNetworkAccessible(QNetworkAccessManager::Accessible);
	m_mqttreply->put(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"response_mqtt_ack pingStrUrl="<<pingStrUrl<<endl;
		

}


void MainApp::response_q_num( QString num)
{
	QNetworkRequest request;
	QUrlQuery querystr;
	QSslConfiguration sslconfig;
	sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
	sslconfig.setProtocol(QSsl::TlsV1_2);
	sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);

	querystr.addQueryItem("message_id",num );
	QString pingStrUrl;
	if(m_https_exsit){
		pingStrUrl = "https://"+m_http_host_name+"/api/v1/devices/receive_message";
	}else {
		pingStrUrl = "http://"+m_http_host_name+"/api/v1/devices/receive_message";
	}

	QUrl url(pingStrUrl);
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_mqttreply->setNetworkAccessible(QNetworkAccessManager::Accessible);
	m_mqttreply->put(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"response_q_num pingStrUrl="<<pingStrUrl<<endl;
		

}

void MainApp::get_last_config()
{
	QNetworkRequest request;
		
  QSslConfiguration sslconfig;
	QString getConfigStrUrl;
  sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
  sslconfig.setProtocol(QSsl::TlsV1_2);
  sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);

	if(m_https_exsit){
		getConfigStrUrl = "https://"+m_http_host_name+"/api/v1/devices/latest_config?uuid="+m_mac_address;
	}else {
		getConfigStrUrl = "http://"+m_http_host_name+"/api/v1/devices/latest_config?uuid="+m_mac_address;
	}
	QUrl url(getConfigStrUrl);
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->get(request);
	qDebug()<<"getConfigStrUrl="<<getConfigStrUrl<<endl;

}

void MainApp::triger_p2_resend()
{
	QNetworkRequest request;
	QUrlQuery querystr;
	QSslConfiguration sslconfig;
	QString getConfigStrUrl;
	sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
  sslconfig.setProtocol(QSsl::TlsV1_2);
  sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);
 
	querystr.addQueryItem("uuid",m_mac_address );
	if(m_https_exsit){
		getConfigStrUrl = "https://"+m_http_host_name+"/api/v1/devices/p2_resend";
	}else {
		getConfigStrUrl = "http://"+m_http_host_name+"/api/v1/devices/p2_resend";
	}

	QUrl url(getConfigStrUrl);
		
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->post(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<"triger_p2_resend="<<getConfigStrUrl<<endl;

}

QByteArray MainApp::buildLogUploadString()
{
  QString path ="/usr/local/log.txt";
  QString filename = "log.txt";

  QString bound="margin";
  QByteArray data(QString("--" + bound + "\r\n").toUtf8());
  data.append("Content-Disposition: form-data; name=\"uuid\"\r\n\r\n");
  data.append(QString(m_mac_address+"\r\n").toUtf8());   
  data.append(QString("--" + bound + "\r\n").toUtf8());   
  data.append("Content-Disposition: form-data; name=\"logfile\"; filename=\"");
  data.append(QString(filename).toUtf8());
  data.append("\"\r\n");  
  data.append("Content-Type:  text/plain\r\n\r\n"); //data type

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)){
    qDebug() << "QFile Error: File not found!";
    return data;
  } else { 
    qDebug() << "File found, proceed as planned";
  }

  data.append(file.readAll());           
  data.append("\r\n");
  data.append("--" + bound + "--\r\n");  //closing boundary according to rfc 1867


  file.close();

  return data;
}

QByteArray MainApp::buildUploadString()
{
  QString path ="/usr/local/fb.jpg";
  QString filename = "fb.jpg";

  QString bound="margin";
  QByteArray data(QString("--" + bound + "\r\n").toUtf8());
  data.append("Content-Disposition: form-data; name=\"uuid\"\r\n\r\n");
  data.append(QString(m_mac_address+"\r\n").toUtf8());   
  data.append(QString("--" + bound + "\r\n").toUtf8());   
  data.append("Content-Disposition: form-data; name=\"screenshot\"; filename=\"");
  data.append(QString(filename).toUtf8());
  data.append("\"\r\n");  
  data.append("Content-Type:  image/jpeg\r\n\r\n"); //data type

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)){
    qDebug() << "QFile Error: File not found!";
    return data;
  } else { 
    qDebug() << "File found, proceed as planned"; 
  }

  data.append(file.readAll());           
  data.append("\r\n");
  data.append("--" + bound + "--\r\n");  //closing boundary according to rfc 1867


  file.close();

  return data;
}


void MainApp::upload_screen_shot()
{
  QNetworkRequest request;
	QString bound="margin"; //name of the boundary
	QByteArray postData;
	QString postScreenShotUrl;

	QSslConfiguration sslconfig;
	sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
	sslconfig.setProtocol(QSsl::TlsV1_2);
	sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);

  system("gsnap /usr/local/fb.jpg /dev/fb0");
  postData = buildUploadString();
	if(postData.size() < 1000){
	 	qDebug()<<"upload eixt, no image!";
		return;
	}

	if(m_https_exsit){
	  postScreenShotUrl = "https://"+m_http_host_name+"/api/v1/devices/screenshot_upload";
	}else {
		postScreenShotUrl = "http://"+m_http_host_name+"/api/v1/devices/screenshot_upload";
	}
	
	QUrl url(postScreenShotUrl);
	
	request.setUrl(url);
	
	request.setRawHeader(QString("Content-Type").toUtf8(),QString("multipart/form-data; boundary=" + bound).toUtf8());
	request.setRawHeader(QString("Content-Length").toUtf8(), QString::number(postData.length()).toUtf8());
	
	m_restclient->post(request,postData);
	
	qDebug()<<"upload_screen_shot="<<postScreenShotUrl<<endl;
	
}

void MainApp::upload_log_file()
{
  QNetworkRequest request;
  QString bound="margin"; //name of the boundary
  QByteArray postData;
  QString postScreenShotUrl;


  QSslConfiguration sslconfig;
  sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
  sslconfig.setProtocol(QSsl::TlsV1_2);
  sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(sslconfig);

  postData = buildLogUploadString();
  if(postData.size() < 1000){
  	qDebug()<<"upload eixt, no image!";
    return;
  }

  if(m_https_exsit){
    postScreenShotUrl = "https://"+m_http_host_name+"/api/v1/devices/logfile_upload";;
  }else {
   postScreenShotUrl = "http://"+m_http_host_name+"/api/v1/devices/logfile_upload";;
  }

  QUrl url(postScreenShotUrl);

  request.setUrl(url);

  request.setRawHeader(QString("Content-Type").toUtf8(),QString("multipart/form-data; boundary=" + bound).toUtf8());
  request.setRawHeader(QString("Content-Length").toUtf8(), QString::number(postData.length()).toUtf8());

  m_restclient->post(request,postData);

  qDebug()<<"upload_screen_shot="<<postScreenShotUrl<<endl;
	
}

void MainApp::MC_Device_Auto_Sleep()
{

	//QStringList timelimitList=m_auto_sleep_time.split(":");
	int sleep_interval= m_auto_sleep_interval.toInt();
	int wakeup_interval= m_auto_wakeup_interval.toInt();

	qDebug()<<"sleep interval="<<QString::number(sleep_interval);
	qDebug()<<"wakeup_interval="<<QString::number(wakeup_interval)<<endl;
	

	// time to shut off mode xpower sec
	//	waiting for wakeup
	int sleepmin=sleep_interval-wakeup_interval;
	qDebug()<<"sleepMinutes="<<QString::number(sleepmin);
	int secnum = sleepmin*60;//minutes*60= seconds
	QString cmdline;
	cmdline = "xpower "+ QString::number(secnum);
	qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
	system(cmdline.toLocal8Bit().data());

}

void MainApp::regesiterDevice()
{
	QNetworkRequest request;
	QUrlQuery querystr;
  QString runtime_mins,refresh_count;
	QTime currTime = QTime::currentTime();
	QString pingStrUrl;
	
	QSslConfiguration sslconfig;
	sslconfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
	sslconfig.setProtocol(QSsl::TlsV1_2);
	sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(sslconfig);

	runtime_mins = QString::number(m_runtime_mins);
	refresh_count = QString::number(m_refresh_count);

	qDebug()<<"ssl_regesiterDevice= started"<<endl;

	querystr.addQueryItem("uuid",m_mac_address );
	//qDebug()<<"uuid="<<m_mac_address<<endl;
	querystr.addQueryItem("ip",m_ip_address);
	//qDebug()<<"ip="<<m_ip_address<<endl;
	querystr.addQueryItem("version",app_version);
	//qDebug()<<"version="<<app_version<<endl;
	querystr.addQueryItem("battery_life",m_bat_voltage);
  //qDebug()<<"battery="<<m_bat_voltage<<endl;
	querystr.addQueryItem("runtime",runtime_mins);
	//qDebug()<<"runtime="<<runtime_mins<<endl;
	querystr.addQueryItem("refresh_count",refresh_count);
	//qDebug()<<"refresh_count="<<refresh_count<<endl;
	querystr.addQueryItem("cpu",m_cpu_percentage);
	//qDebug()<<"cpu="<<m_cpu_percentage<<endl;
	querystr.addQueryItem("memory",m_memory_percentage);
	//qDebug()<<"memory="<<m_memory_percentage<<endl;
	if(!m_ComQmqttTsk->isConnectedMain()){
		m_device_status = "unhealthy";
		querystr.addQueryItem("error_number","2");
		//qDebug()<<"error_number=2"<<endl;
	}else {
		m_device_status = "normal";
	}
	querystr.addQueryItem("status",m_device_status);
	//qDebug()<<"status="<<m_device_status<<endl;
	if(m_bat_charging_status) {
		querystr.addQueryItem("is_charging","1");
		//qDebug()<<"is_charging=1"<<endl;
	} else {
		querystr.addQueryItem("is_charging","0");
	  // qDebug()<<"is_charging=0"<<endl;
	}
	//querystr.addQueryItem("error_number",m_error_number);
	//qDebug()<<"error_number="<<m_error_number<<endl;
	if(m_https_exsit){
	  pingStrUrl = "https://"+m_http_host_name+"/api/v1/devices/ping";
	}else {
		pingStrUrl = "http://"+m_http_host_name+"/api/v1/devices/ping";
	}

	QUrl url(pingStrUrl);
	
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->setNetworkAccessible(QNetworkAccessManager::Accessible);
	m_reply=m_restclient->post(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	_timerHttpReply.start(HTTP_REPLY_TIMEOUT);
	//qDebug()<<"_timerHttpReply.start(60000)";
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"ssl_pingStrUrl="<<pingStrUrl<<endl;

}

void MainApp::cmd_update_config_handle(QJsonObject jsobj)
{
  if(jsobj.contains("content")) {
    QJsonValue contentdata = jsobj.value("content");
    if(contentdata.isObject()) {  // data is object
		  QJsonObject dataobj = contentdata.toObject();
			if (dataobj.contains("blinking_count")) {
			  QJsonValue urlVal = dataobj.value("blinking_count");
			  if (urlVal.isString()) {
			 	  bool ok;
			 	  QString tmpstr;
				  tmpstr = urlVal.toString();
				  m_max_count = tmpstr.toInt(&ok,10);
				  qDebug() << "blinking_count: " << QString::number(m_max_count);
			  }
		  }

			if (dataobj.contains("speaker_volume")) {
				QJsonValue urlVal = dataobj.value("speaker_volume");
				if (urlVal.isString()) {
				  m_speaker_vol = urlVal.toString();
					bool ok;
					int vol =m_speaker_vol.toInt(&ok,10);
					vol = 127*vol/100;
					m_speaker_vol = QString::number(vol);
					qDebug() << "speaker_volume: " << m_speaker_vol;
				}
			}
					 
			if (dataobj.contains("shutdown_timer")) {
				QJsonValue urlVal = dataobj.value("shutdown_timer");
				if (urlVal.isString()) {
				 	QString shutdown_timer;
					shutdown_timer = urlVal.toString();
					bool ok;
				  m_shutdown_timer_count=shutdown_timer.toInt(&ok,10);
					qDebug() << "shutdown_timer: " << shutdown_timer;
				}
      }

			if(dataobj.contains("shutdown_template")) {
				QJsonValue urlVal = dataobj.value("shutdown_template");
				if (urlVal.isString()) {		
					m_shutdown_133_template = urlVal.toString();
				}
			}

			if(dataobj.contains("default_template")) {
				QJsonValue urlVal_def = dataobj.value("default_template");
				if (urlVal_def.isString()) {		
					m_default_133_template = urlVal_def.toString();
				}
			}
	  }
  }
}

void MainApp::MC_Vertical_Display()
{
  QString qFHostName = "/proc/epdc/epdc_9n7";
	QString f97HoriHostName = "/usr/local/epdc_9n7_h";
	QString f133HoriHostName = "/usr/local/epdc_13n3_h";
	QString strCmd9n7Vertical = "cp -f /usr/local/v9n7.profile /home/root/.profile";
	QString strCmd9n7DefTemplate1 ="cp -rf /usr/local/template_97_v/ /usr/local/template_97/";
	QString strCmd9n7DefTemplate2 ="cp -rf /usr/local/template_97_1_v/ /usr/local/template_97_1/";
	QFile file(qFHostName);
	QFile f9n7hfile(f97HoriHostName);
	QFile f13n3hfile(f133HoriHostName);
	if(QFile::exists(qFHostName))
	{
	  if(QFile::exists(f97HoriHostName)) {
		  // delete 9.7 horizon file
		  f9n7hfile.remove();
		  system(strCmd9n7DefTemplate1.toLocal8Bit().data());
		  system(strCmd9n7DefTemplate2.toLocal8Bit().data());
		  system(strCmd9n7Vertical.toLocal8Bit().data());
	  }
	} else {
	  if(!QFile::exists(f133HoriHostName)) {
			// delete 133.3 horizon file
		  f13n3hfile.remove();
	  }
	}
}

void MainApp::MC_Horizon_Display()
{
	QString qFHostName = "/proc/epdc/epdc_9n7";
	QString f97HoriHostName = "/usr/local/epdc_9n7_h";
	QString f133HoriHostName = "/usr/local/epdc_13n3_h";
	QString strCmd9n7Horizon = "cp -f /usr/local/h9n7.profile /home/root/.profile";
	QString strCmd9n7DefTemplate1 ="cp -rf /usr/local/template_97_h/ /usr/local/template_97/";
  QString strCmd9n7DefTemplate2 ="cp -rf /usr/local/template_97_1_h/ /usr/local/template_97_1/";

	QFile file(qFHostName);
	QFile f9n7hfile(f97HoriHostName);
	QFile f13n3hfile(f133HoriHostName);
	if(QFile::exists(qFHostName))
	{
	  if(!QFile::exists(f97HoriHostName)) {
		  // create 9.7 horizon file
		  f9n7hfile.open(QIODevice::WriteOnly);
		  f9n7hfile.close();
		  system(strCmd9n7Horizon.toLocal8Bit().data());
		  system(strCmd9n7DefTemplate1.toLocal8Bit().data());
		  system(strCmd9n7DefTemplate2.toLocal8Bit().data());
	  }
	} else {
	  if(!QFile::exists(f133HoriHostName)) {
		  // create 133.3 horizon file
		  f13n3hfile.open(QIODevice::WriteOnly);
		  f13n3hfile.close();
	  }
	}
}

void MainApp::MC_Bed_Info_Reset(QString data)
{
	QString tmpUrl;

	if(m_https_exsit){
		tmpUrl= "https://"+m_http_host_name+"/call/default_97_template.html";
	}else {
		tmpUrl= "http://"+m_http_host_name+"/call/default_97_template.html";
	}

	qDebug() << "tmpUrl: " << tmpUrl;
	qDebug() << "m_template_url: " << m_template_url;
	if( QString::compare(m_template_url,tmpUrl) != 0) {
		m_template_url = tmpUrl;
		/*reload template*/
		m_webView->load(QUrl(m_template_url));
		//m_webView->show();
	  m_showfullscreen =1;
		_timerReflesh97.stop();
		_timerReflesh97.start(650);
	 }
}

void MainApp::MC_Update_Bed_Info(QString data)
{
	QJsonParseError error;
  QJsonDocument jsdoc = QJsonDocument::fromJson(data.toUtf8(),&error);
	QJsonObject jsobj = jsdoc.object();
	m_webView->update();

	if (jsobj.contains("content")) {
		QJsonValue contentdata = jsobj.value("content");
		if (contentdata.isObject()) {  // data is object
			QJsonObject dataobj = contentdata.toObject();
			
			if (dataobj.contains("url")) {
				QJsonValue urlVal = dataobj.value("url");
				if (urlVal.isString()) {
					QString tmpUrl= urlVal.toString();
					qDebug() << "tmpUrl: " << tmpUrl;
					qDebug() << "m_template_url: " << m_template_url;
					if( QString::compare(m_template_url,tmpUrl) != 0) {
						m_template_url = tmpUrl;
						/*reload template*/
						m_webView->load(QUrl(m_template_url));
						//m_webView->show();
						//Full_reflesh();
						m_template_changed = true;
					}else {
						m_template_changed = false;
					}
				}
			}

			if (dataobj.contains("data")) {
				QJsonValue templateData = dataobj.value("data");
				if(templateData.isObject()) {
					QJsonObject dataobj = templateData.toObject();
					QJsonDocument doc(dataobj);
					QString strJson(doc.toJson(QJsonDocument::Compact));
					qDebug() << "tempate data: " << strJson;
					m_updatewin_data_para = QString("updateWindowDisplay('%1')").arg(strJson);
					if(m_template_changed == false){
						m_webView->page()->mainFrame()->evaluateJavaScript(m_updatewin_data_para);
						_timerReflesh97.stop();
						_timerReflesh97.start(m_content_chang_refresh_interval);
					  m_showing_number = false;
						qDebug() << "show updated data";
					}else{
						qDebug() << "template changed";
					}
					Full_reflesh();
				}
			}
		}
  }
}

bool MainApp::MC_Mqtt_msg_ack(QString data)
{
  QJsonDocument jsdoc;
	QJsonParseError error;
	QString strMsgId;
	QString strCmd;

	jsdoc = QJsonDocument::fromJson(data.toUtf8(),&error);
	if (error.error == QJsonParseError::NoError) {
		QJsonObject jsobj = jsdoc.object();
		if (jsobj.contains("command")) {  // ����ָ���� key
			QJsonValue value = jsobj.value("command");	// ��ȡָ�� key ��Ӧ�� value
			if (value.isString()) {  // �ж� value �Ƿ�Ϊ�ַ���
				strCmd = value.toString();	// �� value ת��Ϊ�ַ���
				qDebug() << "command : " << strCmd;
				if(QString::compare(strCmd,"get_logfile") == 0) return true;
			}
		}
		 
		if (jsobj.contains("message_id")) { 
			QJsonValue value = jsobj.value("message_id");	
			strMsgId = value.toString();	
			qDebug() << "message_id : " << strMsgId;
			if(QString::compare(m_pre_msg_id,strMsgId) == 0) {
				/*same msg id ,discard*/
				m_pre_msg_id = strMsgId;
				return false;
			}else {
			    m_pre_msg_id = strMsgId;
				response_mqtt_ack(strMsgId);
				return true;
		    }
		}
		return false;

	}else {
     /*decode error*/
	 return false;
	}	
}

void MainApp::MC_Mqtt_CMD_Handler(QString data)
{
  QJsonDocument jsdoc;
	QString strCmd;
	QJsonParseError error;
	QString strDownload ;
	QString strOutPutFile;
	QString strChecksum ;
	QString strMsgId;
	QString strTimestamp;
	qDebug() << "commMC_Decode_MQTT_Cmd : " << data;
	
	m_mqtt_msg = data;
	//m_webView->update();
	//Full_reflesh();
	//return;
	jsdoc = QJsonDocument::fromJson(data.toUtf8(),&error);
	if (error.error == QJsonParseError::NoError) {
		QJsonObject jsobj = jsdoc.object();
		//get command
		if (jsobj.contains("command")) {  // ����ָ���� key
			QJsonValue value = jsobj.value("command");	// ��ȡָ�� key ��Ӧ�� value
			if (value.isString()) {  // �ж� value �Ƿ�Ϊ�ַ���
				strCmd = value.toString();	// �� value ת��Ϊ�ַ���
				qDebug() << "command : " << strCmd;
			}
		}
		if (jsobj.contains("message_id")) {  // ����ָ���� key
			QJsonValue value = jsobj.value("message_id");	// ��ȡָ�� key ��Ӧ�� value
			//if (value.isInt isString()) {  // �ж� value �Ƿ�Ϊ�ַ���
			strMsgId = value.toString();	// �� value ת��Ϊ�ַ���
			qDebug() << "message_id : " << strMsgId;
			response_q_num(strMsgId);
				//}
		}
		if (jsobj.contains("beep")) {
			QJsonValue urlVal = jsobj.value("beep");
		  if (urlVal.isString()) {
				m_speaker_beep = urlVal.toString();	
				qDebug() << "m_speaker_beep: " << m_speaker_beep;
			}
		}
		if (jsobj.contains("call_action")) {
			QJsonValue urlVal = jsobj.value("call_action");
			 if (urlVal.isString()) {
				m_call_action = urlVal.toString();			
				qDebug() << "m_call_action: " << m_call_action;
			}
		}
		if(QString::compare(strCmd,"turn_off") == 0) {
			qDebug() << "turn off show";
			m_mqtt_queue.clear();
			m_template_url = m_shutdown_template;
			qDebug()<<"shutdown template"<<m_shutdown_template;
		  // m_webView->load(QUrl(m_shutdown_template));
		  // m_webView->show();
		  MC_LoadInactiveTemplate();
			//_timerReflesh.start(TIMER_RREFLESH_DELAY);
			//m_template_changed = true;
			_timerReflesh.start(TIMER_RREFLESH_DELAY);
	    m_showing_number = false;
		} else if(QString::compare(strCmd,"reset_lifedata") == 0) {
			qDebug() << "reset life data;";
			MC_Reset_Life_Time_Data();
			system("reboot");
		} else if(QString::compare(strCmd,"update_prod") == 0) {
			MC_write_Host_Name(DEFAULT_PRODUCT_HTTP_HOST_NAME,DEFAULT_PRODUCT_MQTT_HOST_NAME,DEFAULT_MQTT_HOST_PORT,DEFAULT_PRODUCT_MQTT_BK_HOST_NAME);
			system("reboot");
		} else if(QString::compare(strCmd,"update_staging") == 0) {
			MC_write_Host_Name(DEFAULT_STAGING_HTTP_HOST_NAME,DEFAULT_STAGING_MQTT_HOST_NAME,DEFAULT_MQTT_HOST_PORT,"");
			system("reboot");
		} else if(QString::compare(strCmd,"update_config") == 0){
			cmd_update_config_handle(jsobj);
			m_showing_number = false;
		} else if(QString::compare(strCmd,"screenshot_upload") == 0){
			upload_screen_shot();
			m_showing_number = false;
		}else if(QString::compare(strCmd,"get_logfile") == 0){
			upload_log_file();
			m_showing_number = false;
		} else if(QString::compare(strCmd,"server_update") == 0) {
			if (jsobj.contains("content")) {
				QJsonValue contentdata = jsobj.value("content");
				if (contentdata.isObject()) {  // data is object
					QJsonObject dataobj = contentdata.toObject();
					QString strMqttServer,strWebserver,strMqttport,strMqttbkServer;
					if (dataobj.contains("mqttserver")) {
						QJsonValue versionVal = dataobj.value("mqttserver");
						if (versionVal.isString()) {
						  strMqttServer = versionVal.toString();
							qDebug() << "mqttserver: " << strMqttServer;
						}
					}
					if (dataobj.contains("webserver")) {
						QJsonValue urlVal = dataobj.value("webserver");
						 if (urlVal.isString()) {
							strWebserver = urlVal.toString();
							qDebug() << "webserver: " << strWebserver;
						}
					}

					if (dataobj.contains("mqttport")) {
						QJsonValue urlVal = dataobj.value("mqttport");
						 if (urlVal.isString()) {
							strMqttport = urlVal.toString();
							qDebug() << "webserver: " << strMqttport;
						}
					}

					if (dataobj.contains("mqttbkserver")) {
					  QJsonValue urlVal = dataobj.value("mqttbkserver");
						if (urlVal.isString()) {
							strMqttbkServer = urlVal.toString();
							qDebug() << "mqttbkserver: " << strMqttbkServer;
						}
					}else strMqttbkServer="";
					  MC_write_Host_Name(strWebserver,strMqttServer,strMqttport,strMqttbkServer);
					  system("reboot");
				}
			}//content
		} else if(QString::compare(strCmd,"dnsserver_update") == 0) {		
			if (jsobj.contains("content")) {
				QJsonValue contentdata = jsobj.value("content");
				if (contentdata.isObject()) {  // data is object
					QJsonObject dataobj = contentdata.toObject();
					QString strDNS1,strDNS2,strDNS3,strDNS4,strDNS5;
					  if (dataobj.contains("dnsserver1")) {
						  QJsonValue versionVal = dataobj.value("dnsserver1");
						  if (versionVal.isString()) {
							  strDNS1 = versionVal.toString();
							  qDebug() << "dnsserver1: " << strDNS1;
						  }
					  }
					if (dataobj.contains("dnsserver2")) {
						QJsonValue urlVal = dataobj.value("dnsserver2");
						if (urlVal.isString()) {
							strDNS2 = urlVal.toString();
							qDebug() << "dnsserver2: " << strDNS2;
						}
					}
					if (dataobj.contains("dnsserver3")) {
						QJsonValue urlVal = dataobj.value("dnsserver3");
						if (urlVal.isString()) {
							strDNS3 = urlVal.toString();
							qDebug() << "dnsserver3: " << strDNS3;
						}
					}
					if (dataobj.contains("dnsserver4")) {
						QJsonValue urlVal = dataobj.value("dnsserver4");
						if (urlVal.isString()) {
							strDNS4 = urlVal.toString();
							qDebug() << "dnsserver4: " << strDNS4;
						}
					}
					if (dataobj.contains("dnsserver5")) {
						QJsonValue urlVal = dataobj.value("dnsserver5");
						if (urlVal.isString()) {
							strDNS5 = urlVal.toString();
							qDebug() << "dnsserver5: " << strDNS5;
						}
					}
					MC_write_DNS_IP(strDNS1,strDNS2,strDNS3,strDNS4,strDNS5);
					system("reboot");
			  }
		  } // content
    } else if(QString::compare(strCmd,"update_firmware") == 0) {
			//update firmware command received, decode url first.
			if (jsobj.contains("content")) {
				QJsonValue contentdata = jsobj.value("content");
				if (contentdata.isObject()) {  // data is object
					QJsonObject dataobj = contentdata.toObject();
					if (dataobj.contains("version")) {
						QJsonValue versionVal = dataobj.value("version");
						if (versionVal.isString()) {
							QString strVer = versionVal.toString();
							qDebug() << "version: " << strVer;
						}
					}
					if (dataobj.contains("url")) {
						QJsonValue urlVal = dataobj.value("url");
						if (urlVal.isString()) {
							strDownload = urlVal.toString();
							qDebug() << "url: " << strDownload;
						}
					}
					if (dataobj.contains("checksum")) {
						QJsonValue ChecksumVal = dataobj.value("checksum");
						if (ChecksumVal.isString()) {
							 strChecksum = ChecksumVal.toString();
							qDebug() << "checksum: " << strChecksum;
						}
					}

					//,checkSumMD5;

          #if 0
					  /*down load zip file*/
					  QString cmdline,cmdecho;
					 
					  strOutPutFile = "/run/skh_upgrade.zip";
					  QFile outfile(strOutPutFile);
					  if(outfile.exists())outfile.remove();
					 
					  cmdline = "wget -T 180 "+strDownload+" -O "+strOutPutFile;
					  qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
					  system(cmdline.toLocal8Bit().data());
					  cmdecho = "echo $? > /usr/local/downloadresult";
					  qDebug() <<"echo cmd= "<< cmdecho.toLocal8Bit().data();
					  system(cmdecho.toLocal8Bit().data());
					  QString qEchoFileName = "/usr/local/downloadresult";
					  QFile echfile(qEchoFileName);
					  echfile.open(QIODevice::ReadWrite | QIODevice::Text);
					  QString strResult =  echfile.readLine();
					  strResult = strResult.simplified();
					  qDebug() << "strResult="<<strResult;
					  echfile.close();
					  echfile.remove();
					  if( QString::compare(strResult,"0") == 0) {
						  /*unzip file*/
						  qDebug() << "download successful\r\n";
						  system("unzip -o /run/skh_upgrade.zip -d /run");
						  cmdecho = "echo $? > /usr/local/unzipresult";
						  qDebug() <<"echo unzipcmd= "<< cmdecho.toLocal8Bit().data();
					 	  system(cmdecho.toLocal8Bit().data());
					 	  QString qEchoUnzipFileName = "/usr/local/unzipresult";
						  QFile echfile(qEchoUnzipFileName);
						  echfile.open(QIODevice::ReadWrite | QIODevice::Text);
						  QString strResult =  echfile.readLine();
						  strResult = strResult.simplified();
						  qDebug() << "unzip strResult="<<strResult;
						  echfile.close();
						  echfile.remove();
						  if( QString::compare(strResult,"0") == 0) {
							  system("cd /run/skh_upgrade");
							  system("chmod 777 /run/skh_upgrade/ota_updater.sh");
							  system("/run/skh_upgrade/ota_updater.sh");
						  } else {
							  qDebug() << "unzip failed\r\n";
						  }
					
					  } else {
						  qDebug() << "download failed\r\n";
					  }
					  //system("reboot");
			    #endif
			    QString cmdline;
		      strOutPutFile = "/opt/skh_upgrade.zip";
			    system("rm /opt/skh_upgrade.zip");
		      cmdline = "wget -T 180 "+strDownload+" -O "+strOutPutFile;
		      qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
		      if (system(cmdline.toLocal8Bit().data()) == 0) {
					  if (system("unzip -o /opt/skh_upgrade.zip -d /run") == 0) {
						  system("chmod 777 /opt/skh_upgrade/ota_updater.sh");
						  system("/opt/skh_upgrade/ota_updater.sh");
					  } else
					    qDebug() << "unzip failed\r\n";
		      } else {
					  qDebug() << "download failed\r\n";
		      }
			    m_showing_number = false;
			  }
		  }
	  } else if(QString::compare(strCmd,"update_q_number") == 0) {
		  m_webView->update();
		  _shutdownTimer.stop();
		  _shutdownTimer.start(m_shutdown_timer_count*60000);
		  //update firmware command received, decode url first.
		  if (jsobj.contains("content")) {
			  QJsonValue contentdata = jsobj.value("content");
			  if (contentdata.isObject()) {  // data is object
			    QJsonObject dataobj = contentdata.toObject();
			
				  if (dataobj.contains("url")) {
					  QJsonValue urlVal = dataobj.value("url");
					  if (urlVal.isString()) {
							QString tmpUrl= urlVal.toString();
							qDebug() << "tmpUrl: " << tmpUrl;
							qDebug() << "m_template_url: " << m_template_url;
							if( QString::compare(m_template_url,tmpUrl) != 0) {
								m_template_url = tmpUrl;
								/*reload template*/
								m_webView->load(QUrl(m_template_url));
								m_webView->show();
								Full_reflesh();
								m_template_changed = true;
							}else {
								m_template_changed = false;
							}
					  }
				  }

				  if (dataobj.contains("data")) {
					  QJsonValue templateData = dataobj.value("data");
					  if(templateData.isObject()) {
							QJsonObject dataobj = templateData.toObject();
							QJsonDocument doc(dataobj);
							QString strJson(doc.toJson(QJsonDocument::Compact));
							qDebug() << "tempate data: " << strJson;
							m_updatewin_data_para = QString("updateWindowDisplay('%1')").arg(strJson);
							if(m_template_changed == false){
							  if(m_max_count > 0){
								  if(QString::compare(m_call_action,"1") == 0) {  
									  m_webView->page()->mainFrame()->evaluateJavaScript(m_updatewin_data_para);
									  //QThread::msleep(200);

								    //_timerBlinking.stop();
									  //_timerBlinking.start(m_blinking_interval);
									  //m_dingdong_sound_count = 1;
									  m_current_blank_number=false;
									  m_queue_number_time = strMsgId;
									  _timerReflesh133.stop();
									  _timerReflesh133.start(m_content_chang_refresh_interval);
								  } else {
									  _timerBlinking.stop();
									  if(m_current_blank_number ==false) {
										
										  if(strMsgId.toInt() < m_queue_number_time.toInt()){
											  /*if remove timestamp is previous 
											  than current queue numuber, then ignore*/
											  qDebug()<<"ignore remove";
											
										  } else {
											  /*number to blank , then need to reflesh*/
											  m_webView->page()->mainFrame()->evaluateJavaScript(m_updatewin_data_para);
											  //QThread::msleep(500);
											  //Full_reflesh();
										    _timerReflesh.stop();
										    _timerReflesh.start(500);
										    m_current_blank_number = true;
										    qDebug()<<"remove done";
								      }
									
								    } else {
								      /*if already blank number do nothing*/
									    qDebug()<<"already blank,do nothing";
							      }
								    m_showing_number = false;
								    return;
							
							    }
						    } else {
							    m_showing_number = false;
							
							    _timerBlinking.stop();
					      }
					    } else{
					 	    qDebug() << "template changed";
						    m_current_blank_number=false;
						    m_queue_number_time = strMsgId;
				      }
					    //Full_reflesh();
				    }
			    }  // dataobj.contains("data")
		    }  // contentdata.isObject()
	    }  // jsobj.contains("content")
    } else if(QString::compare(strCmd,"update_bed_info") == 0) {
      MC_Update_Bed_Info(data);
    } else if(QString::compare(strCmd,"bed_info_reset") == 0) {
      MC_Bed_Info_Reset(data);
      m_showing_number = false;
    } else if(QString::compare(strCmd,"conf_9n7_v") == 0) {
      // 9.7 vertical
      MC_Vertical_Display();
      system("reboot");
    } else if(QString::compare(strCmd,"conf_9n7_h") == 0) {
      // 9.7 horizon
      MC_Horizon_Display();
      system("reboot");
    } else if(QString::compare(strCmd,"conf_13n3_v") == 0) {
      //13.3 vetical
      MC_Vertical_Display();
      system("reboot");
    } else if(QString::compare(strCmd,"conf_13n3_h") == 0) {
      //13.3 horizon
      MC_Horizon_Display();
      system("reboot");
    }
	} else {  // QString::compare(strCmd,"turn_off") 
		qDebug() << "cmdDecode error, json format error.";
	}
}

void MainApp::MC_Decode_MQTT_Cmd(QString mqttdata)
{
	QString data = mqttdata;
  qDebug()<< "mqtt cmd received";

  /*mqtt ack*/
	if(false == MC_Mqtt_msg_ack(mqttdata)){
		/*discard msg id if same msg id*/
	  return;
	}
			
	if(m_mqtt_queue.isEmpty()){
		qDebug()<<"empty ,just show";

		if(m_showing_number==true){
			m_mqtt_queue.enqueue(mqttdata);
			/*display number on going, put current number in the queue*/
			qDebug()<< "show number put into queue;";		
		} else {
			m_showing_number = true;
			MC_Mqtt_CMD_Handler(data);
		}

	} else {

	  if(m_mqtt_queue.size()>= MAX_MQTT_QUEUE)m_mqtt_queue.dequeue();

	  if(m_showing_number==true){
			m_mqtt_queue.enqueue(mqttdata);
			/*display number on going, put current number in the queue*/
			qDebug()<< "show number put into queue;";
	  } else {
		  m_showing_number = true;
		  data = m_mqtt_queue.dequeue();
		  MC_Mqtt_CMD_Handler(data);
	  }
	}
}

void MainApp::MC_LoadLocalTemplate(void)
{
	QString default_template;
	/*LOCAL_TEMPLATE_MODE*/
	m_template_mode = LOCAL_TEMPLATE_MODE;
	
  #ifdef SKH_PRODUCTION
  	if(m_screen_13n3_exit){
  		default_template= QString("/usr/local/template_loading/loading_template.html") ;
  	}else {
  		default_template= QString("/usr/local/template_loading/loading_97_template.html") ;
  	}
  #else
  	if(m_screen_13n3_exit){
  		default_template= QString("/usr/local/template_133/")+ DEFAULT_TEMPLATE_NAME ;
  	}else {
  		default_template= QString("/usr/local/template_97/")+ DEFAULT_TEMPLATE_NAME ;
  	}
  #endif

	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_showfullscreen = 1;
  m_webView->load(baseUrl);

  #ifdef SKH_HK_DEMO
    QString default_back_template;
	
  	if(!m_screen_13n3_exit){
  		default_back_template= QString("/usr/local/template_97_1/")+ DEFAULT_TEMPLATE_NAME ;
  		
  		QUrl baseUrlback = QUrl::fromLocalFile(default_back_template.trimmed());
  		//qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
  		m_webViewHiden->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  	  m_webViewHiden->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  	  m_webViewHiden->load(baseUrlback);
  		m_webViewHiden->resize(g_screen_width, g_screen_height);
  		qDebug() << "LoadDefaulthiddenTemplate==>" << baseUrlback << endl;
  		MC_Picture_Mode();
  	} else {
  		MC_Speed_Mode();
  	}
  #else
  	MC_Speed_Mode();
  #endif

  #ifdef SKH_HK_BUS_STOP
  	if(m_screen_13n3_exit){
  	  default_back_template= QString("/usr/local/template_133_1/")+ DEFAULT_TEMPLATE_NAME ;
  		QUrl baseUrlback = QUrl::fromLocalFile(default_back_template.trimmed());
  		//qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
  		m_webViewHiden->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  		m_webViewHiden->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  		m_webViewHiden->load(baseUrlback);
  		m_webViewHiden->resize(g_screen_width, g_screen_height);
  		qDebug() << "hiden LoadDefaulthiddenTemplate==>" << baseUrlback << endl;
  	}
  #endif

	m_webView->resize(g_screen_width, g_screen_height);
  //QThread::msleep(1500);

	if(!m_screen_13n3_exit){
		_timerReflesh.stop();
		_timerReflesh.start(LOADING_REFRESH_INTERVAL);
		_demoTimer.start(DEMO_DISPLAY_INTERVLAL);
	}else{
		qDebug() << "show screen "<<endl;
		m_webView->showFullScreen();
		_timerReflesh.stop();
		_timerReflesh.start(TIMER_RREFLESH_DELAY);
		_demoTimer.start(DEMO_DISPLAY_INTERVLAL);
	}
	readBatterLevel();

}

void MainApp::UpdateMeetingRoomDisplay(const QString & data)
{
	qDebug() << "UpdateMeetingRoomDisplay : " << data;
	
	if(QString::compare(data,"update") == 0) {
		Full_reflesh();
	}else{
	  MC_LoadRemoteMeetingRoomTemplate(data);
	}

}

void MainApp::MC_LoadRemoteMeetingRoomTemplate(QString url)
{
	QString default_template = url;

	qDebug() << "meeting room url==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(QUrl(default_template));
	m_template_url = default_template;
	m_webView->resize(g_screen_width, g_screen_height);
	m_webView->showFullScreen();
	m_template_mode =REMOTE_TEMPLATE_MODE;
	_timerReflesh.start(TIMER_RREFLESH_DELAY);
	readBatterLevel();
}


void MainApp::MC_LoadDefaultMeetingRoomTemplate(void)
{
	
	QString default_template;
	m_template_mode = LOCAL_TEMPLATE_MODE;
	if(m_screen_13n3_exit){
		default_template= QString("/usr/local/template_133/")+ DEFAULT_TEMPLATE_NAME ;
	}else {
		default_template= QString("/usr/local/template_97/")+ DEFAULT_TEMPLATE_NAME ;
	}
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "meeting room display==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);
	MC_Speed_Mode();
	
	m_webView->resize(g_screen_width, g_screen_height);
	   
	m_webView->showFullScreen();
	_timerReflesh.start(TIMER_RREFLESH_DELAY);
	readBatterLevel();

}


void MainApp::MC_LoadWhiteTemplate(void)
{
	QString default_template ;
	m_template_mode = REMOTE_TEMPLATE_MODE;
	if(m_screen_13n3_exit){
		default_template = QString("/usr/local/template_black_white/133_white.html") ;
	} else {
		default_template = QString("/usr/local/template_black_white/97_white.html") ;
	}
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);

	m_webView->resize(g_screen_width, g_screen_height);
	
	m_webView->showFullScreen();
	_demoTimer.start(SHUT_OFF_REFRESH_DELAY);
	//_demoTimer.start(DEMO_DISPLAY_INTERVLAL);
	//Full_reflesh();	
	
}


void MainApp::MC_LoadBlackTemplate(void)
{
	QString default_template ;
	m_template_mode = REMOTE_TEMPLATE_MODE;
	if(m_screen_13n3_exit){
		default_template = QString("/usr/local/template_black_white/133_black.html") ;
	} else {
		default_template = QString("/usr/local/template_black_white/97_black.html") ;
	}
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);

	m_webView->resize(g_screen_width, g_screen_height);
	
	m_webView->showFullScreen();
	_demoTimer.start(SHUT_OFF_REFRESH_DELAY);
	//_demoTimer.start(DEMO_DISPLAY_INTERVLAL);
	//Full_reflesh();	
	
}

void MainApp::MC_LoadInactiveTemplate(void)
{
	QString default_template ;
	m_template_mode = REMOTE_TEMPLATE_MODE;
	if(m_screen_13n3_exit){
		#if 0
	    if(m_shutdown_133_template.size()<10){
			default_template = QString("/usr/local/template_inactive_off/133_inactive_off.html") ;
		
	    }else {
	    	default_template = m_shutdown_133_template;
	    }
		#endif
		default_template = QString("/usr/local/template_inactive_off/133_inactive_off.html") ;
	} else {
		default_template = QString("/usr/local/template_inactive_off/97_inactive_off.html") ;
	}
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "MC_LoadInactiveTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);

	m_webView->resize(g_screen_width, g_screen_height);
	
	m_webView->showFullScreen();
	
	
}



void MainApp::MC_LoadShutOffTemplate(void)
{
	QString default_template ;
	m_template_mode = REMOTE_TEMPLATE_MODE;
	if(m_screen_13n3_exit){
		default_template = QString("/usr/local/template_power_off/133_shut_off.html") ;
	} else {
		default_template = QString("/usr/local/template_power_off/97_shut_off.html") ;
	}
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);

	m_webView->resize(g_screen_width, g_screen_height);
	
	m_webView->showFullScreen();
	_demoTimer.start(SHUT_OFF_REFRESH_DELAY);
	//_demoTimer.start(DEMO_DISPLAY_INTERVLAL);
	//Full_reflesh();	
	
}

void MainApp::MC_Read_Device_Config(void)
{
	/*get device mac address*/
	QString strConfigData;
	QJsonParseError error;
	QJsonDocument jsdoc;
	QFile mConfigFile("/usr/local/dev_config");

	if(mConfigFile.exists()){
		mConfigFile.open(QIODevice::ReadOnly | QIODevice::Text);
		strConfigData.clear();
		strConfigData= mConfigFile.readAll();
		mConfigFile.close();
	} else {
    m_template_change_refresh_interval = 5000;//ms
    m_content_chang_refresh_interval = 700;//ms
    return;
	}

	jsdoc = QJsonDocument::fromJson(strConfigData.toUtf8(),&error);

	if (error.error == QJsonParseError::NoError) {
		QJsonObject jsobj = jsdoc.object();
		//get command
		if (jsobj.contains("chg_t_interval")) {  // ����ָ���� key
			QJsonValue value = jsobj.value("chg_t_interval");	// ��ȡָ�� key ��Ӧ�� value
			if (value.isString()) {  // �ж� value �Ƿ�Ϊ�ַ���
				m_template_change_refresh_interval = value.toString().toInt();	// �� value ת��Ϊ�ַ���
				qDebug() << "chg_t_interval : " << m_template_change_refresh_interval;
			}
		}
		if (jsobj.contains("chg_c_interval")) {  // ����ָ���� key
			QJsonValue value = jsobj.value("chg_c_interval");	// ��ȡָ�� key ��Ӧ�� value
			if (value.isString()) {  // �ж� value �Ƿ�Ϊ�ַ���
				m_content_chang_refresh_interval = value.toString().toInt();	// �� value ת��Ϊ�ַ���
				qDebug() << "chg_c_interval : " << m_content_chang_refresh_interval;
			}
		}
	} else {
	  	m_template_change_refresh_interval = 5000;//ms
	    m_content_chang_refresh_interval = 700;//ms
	}

}


void MainApp::MC_Read_DeviceInfo(void)
{
	/*get device mac address*/
	QFile mIfconfigInfoLine("/run/ifconfig_info");
	if(mIfconfigInfoLine.exists()){
		mIfconfigInfoLine.remove();
	}

	system("ifconfig > /run/ifconfig_info");
	mIfconfigInfoLine.open(QIODevice::ReadOnly | QIODevice::Text);
	m_ifconfigStr.clear();
	m_ifconfigStr= mIfconfigInfoLine.readAll();
	//m_ifconfigStr = m_ifconfigStr.simplified();
	mIfconfigInfoLine.close();
}

void MainApp::MC_Read_VersionNumber(void)
{
	/*get device mac address*/
	QFile mVersionFile("/etc/fw_version");
	if(mVersionFile.exists()){
		mVersionFile.open(QIODevice::ReadOnly | QIODevice::Text);
		m_fwVersionStr.clear();
		m_fwVersionStr= mVersionFile.readAll();
		mVersionFile.close();
	}
}

void MainApp::MC_Read_UiAppVersionNumber(void)
{
	/*get device mac address*/
	QFile mVersionFile("/usr/local/uiapp_version");
	if(mVersionFile.exists()){
		mVersionFile.open(QIODevice::ReadOnly | QIODevice::Text);
		m_uiAppVersionStr.clear();
		m_uiAppVersionStr= mVersionFile.readAll();
		mVersionFile.close();
	} else {
	    m_uiAppVersionStr = QString(app_version);
	}
}

void MainApp::MC_LoadDeviceInfo(void)
{
	QString default_template ;
	m_template_mode = LOCAL_TEMPLATE_MODE;
	default_template = QString("/usr/local/template_device_info/device_info.html") ;
	QUrl baseUrl = QUrl::fromLocalFile(default_template.trimmed());
	qDebug() << "LoadDefaultTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(baseUrl);

	m_webView->resize(g_screen_width, g_screen_height);
	
	m_webView->show();
	_demoTimer.start(DEVICE_INFO_REFRESH_DELAY);
	
}

void MainApp::LoadDefaultTemplate(void)
{
	QString default_template;
	
	if(m_screen_13n3_exit){
			
		if(m_https_exsit)
		{
			default_template= "https://"+m_http_host_name+"/call/default_template.html";
		}
		else
		{
			default_template= "http://"+m_http_host_name+"/call/default_template.html";
		}
	}else {
		if(m_https_exsit)
		{
			default_template= "https://"+m_http_host_name+"/call/default_97_template.html";
		}
		else
		{
			default_template= "http://"+m_http_host_name+"/call/default_97_template.html";
		}
			//no sound for 9.7 bed info
			m_max_count = 0;
	}

	qDebug() << "LoadRemoteTemplate==>" << default_template << endl;
	m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  m_webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	m_webView->load(QUrl(default_template));
	m_template_url = default_template;
	m_webView->resize(g_screen_width, g_screen_height);
  MC_Speed_Mode();

	if(!m_screen_13n3_exit){
		m_showfullscreen = 1;
		_timerReflesh.stop();
	    _timerReflesh.start(FIRST_LOADING_REFRESH_INTERVAL);
	}else{
		//m_webView->showFullScreen();
		m_showfullscreen = 1;
		_timerReflesh.stop();
	    _timerReflesh.start(TIMER_RREFLESH_DELAY);
	}
	
	m_template_mode =REMOTE_TEMPLATE_MODE;
	
	_demoTimer.stop();
	
}

void MainApp::MC_Get_Device_Display_Size()
{
  /*get device type*/
  QString qFHostName9n7 = "/proc/epdc/epdc_9n7";
  QString f97HoriHostName = "/proc/epdc/epdc_9n7_h";
  QString f133HoriHostName = "/proc/epdc/epdc_13n3_h";
  QString qFHostName13n3LowResolution = "/proc/epdc/lcd_1600x1200";
  QString qFHostName13n3 = "/proc/epdc/epdc_13n3";

  if(QFile::exists(qFHostName9n7)) {
    g_screen_width = EINK_SCREEN97_WIDTH;
    g_screen_height =EINK_SCREEN97_HEIGHT;
    m_screen_13n3_exit=false;
    m_device_type = DEVICE_9N7;
  }
  else if(QFile::exists(qFHostName13n3)){
    g_screen_width = EINK_SCREEN133_WIDTH;
    g_screen_height = EINK_SCREEN133_HEIGHT;
    m_screen_13n3_exit= true;
    m_device_type = DEVICE_13N3;
  }
  else if(QFile::exists(f133HoriHostName)){
    g_screen_width = EINK_SCREEN133_WIDTH_H;
    g_screen_height =EINK_SCREEN133_HEIGHT_H;
    m_screen_13n3_exit= true;
    m_device_type = DEVICE_13N3_H;
  }
  else if(QFile::exists(qFHostName13n3LowResolution)){
    g_screen_width = EINK_SCREEN133_WIDTH_1200;
    g_screen_height =EINK_SCREEN133_HEIGHT_1600;
    m_screen_13n3_exit=false;
    m_device_type = DEVICE_13N3_LR;
  }
  else if(QFile::exists(f97HoriHostName)) {
    g_screen_width = EINK_SCREEN97_WIDTH_H;
    g_screen_height =EINK_SCREEN97_HEIGHT_H ;
    m_screen_13n3_exit=false;
    m_device_type = DEVICE_9N7_H;
  } else {
    g_screen_width = EINK_SCREEN133_WIDTH;
    g_screen_height = EINK_SCREEN133_HEIGHT;
    m_screen_13n3_exit= true;
    m_device_type = DEVICE_13N3;
  }
}

int MainApp::MC_Get_Device_Display_Width(void)
{
	return  g_screen_width; 
}

int MainApp::MC_Get_Template_Mode(void)
{
 return m_template_mode;
}

#if 0
bool MainApp::MC_Check_Host_Network_Http(int backrun_flag)
{

    QNetworkRequest request;
	QUrlQuery querystr;

	QString pingStrUrl = "https://"+m_http_host_name+"/api/v1";
	QUrl url(pingStrUrl);
	
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_restclient->post(request,querystr.toString(QUrl::FullyEncoded).toUtf8());
	qDebug()<<querystr.toString(QUrl::FullyEncoded).toUtf8()<<endl;
	qDebug()<<"pingStrUrl="<<pingStrUrl<<endl;

}
#endif

bool MainApp::MC_Check_Host_Network(int backrun_flag)
{
  /**/
  
  bool ret =false;

  #if 0
    QString strPingFileName = "/run/ping";
    QFile pingfile(strPingFileName);
    if(pingfile.exists()){
   	  pingfile.remove();
   	}
    QString cmdline;
    if(backrun_flag == 0){
   		cmdline = "ping -w 2 "+ m_http_host_name +" -c 2 > /run/ping";
    }
    else {
   	  cmdline = "ping -w 2 "+ m_http_host_name +" -c 2 > /run/ping &";
    }
    qDebug() <<"cmdline= "<<cmdline.toLocal8Bit().data();
    system(cmdline.toLocal8Bit().data());
    //QString strPingFileName = "/run/ping";
    //QFile pingfile(strPingFileName);
    if(pingfile.exists()){
	    pingfile.open(QIODevice::ReadWrite | QIODevice::Text);
	    QString strResult =  pingfile.readAll();

	    strResult = strResult.simplified();
   	  if(strResult.contains("100% packet loss", Qt::CaseInsensitive))ret = false;
	    else ret = true;
	    pingfile.close();
	    pingfile.remove();
   	}
  #else
	  ret = m_device_register;
	#endif

  return ret;
}

QString MainApp::MC_Get_HTTP_Host_Name(void)
{
  return m_http_host_name;
}

QString MainApp::MC_Get_MQTT_Host_Name(void)
{
  return m_mqtt_host_name;
}

QString MainApp::MC_Get_MQTT_Backup_Host_Name(void)
{
  return m_mqtt_bk_host_name;
}

QString MainApp::MC_Get_MQTT_Host_Port(void)
{
 	return m_mqtt_host_port;
}

void MainApp::MC_refresh_count_update(void)
{
  m_refresh_count++;
}

void MainApp::MC_Write_Life_Time_Data(void)
{
	QString qFHostName = "/run/lifetimedata";
	QString strCmd = "cp -f /usr/local/lifetimedata /run/lifetimedata";
	QFile file(qFHostName);
	QFile lifeDataFile("/usr/local/lifetimedata");
	
	QString runtime_mins,refresh_count,reboot_count;
	runtime_mins = QString::number(m_runtime_mins);
	refresh_count = QString::number(m_refresh_count);
	reboot_count = QString::number(m_reboot_count);
	qDebug () << "MC_Write_Life_Time_Data run min:" <<runtime_mins<<"re cn:"<<refresh_count << endl;
	
  if((!file.exists())&&(lifeDataFile.exists())){ 
   	system(strCmd.toLocal8Bit().data());
  }
	file.open(QIODevice::ReadWrite |QIODevice::Text );
	file.write(runtime_mins.toLocal8Bit().data());
	file.write("\r\n");
	file.write(refresh_count.toLocal8Bit().data());
	file.write("\r\n");
	file.write(reboot_count.toLocal8Bit().data());
	file.write("\r\n");
	qDebug()<<"MC_Write_Life_Time_Data done"<<endl;
	file.close();
}

void MainApp::MC_Reset_Life_Time_Data(void)
{
  QString qFHostName = "/usr/local/lifetimedata";
	QFile file(qFHostName);
  if(QFile::exists(qFHostName))
	{
	  file.remove();
	}

}

void MainApp::MC_Read_Life_Time_Data(void)
{
	/*get device mac address*/
	QString qFHostName = "/usr/local/lifetimedata";
	QFile file(qFHostName);
	QString runtime_mins,refresh_count,reboot_count;
	if(QFile::exists(qFHostName))
	{
	  qDebug () << "MC_Read_Life_Time_Data:file exists" << endl;
	  file.open(QIODevice::ReadWrite | QIODevice::Text);
	  runtime_mins =  file.readLine();
		runtime_mins = runtime_mins.simplified();
		refresh_count =  file.readLine();
		refresh_count= refresh_count.simplified();
		reboot_count =  file.readLine();
		if(!reboot_count.isNull())
		  reboot_count= reboot_count.simplified();
		else reboot_count="1";
		bool ok;
		m_runtime_mins = runtime_mins.toInt(&ok,10);
		m_refresh_count = refresh_count.toInt(&ok,10);
		m_reboot_count = reboot_count.toInt(&ok,10);
		m_reboot_count++;
	  qDebug () << "run mins:" << QString::number(m_runtime_mins)<<"refresh cn:"<< QString::number(m_refresh_count)<< endl;
		qDebug () << "reboot cn:"<< QString::number(m_reboot_count)<< endl;
	  file.close();
	} else {
	  qDebug () << "file does not exists" << endl;
	  file.open(QIODevice::ReadWrite |QIODevice::Text );
	  file.write("1");
		file.write("\r\n");
		file.write("1");
		file.write("\r\n");
		file.write("1");
		file.write("\r\n");
		m_runtime_mins = 1;
		m_refresh_count = 1;
		m_reboot_count = 1;
	  qDebug()<<"MC_Read_Life_Time_Data:file created"<<endl;
	  file.close();
	}

	/*ping per timer mins*/
  _runtimeTimer.start(RUNTIME_TIMER_MINS*1000*60);
	
	/*clear cache per 5 days 5*24*60*60*1000*/
	//_timerClrCache.start(432000000);
	/*clear cache per 1 days 1*24*60*60*1000*/
	_timerClrCache.start(86400000);
}


void MainApp::MC_Read_Auto_Sleep_Wakeup_Config(void)
{
	/*get device mac address*/
	QString qFSWakeupName = "/usr/local/autosleepwakeup";
	QFile file(qFSWakeupName);
	#if 0
  	if(QFile::exists(qFSWakeupName))
  	{
  	  qDebug () << "MC_Read_Auto_Sleep_Wakeup_Config:file exists" << endl;
  	  file.open(QIODevice::ReadWrite | QIODevice::Text);
  	  m_auto_sleep_time  =  file.readLine();
  		m_auto_sleep_time = m_auto_sleep_time.simplified();
  		m_auto_sleep_hours =  file.readLine();
  		m_auto_sleep_hours = m_auto_sleep_hours.simplified();
  	  qDebug () << "auto sleep at " << m_auto_sleep_time<< ";sleep hours:"<<m_auto_sleep_hours<< endl;
  	  file.close();
  	}
  	else
  	{
  	  qDebug () << "file does not exists" << endl;
  	  file.open(QIODevice::ReadWrite |QIODevice::Text );
  	  file.write(DEFAULT_SLEEP_TIME);
  		file.write("\r\n");
  		file.write(DEFAULT_SLEEP_HOURS);
  		file.write("\r\n");
  	  qDebug()<<"MC_Read_Auto_Sleep_Wakeup_Config:file created"<<endl;
  	  file.close();
  	}
	#else
  	if(QFile::exists(qFSWakeupName))
  	{
  	  qDebug () << "MC_Read_Auto_Sleep_Wakeup_Config:file exists" << endl;
  	  file.open(QIODevice::ReadWrite | QIODevice::Text);
  	  m_auto_sleep_interval  =  file.readLine();
  		m_auto_sleep_interval = m_auto_sleep_interval.simplified();
  		m_auto_wakeup_interval =  file.readLine();
  		m_auto_wakeup_interval = m_auto_wakeup_interval.simplified();
  		
  	  qDebug () << "auto sleep interval: " << m_auto_sleep_interval<<endl;
  		file.close();
  	}
  	else
  	{
  	  qDebug () << "file does not exists" << endl;
  	  file.open(QIODevice::ReadWrite |QIODevice::Text );
  	  file.write(DEFAULT_AUTO_SLEEP_INTERVAL);
  		file.write("\r\n");
  		file.write(DEFAULT_AUTO_WAKEUP_INTERVAL);
  		file.write("\r\n");
  		
  	  qDebug()<<"MC_Read_Auto_Sleep_Wakeup_Config:file created"<<endl;
  	  file.close();
  	}
	#endif
}


void MainApp::MC_Read_Disconnect_Threshold_Config(void)
{
	/*get device mac address*/
	QString qFSDisThreshName = "/usr/local/disconnectthreshold";
	QFile file(qFSDisThreshName);
	
	if(QFile::exists(qFSDisThreshName))
	{
	  qDebug () << "MC_Read_Disconnect_Threshold_Config:file exists" << endl;
	  file.open(QIODevice::ReadWrite | QIODevice::Text);
	  m_dischonnect_threshold  =  file.readLine();
		m_dischonnect_threshold = m_dischonnect_threshold.simplified();
	  qDebug () << "disconnect_thresshold value: " << m_dischonnect_threshold<<endl;
		file.close();
	}
	else
	{
	  qDebug () << "file does not exists" << endl;
	  file.open(QIODevice::ReadWrite |QIODevice::Text );
	  file.write(DEFAULT_DISCONNECT_THRESHOLD);
		file.write("\r\n");
	  qDebug()<<"MC_Read_Disconnect_Threshold_Config:file created"<<endl;
	  file.close();
	}
}


void MainApp::MC_Check_Https_Exsit(void)
{
	QString qFHttpFlag = "/usr/local/use_https";

	if(QFile::exists(qFHttpFlag))m_https_exsit=true;
	else m_https_exsit=false;

}

bool MainApp::MC_Get_Https_Flag(void)
{
	return m_https_exsit;
}

void MainApp::MC_Read_Host_Name(void)
{
	/*get device mac address*/
	QString qFHostName;
	
	QFile file;
	if(m_https_exsit){
		qFHostName = "/usr/local/hostnamehttps";
	} else {
		qFHostName = "/usr/local/hostname";
	}
	file.setFileName(qFHostName);
	if(QFile::exists(qFHostName))
	{
	  qDebug () << "ssl MC_Read_Host_Name:file exists" << endl;
	  file.open(QIODevice::ReadWrite | QIODevice::Text);
	  m_http_host_name =  file.readLine();
		m_http_host_name= m_http_host_name.simplified();
		m_mqtt_host_name =  file.readLine();
		m_mqtt_host_name=m_mqtt_host_name.simplified();
		m_mqtt_host_port =  file.readLine();
		m_mqtt_host_port=m_mqtt_host_port.simplified();
		m_mqtt_bk_host_name =  file.readLine();
		if(!m_mqtt_bk_host_name.isNull()) {
			m_mqtt_bk_host_name=m_mqtt_bk_host_name.simplified();
	  }
	  qDebug () << "ssl http and mqtt and port" << m_http_host_name<< m_mqtt_host_name<<m_mqtt_host_port<< endl;
	  file.close();
	}
	else
	{
	  qDebug () << "file does not exists" << endl;
	  file.open(QIODevice::ReadWrite |QIODevice::Text );
	  file.write(DEFAULT_STAGING_HTTP_HOST_NAME);
		file.write("\r\n");
		file.write(DEFAULT_STAGING_MQTT_HOST_NAME);
		file.write("\r\n");
		file.write(DEFAULT_MQTT_HOST_PORT);
		file.write("\r\n");
		
	  qDebug()<<"ssl MC_Read_Host_Name:file created"<<endl;
	  file.close();
	}
}


void MainApp::MC_write_Host_Name(QString webserver,QString mqttserver,QString mqttport,QString mqttbkserver)
{
	/*get device mac address*/
	QString qFHostName = "/usr/local/hostname";
	QFile file(qFHostName);
	if(QFile::exists(qFHostName))
	{
	    file.remove();
	}

	{
	qDebug () << "MC_write_Host_Name:" << webserver<<"mqser:"<<mqttserver<<"mqport:"<< mqttport<<"mqttbkserver:"<<mqttbkserver<< endl;
	file.open(QIODevice::ReadWrite |QIODevice::Text );
	file.write(webserver.toLocal8Bit().data());
	file.write("\r\n");
	file.write(mqttserver.toLocal8Bit().data());
	file.write("\r\n");
	file.write(mqttport.toLocal8Bit().data());
	file.write("\r\n");
	if(!mqttbkserver.isEmpty()) {
		file.write(mqttbkserver.toLocal8Bit().data());
	  file.write("\r\n");
	}
		
	qDebug()<<"MC_write_Host_Name:file created"<<endl;
	file.close();
	}
}

void MainApp::MC_write_DNS_IP(QString dns1,QString dns2,QString dns3,QString dns4,QString dns5)
{
	/*get device mac address*/
	QString qFHostName = "/etc/resolv.conf";
	QFile file(qFHostName);
	if(QFile::exists(qFHostName))
	{
	  file.remove();
	}

	{
	qDebug () << "MC_write_Host_Name dns1:" << dns1<<"dns2:"<<dns2<<"dns3:"<< dns3<<"dns4:"<<dns4<<"dns5:"<<dns5<< endl;
	file.open(QIODevice::ReadWrite |QIODevice::Text );
	file.write(dns1.toLocal8Bit().data());
	file.write("\r\n");
	file.write(dns2.toLocal8Bit().data());
	file.write("\r\n");
	file.write(dns3.toLocal8Bit().data());
	file.write("\r\n");
	file.write(dns4.toLocal8Bit().data());
	file.write("\r\n");
	file.write(dns5.toLocal8Bit().data());
	file.write("\r\n");
		
	qDebug()<<"MC_write_Host_Name:file created"<<endl;
	file.close();
	}

}

void  MainApp::MC_Read_Device_MAC()
{
  /*get device mac address*/
	QFile mcAddressLine("/usr/local/address");
	if(!mcAddressLine.exists()){
		system("cat /sys/class/net/eth0/address > /usr/local/address");
	} 
	mcAddressLine.open(QIODevice::ReadOnly | QIODevice::Text);
	m_mac_address.clear();
	m_mac_address = mcAddressLine.readLine();
	m_mac_address = m_mac_address.simplified();
	mcAddressLine.close();
}

QString  MainApp::MC_Get_mqtt_msg()
{
  return m_mqtt_msg;
}

QString  MainApp::MC_Get_Device_MAC()
{
  return m_mac_address;
}

QString  MainApp::MC_Get_Device_Ip_Addr()
{
	return m_ip_address;
}

void  MainApp::MC_Read_Device_Ip_Addr()
{
  /*get device mac address*/
  QFile ipAddressLine("/usr/local/ipaddr");
  if(ipAddressLine.exists()){
		ipAddressLine.remove();
  }
  
  #ifdef WIFI_VERSION	
	  QString ipCmd ="/sbin/ifconfig wlan0 |sed -n '/inet addr/s/^[^:]*:\\([0-9.]\\{7,15\\}\\) .*/\\1/p' > /usr/local/ipaddr";
  #else
	  QString ipCmd ="/sbin/ifconfig eth0 |sed -n '/inet addr/s/^[^:]*:\\([0-9.]\\{7,15\\}\\) .*/\\1/p' > /usr/local/ipaddr";
  #endif
	
  qDebug() <<"ipCmd:"<<ipCmd<<endl;
  system(ipCmd.toLocal8Bit().data());
	ipAddressLine.open(QIODevice::ReadOnly | QIODevice::Text);
	m_ip_address.clear();
  m_ip_address = ipAddressLine.readAll();
	m_ip_address = m_ip_address.simplified();
  ipAddressLine.close();
	
  if(m_ip_address.size()<4){
		/*no ip,put xx.xx.xx.xx*/
		m_ip_address = "xx.xx.xx.xx";
	}
}

bool MainApp::MC_Get_Con_Status(){
	return m_web_con;
}

bool MainApp::MC_Get_Charging_Status(){
	return m_bat_charging_status;
}

bool MainApp::MC_Get_Mqtt_Con_Status(){
	return m_ComQmqttTsk->isConnectedMain();
}

void MainApp::MC_Start_Watchdog_Timer(){
  #ifdef SKH_DEVICE_INFO_SHOW
	  /*feed watchdog*/
    QString cmdline ="echo 1 >/dev/watchdog";
	  system(cmdline.toLocal8Bit().data());
  #else
	  system("killall watchdogd");
	  qDebug()<<"killall watchdogd"<<endl;
  #endif
	
  _timerFeedWatchDog.start(10000);
}

