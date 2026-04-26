/****************************************************************************
** Meta object code from reading C++ file 'decodethread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playThread/decodethread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'decodethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DecodeThread_t {
    QByteArrayData data[23];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DecodeThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DecodeThread_t qt_meta_stringdata_DecodeThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DecodeThread"
QT_MOC_LITERAL(1, 13, 7), // "sentImg"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 3), // "img"
QT_MOC_LITERAL(4, 26, 13), // "sentvideoTime"
QT_MOC_LITERAL(5, 40, 5), // "times"
QT_MOC_LITERAL(6, 46, 16), // "sentProgressTime"
QT_MOC_LITERAL(7, 63, 9), // "checkFlag"
QT_MOC_LITERAL(8, 73, 4), // "flag"
QT_MOC_LITERAL(9, 78, 15), // "getProgressTime"
QT_MOC_LITERAL(10, 94, 5), // "value"
QT_MOC_LITERAL(11, 100, 8), // "getSpeed"
QT_MOC_LITERAL(12, 109, 5), // "speed"
QT_MOC_LITERAL(13, 115, 7), // "set_X_Y"
QT_MOC_LITERAL(14, 123, 1), // "x"
QT_MOC_LITERAL(15, 125, 1), // "y"
QT_MOC_LITERAL(16, 127, 14), // "savaPictureScr"
QT_MOC_LITERAL(17, 142, 7), // "picName"
QT_MOC_LITERAL(18, 150, 23), // "startStopDetectfaceSlot"
QT_MOC_LITERAL(19, 174, 14), // "detectFaceFlag"
QT_MOC_LITERAL(20, 189, 10), // "saveVideos"
QT_MOC_LITERAL(21, 200, 7), // "getTime"
QT_MOC_LITERAL(22, 208, 7) // "m_times"

    },
    "DecodeThread\0sentImg\0\0img\0sentvideoTime\0"
    "times\0sentProgressTime\0checkFlag\0flag\0"
    "getProgressTime\0value\0getSpeed\0speed\0"
    "set_X_Y\0x\0y\0savaPictureScr\0picName\0"
    "startStopDetectfaceSlot\0detectFaceFlag\0"
    "saveVideos\0getTime\0m_times"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DecodeThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   78,    2, 0x0a /* Public */,
       9,    1,   81,    2, 0x0a /* Public */,
      11,    1,   84,    2, 0x0a /* Public */,
      13,    2,   87,    2, 0x0a /* Public */,
      16,    1,   92,    2, 0x0a /* Public */,
      18,    1,   95,    2, 0x0a /* Public */,
      20,    0,   98,    2, 0x0a /* Public */,
      21,    1,   99,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,

       0        // eod
};

void DecodeThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DecodeThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sentImg((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->sentvideoTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->sentProgressTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->checkFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->getProgressTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->getSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->set_X_Y((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->savaPictureScr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->startStopDetectfaceSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->saveVideos(); break;
        case 10: _t->getTime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DecodeThread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DecodeThread::sentImg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DecodeThread::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DecodeThread::sentvideoTime)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DecodeThread::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DecodeThread::sentProgressTime)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DecodeThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_DecodeThread.data,
    qt_meta_data_DecodeThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DecodeThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DecodeThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DecodeThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int DecodeThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DecodeThread::sentImg(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DecodeThread::sentvideoTime(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DecodeThread::sentProgressTime(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
