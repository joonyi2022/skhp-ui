/****************************************************************************
** Meta object code from reading C++ file 'webview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/webview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WebView_t {
    QByteArrayData data[13];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebView_t qt_meta_stringdata_WebView = {
    {
QT_MOC_LITERAL(0, 0, 7), // "WebView"
QT_MOC_LITERAL(1, 8, 14), // "stopfreshTimer"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 20), // "showFullLoadImageSig"
QT_MOC_LITERAL(4, 45, 25), // "contentsSizeChangedHandle"
QT_MOC_LITERAL(5, 71, 4), // "size"
QT_MOC_LITERAL(6, 76, 14), // "onLoadFinished"
QT_MOC_LITERAL(7, 91, 4), // "okay"
QT_MOC_LITERAL(8, 96, 15), // "handleSslErrors"
QT_MOC_LITERAL(9, 112, 14), // "QNetworkReply*"
QT_MOC_LITERAL(10, 127, 5), // "reply"
QT_MOC_LITERAL(11, 133, 16), // "QList<QSslError>"
QT_MOC_LITERAL(12, 150, 6) // "errors"

    },
    "WebView\0stopfreshTimer\0\0showFullLoadImageSig\0"
    "contentsSizeChangedHandle\0size\0"
    "onLoadFinished\0okay\0handleSslErrors\0"
    "QNetworkReply*\0reply\0QList<QSslError>\0"
    "errors"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   41,    2, 0x0a /* Public */,
       6,    1,   44,    2, 0x0a /* Public */,
       8,    2,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QSize,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,   12,

       0        // eod
};

void WebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WebView *_t = static_cast<WebView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopfreshTimer(); break;
        case 1: _t->showFullLoadImageSig(); break;
        case 2: _t->contentsSizeChangedHandle((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        case 3: _t->onLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->handleSslErrors((*reinterpret_cast< QNetworkReply*(*)>(_a[1])),(*reinterpret_cast< const QList<QSslError>(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WebView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebView::stopfreshTimer)) {
                *result = 0;
            }
        }
        {
            typedef void (WebView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebView::showFullLoadImageSig)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject WebView::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_WebView.data,
      qt_meta_data_WebView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WebView.stringdata0))
        return static_cast<void*>(const_cast< WebView*>(this));
    return QWebView::qt_metacast(_clname);
}

int WebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void WebView::stopfreshTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void WebView::showFullLoadImageSig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
