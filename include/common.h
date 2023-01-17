#ifndef COMMON_H
#define COMMON_H


#define     app_version                         "V2.3.7"  
#define     DISPLAY_SIZE_13_3                    0
#define     DISPLAY_SIZE_9_7                     1

//#define     SCREEN_97INCH
//#ifdef      SCREEN_97INCH
#define     EINK_SCREEN97_WIDTH                   825
#define     EINK_SCREEN97_HEIGHT                  1200
//#else
#define     EINK_SCREEN133_WIDTH                   2200
#define     EINK_SCREEN133_HEIGHT                  1650

#define     EINK_SCREEN97_WIDTH_H                  1200
#define     EINK_SCREEN97_HEIGHT_H                 825
//#else
#define     EINK_SCREEN133_WIDTH_H                 1650
#define     EINK_SCREEN133_HEIGHT_H                2200

#define     EINK_SCREEN133_NEW_WIDTH               1600
#define     EINK_SCREEN133_NEW_HEIGHT              1200

#define     EINK_SCREEN133_WIDTH_1200              1200
#define     EINK_SCREEN133_HEIGHT_1600             1600


//#define     SKH_PRODUCTION

//#endif
//#define     EINK_FACTORY_AGING_TESTING      
//#define SKH_DEMO_DED_INFO
//#define CS_TESTING
//#define LOCAL_SERVER

#define LOCAL_TEMPLATE_MODE              0
#define REMOTE_TEMPLATE_MODE             1
//#define SKH_POWER_OFF_SHOW
#define     SKH_DEVICE_INFO_SHOW 
#define     SKH_WHITE_SHOW               0             
#define     SKH_BLACK_SHOW               0  
//#define     HORIZON_DISPLAY_9_7
//#define     SKH_HK_DEMO
//#define     SKH_HK_BUS_STOP
//#define     AUTO_SHUTOFF
//#define     DISPLAY_133INCH_1600_1200
//#define     OPEN_WATCHDOG
//#define     WIFI_VERSION
#define     DEBUG_MODE
#ifdef      DEBUG_MODE
#else
#define printf
#endif



#endif //COMMON_H
