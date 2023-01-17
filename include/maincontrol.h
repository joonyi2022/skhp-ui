#ifndef MAINCONTROL_H
#define MAINCONTROL_H



#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QQueue>



class WebView;
class ComHttpsThread;
class ComQmqttThread;
class FwUpgradeThread;
typedef enum{
    DEVICE_9N7,
    DEVICE_9N7_H,
    DEVICE_13N3,
    DEVICE_13N3_H,
    DEVICE_13N3_LR
} Device_Type;


/****************************************************/
class MainApp : public QMainWindow
{
    Q_OBJECT


public:
     MainApp();
    ~MainApp();
    void start();
    void MC_LoadRemoteMeetingRoomTemplate(QString url);
    void MC_LoadDefaultMeetingRoomTemplate(void);
    void LoadDefaultTemplate(void);
    void MC_LoadDeviceInfo(void);
    void MC_LoadLocalTemplate(void);
    void MC_LoadRomoteTemplate(void);
    void Full_reflesh();
	void UpdateDisplayDemo();
    void    MC_Read_Host_Name(void);
    QString MC_Get_HTTP_Host_Name(void);
    QString MC_Get_MQTT_Host_Port(void);
    QString MC_Get_MQTT_Host_Name(void); 
    QString MC_Get_MQTT_Backup_Host_Name(void);
    QString  MC_Get_mqtt_msg();
    QString    MC_Get_Device_MAC();
    QString    MC_Get_Device_Ip_Addr();
    void    MC_Read_Device_MAC();
    void    MC_Read_Device_Ip_Addr();
    void    MC_Read_Life_Time_Data(void);
    void    MC_Write_Life_Time_Data(void);
    void    MC_refresh_count_update(void);
    void    MC_Reset_Life_Time_Data(void);
    void    MC_Speed_Mode();
    void    MC_Picture_Mode();
    bool    MC_Check_Host_Network(int backrun_flag);
    int     MC_Get_Template_Mode(void);
    void    MC_Start_Ping();
    void    MC_Get_Device_Display_Size();
    void    MC_LoadShutOffTemplate(void);
    void    MC_LoadInactiveTemplate(void);
    int     MC_Get_Device_Display_Width(void);
   QString  MC_Get_DeviceInfo(void);
   QString  MC_Get_FwVersion(void);
   QString  MC_Get_UiAppVersion(void);
   void    MC_Read_VersionNumber(void);
   void    MC_Read_UiAppVersionNumber(void);
   void     MC_Read_DeviceInfo(void);
   void     MC_LoadWhiteTemplate(void);
   void     MC_LoadBlackTemplate(void);
   void     MC_write_Host_Name(QString webserver,QString mqttserver,QString mqttport,QString mqttbkserver);
   void     MC_write_DNS_IP(QString dns1,QString dns2,QString dns3,QString dns4,QString dns5);
   void     MC_Update_Bed_Info(QString data);
   void     MC_Bed_Info_Reset(QString data);
   void     MC_Vertical_Display();
   void     MC_Horizon_Display();
    bool    MC_Get_Con_Status();
    bool    MC_Get_Mqtt_Con_Status();
    bool    MC_Get_Charging_Status();
    void    MC_Read_Device_Config(void);
    void    MC_Read_Auto_Sleep_Wakeup_Config(void);
    void    MC_Read_Disconnect_Threshold_Config(void);
    void    MC_Device_Auto_Sleep();
    void    MC_Start_Watchdog_Timer();
    void    MC_Check_Https_Exsit(void);
    bool    MC_Get_Https_Flag(void);

signals:
    void    checkNetworkConSignal();
    void    replyQnumberSignal(QString);
public slots:
    void UpdateDisplay();
    void UpdateMeetingRoomDisplay(const QString &);
    void MC_Mqtt_CMD_Handler(QString data);
    bool MC_Mqtt_msg_ack(QString data);
    void MC_Decode_MQTT_Cmd(QString data);

