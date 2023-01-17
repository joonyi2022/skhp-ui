/****************************************************************************
** Meta object code from reading C++ file 'com_qmqtt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/com_qmqtt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'com_qmqtt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Publisher_t {
    QByteArrayData data[5];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Publisher_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Publisher_t qt_meta_stringdata_Publisher = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Publisher"
QT_MOC_LITERAL(1, 10, 11), // "onConnected"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 9), // "onTimeout"
QT_MOC_LITERAL(4, 33, 14) // "onDisconnected"

    },
    "Publisher\0onConnected\0\0onTimeout\0"
    "onDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Publisher[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Publisher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Publisher *_t = static_cast<Publisher *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onConnected(); break;
        case 1: _t->onTimeout(); break;
        case 2: _t->onDisconnected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Publisher::staticMetaObject = {
    { &QMQTT::Client::staticMetaObject, qt_meta_stringdata_Publisher.data,
      qt_meta_data_Publisher,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Publisher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Publisher::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Publisher.stringdata0))
        return static_cast<void*>(const_cast< Publisher*>(this));
    return QMQTT::Client::qt_metacast(_clname);
}

int Publisher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMQTT::Client::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_Subscriber_t {
    QByteArrayData data[12];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Subscriber_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Subscriber_t qt_meta_stringdata_Subscriber = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Subscriber"
QT_MOC_LITERAL(1, 11, 19), // "updateDisplaySignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "reloadTemplateSignal"
QT_MOC_LITERAL(4, 53, 16), // "downloadfwSignal"
QT_MOC_LITERAL(5, 70, 14), // "onDisconnected"
QT_MOC_LITERAL(6, 85, 11), // "onConnected"
QT_MOC_LITERAL(7, 97, 12), // "onSubscribed"
QT_MOC_LITERAL(8, 110, 5), // "topic"
QT_MOC_LITERAL(9, 116, 10), // "onReceived"
QT_MOC_LITERAL(10, 127, 14), // "QMQTT::Message"
QT_MOC_LITERAL(11, 142, 7) // "message"

    },
    "Subscriber\0updateDisplaySignal\0\0"
    "reloadTemplateSignal\0downloadfwSignal\0"
    "onDisconnected\0onConnected\0onSubscribed\0"
    "topic\0onReceived\0QMQTT::Message\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Subscriber[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    0,   52,    2, 0x06 /* Public */,
       4,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   54,    2, 0x0a /* Public */,
       6,    0,   55,    2, 0x0a /* Public */,
       7,    1,   56,    2, 0x0a /* Public */,
       9,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void Subscriber::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Subscriber *_t = static_cast<Subscriber *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateDisplaySignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->reloadTemplateSignal(); break;
        case 2: _t->downloadfwSignal(); break;
        case 3: _t->onDisconnected(); break;
        case 4: _t->onConnected(); break;
        case 5: _t->onSubscribed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onReceived((*reinterpret_cast< const QMQTT::Message(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMQTT::Message >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Subscriber::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::updateDisplaySignal)) {
                *result = 0;
            }
        }
        {
            typedef void (Subscriber::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::reloadTemplateSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (Subscriber::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Subscriber::downloadfwSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Subscriber::staticMetaObject = {
    { &QMQTT::Client::staticMetaObject, qt_meta_stringdata_Subscriber.data,
      qt_meta_data_Subscriber,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Subscriber::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Subscriber::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Subscriber.stringdata0))
        return static_cast<void*>(const_cast< Subscriber*>(this));
    return QMQTT::Client::qt_metacast(_clname);
}

int Subscriber::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMQTT::Client::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Subscriber::updateDisplaySignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Subscriber::reloadTemplateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Subscriber::downloadfwSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
struct qt_meta_stringdata_ComQmqttThread_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComQmqttThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComQmqttThread_t qt_meta_stringdata_ComQmqttThread = {
    {
QT_MOC_LITERAL(0, 0, 14) // "ComQmqttThread"

    },
    "ComQmqttThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComQmqttThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ComQmqttThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ComQmqttThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ComQmqttThread.data,
      qt_meta_data_ComQmqttThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ComQmqttThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComQmqttThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ComQmqttThread.stringdata0))
        return static_cast<void*>(const_cast< ComQmqttThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ComQmqttThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
