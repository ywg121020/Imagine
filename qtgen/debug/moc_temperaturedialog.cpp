/****************************************************************************
** Meta object code from reading C++ file 'temperaturedialog.h'
**
** Created: Fri Apr 22 15:42:30 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../temperaturedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'temperaturedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TemperatureDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   18,   19,   18, 0x0a,
      43,   18,   18,   18, 0x08,
      63,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TemperatureDialog[] = {
    "TemperatureDialog\0\0int\0updateTemperature()\0"
    "on_btnSet_clicked()\0on_radioButtonOn_toggled(bool)\0"
};

const QMetaObject TemperatureDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TemperatureDialog,
      qt_meta_data_TemperatureDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TemperatureDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TemperatureDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TemperatureDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TemperatureDialog))
        return static_cast<void*>(const_cast< TemperatureDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int TemperatureDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = updateTemperature();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: on_btnSet_clicked(); break;
        case 2: on_radioButtonOn_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