    void UpdateDisplayStopTimer();
    void showFullLoadImage();
    void onTimeout();
    void onNetworkCheckTimeout();
	void onDemoTimeout();
    void onRefleshTimeout();
    void onRefleshTimeout97();
    void onRefleshTimeout133();
    void onJustRefleshTimeout();
    void onFullLoadTimeout();
    void onDemoBatTimeout();
    void onShutdownTimeout();
    void onRuntimeTimeout();
    bool showBat();
    void blinkingTimeout();
    void onTimeClrCacheout();
    void onQueueNumTimerTimeout();
    void onHttpReplyTimerTimeout();
    void onWatchDogTimeout();
    void reloadTemplate();
    QString getBatterLevel();
    void replyFinished(QNetworkReply * reply);
    void mqttReplyFinished(QNetworkReply * reply);
	void handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

protected:
   

private:
    void readBatterLevel();
    void readBatterVol();
    void regesiterDevice();
    void response_q_num( QString num);
    void response_mqtt_ack( QString num);
    void cmd_update_config_handle(QJsonObject jsobj);
    void get_last_config();
    void triger_p2_resend();
    void upload_screen_shot();
    void upload_log_file();
    void readCPUInfo(void);
    void readMemInfo(void);
    QByteArray buildUploadString();
    QByteArray buildLogUploadString();
  
    
    WebView* m_webView;
    WebView* m_webViewHiden;
    ComHttpsThread* m_ComHttpsTsk;
    ComQmqttThread* m_ComQmqttTsk;
    FwUpgradeThread* m_FwUpgradeTsk;
    int m_num;
    QTimer _timer;
    QTimer _timerFeedWatchDog;
    QTimer _timerBlinking;
    QTimer _timerClrCache;
    QTimer _queueNumTimer;
    QTimer _timerNetworkCheck;
    QTimer _timerReflesh;
    QTimer _timerReflesh97;
    QTimer _timerReflesh133;
    QTimer _timerJustReflesh;
    QTimer _timerHttpReply;
    QTimer _timerFullLoading;
	int m_blinking_interval;
    QString m_batStr;
    QString m_bat_voltage;
    bool    m_bat_charging_status;
    QString m_ifconfigStr;
    QString m_fwVersionStr;
    QString m_uiAppVersionStr;
    QString m_http_host_name;
    QString m_mqtt_host_name;
    QString m_mqtt_bk_host_name;
    QString m_mqtt_host_port;
    int     m_runtime_mins;
    int     m_refresh_count;
    int     m_reboot_count;
    int     m_fullrefresh97_count;
    bool    m_screen_13n3_exit;
    QString m_mac_address;
    QString m_ip_address;
    QString m_mqtt_msg;
    QString m_speaker_vol;
    QString m_speaker_beep;
    QString m_call_action;
    QQueue<QString> m_mqtt_queue;
    int  m_shutdown_timer_count;
    bool m_commqtt_tsk_runing;
    QString m_shutdown_template;
    QString m_cpu_percentage;
    QString m_memory_percentage;
    QString m_device_status;
    QString m_error_number;
    int g_screen_width;
    int g_screen_height;
    QString m_template_url;
    QString m_pre_msg_id;
    QString m_queue_number_time;
    int m_template_mode;
	QTimer _demoTimer;
    QTimer _shutdownTimer;
    QTimer _demoTimerBattery;
    QTimer _runtimeTimer;// record the running time of the device
    bool m_bat_show;
    bool m_template_changed;
    bool m_showing_number;
    bool m_current_blank_number;
    QString m_updatewin_data_para;
	QStringList demo_docname;
	int     demo_docname_index;
	int     demo_call_number;
    int     _number;
    int     m_sound_count;
    int     m_dingdong_sound_count;
    int     m_max_count;
    int     m_showfullscreen;
    #ifdef SKH_HK_DEMO
    int   m_exchange ;
    #endif
	QNetworkAccessManager *m_restclient;
    QNetworkAccessManager *m_mqttreply;
    QNetworkReply *m_reply;
    bool m_device_register;
    bool m_https_exsit;
    bool m_web_con;
    int m_template_change_refresh_interval;
    int m_content_chang_refresh_interval;
    int m_disconnect_count;
    QString m_auto_sleep_time;
    QString m_auto_sleep_hours;
    QString m_auto_sleep_interval;
    QString m_auto_wakeup_interval;
    QString m_dischonnect_threshold;
    QString m_default_133_template;
    QString m_shutdown_133_template;
    Device_Type m_device_type;
};

#endif // MAINCONTROL_H
