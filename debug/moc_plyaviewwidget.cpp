/****************************************************************************
** Meta object code from reading C++ file 'plyaviewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../View/plyaviewwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plyaviewwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_plyaViewWidget_t {
    QByteArrayData data[24];
    char stringdata0[268];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_plyaViewWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_plyaViewWidget_t qt_meta_stringdata_plyaViewWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "plyaViewWidget"
QT_MOC_LITERAL(1, 15, 8), // "sent_X_Y"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 1), // "x"
QT_MOC_LITERAL(4, 27, 1), // "y"
QT_MOC_LITERAL(5, 29, 12), // "sentSwichDev"
QT_MOC_LITERAL(6, 42, 10), // "curDevName"
QT_MOC_LITERAL(7, 53, 13), // "targetDevName"
QT_MOC_LITERAL(8, 67, 11), // "savePicture"
QT_MOC_LITERAL(9, 79, 7), // "picName"
QT_MOC_LITERAL(10, 87, 25), // "startStopDetectfaceSignal"
QT_MOC_LITERAL(11, 113, 14), // "detectFaceFlag"
QT_MOC_LITERAL(12, 128, 16), // "saveVideosSignal"
QT_MOC_LITERAL(13, 145, 13), // "setTimeSignal"
QT_MOC_LITERAL(14, 159, 6), // "m_time"
QT_MOC_LITERAL(15, 166, 10), // "updateTime"
QT_MOC_LITERAL(16, 177, 6), // "getImg"
QT_MOC_LITERAL(17, 184, 3), // "img"
QT_MOC_LITERAL(18, 188, 10), // "changeSize"
QT_MOC_LITERAL(19, 199, 15), // "SlotMenuClicked"
QT_MOC_LITERAL(20, 215, 8), // "QAction*"
QT_MOC_LITERAL(21, 224, 6), // "action"
QT_MOC_LITERAL(22, 231, 17), // "addMenuSwitchItem"
QT_MOC_LITERAL(23, 249, 18) // "SlotMenuDevClicked"

    },
    "plyaViewWidget\0sent_X_Y\0\0x\0y\0sentSwichDev\0"
    "curDevName\0targetDevName\0savePicture\0"
    "picName\0startStopDetectfaceSignal\0"
    "detectFaceFlag\0saveVideosSignal\0"
    "setTimeSignal\0m_time\0updateTime\0getImg\0"
    "img\0changeSize\0SlotMenuClicked\0QAction*\0"
    "action\0addMenuSwitchItem\0SlotMenuDevClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_plyaViewWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    2,   79,    2, 0x06 /* Public */,
       8,    1,   84,    2, 0x06 /* Public */,
      10,    1,   87,    2, 0x06 /* Public */,
      12,    0,   90,    2, 0x06 /* Public */,
      13,    1,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   94,    2, 0x0a /* Public */,
      16,    1,   95,    2, 0x0a /* Public */,
      18,    0,   98,    2, 0x0a /* Public */,
      19,    1,   99,    2, 0x0a /* Public */,
      22,    0,  102,    2, 0x0a /* Public */,
      23,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void plyaViewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<plyaViewWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sent_X_Y((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sentSwichDev((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->savePicture((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->startStopDetectfaceSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->saveVideosSignal(); break;
        case 5: _t->setTimeSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updateTime(); break;
        case 7: _t->getImg((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 8: _t->changeSize(); break;
        case 9: _t->SlotMenuClicked((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 10: _t->addMenuSwitchItem(); break;
        case 11: _t->SlotMenuDevClicked((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (plyaViewWidget::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::sent_X_Y)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (plyaViewWidget::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::sentSwichDev)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (plyaViewWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::savePicture)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (plyaViewWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::startStopDetectfaceSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (plyaViewWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::saveVideosSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (plyaViewWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&plyaViewWidget::setTimeSignal)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject plyaViewWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_plyaViewWidget.data,
    qt_meta_data_plyaViewWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *plyaViewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *plyaViewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_plyaViewWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int plyaViewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void plyaViewWidget::sent_X_Y(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void plyaViewWidget::sentSwichDev(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void plyaViewWidget::savePicture(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void plyaViewWidget::startStopDetectfaceSignal(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void plyaViewWidget::saveVideosSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void plyaViewWidget::setTimeSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
