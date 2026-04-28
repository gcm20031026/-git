/****************************************************************************
** Meta object code from reading C++ file 'ffmpegtorgbkun.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playThread/ffmpegtorgbkun.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ffmpegtorgbkun.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ffmpegToRGBKun_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ffmpegToRGBKun_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ffmpegToRGBKun_t qt_meta_stringdata_ffmpegToRGBKun = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ffmpegToRGBKun"
QT_MOC_LITERAL(1, 15, 5), // "toImg"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "QLabel*"
QT_MOC_LITERAL(4, 30, 15), // "progressChanged"
QT_MOC_LITERAL(5, 46, 5) // "value"

    },
    "ffmpegToRGBKun\0toImg\0\0QLabel*\0"
    "progressChanged\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ffmpegToRGBKun[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,
       4,    1,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void ffmpegToRGBKun::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ffmpegToRGBKun *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toImg((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< QLabel*(*)>(_a[2]))); break;
        case 1: _t->progressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLabel* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ffmpegToRGBKun::*)(QImage , QLabel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ffmpegToRGBKun::toImg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ffmpegToRGBKun::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ffmpegToRGBKun::progressChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ffmpegToRGBKun::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_ffmpegToRGBKun.data,
    qt_meta_data_ffmpegToRGBKun,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ffmpegToRGBKun::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ffmpegToRGBKun::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ffmpegToRGBKun.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ffmpegToRGBKun::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ffmpegToRGBKun::toImg(QImage _t1, QLabel * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ffmpegToRGBKun::progressChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
