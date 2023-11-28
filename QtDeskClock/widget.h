#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "clockwgt.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    ClockWgt* wgt_clock;

    QLabel* label_time[5];
    QLabel* label_date;

private:
    Ui::Widget *ui;

public slots:
    void update_dateTime();

};
#endif // WIDGET_H
