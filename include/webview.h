#ifndef WEBVIEW_H  
#define WEBVIEW_H  

#include <QWidget>
#include <QtWebKitWidgets/QWebView>
#include <QFile>

#include <QNetworkReply>
#include <QSslError>



class WebPage;
class MainApp;
class QPainter;
class WebView : public QWebView  
{  
   Q_OBJECT  

public:  
    //WebView(QWidget *parent = 0); 
    WebView(); 
    ~WebView();  
    void makeConnect();
    void LoadDefaultTemplate(void);
    void bind( MainApp* ma);
    void set_hide_flag(bool flag);
    void show_disconnect_charging_status();
	
signals:  

    void stopfreshTimer();
    void showFullLoadImageSig();


public slots:  

    void contentsSizeChangedHandle(const QSize& size);
    void onLoadFinished(bool okay);
	void handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

protected:
    void paintEvent(QPaintEvent *event);
    
private:


    QFile* m_file; 
    MainApp* m_mainApp;
    bool m_hide_flag;
	

};  
  
#endif // WEBVIEW_H  

