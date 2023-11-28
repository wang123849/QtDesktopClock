#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(999,999);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//设置窗体标志
    this->setStyleSheet("QWidget{background:#FCF9F8;}");
    setAttribute(Qt::WA_TranslucentBackground);
    //    setWindowOpacity(0.2);
    move(this->width() / 2,0);

    setWindowTitle("Desktop Clock");//窗口标题
    setWindowIcon(QIcon(":/resource/1.jpg"));//资源路径

    QFont font;
    font.setPointSize(80);//数字字号
    font.setFamily("华文行楷");
    font.setBold(true);//加粗
    setFont(font);

    QTimer* m_timer=new QTimer(this);
    m_timer->start(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(update_dateTime()));

    wgt_clock=new ClockWgt(this);
    label_date=new QLabel(this);
    for(int i=0;i<5;i++)
    {
        label_time[i]=new QLabel(this);

        label_time[i]->setStyleSheet("QLabel{color:#9f58ac;"
                                     "background-color:transparent;}");
        label_time[i]->setFixedSize(150,125);
        label_time[i]->setFont(font);
        label_time[i]->setAlignment(Qt::AlignCenter);
    }
    label_date->setStyleSheet("QLabel{color:#9f58ac;"
                              "background-color:transparent;}");

    label_date->setFixedSize(960,125);
    label_date->setFont(font);

    QHBoxLayout* hbox_clock = new QHBoxLayout;
    QHBoxLayout* hbox_time = new QHBoxLayout;
    QHBoxLayout* hbox_date = new QHBoxLayout;
    hbox_clock->addStretch();
    hbox_clock->addWidget(wgt_clock);
    hbox_clock->addStretch();
    hbox_time->addStretch();
    hbox_time->setSpacing(0);
    hbox_time->addWidget(label_time[0]);
    hbox_time->addWidget(label_time[1]);
    hbox_time->addWidget(label_time[2]);
    hbox_time->addWidget(label_time[3]);
    hbox_time->addWidget(label_time[4]);
    hbox_time->addStretch();
    hbox_date->addStretch();
    hbox_date->addWidget(label_date);
    hbox_date->addStretch();

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addStretch();
    vbox->addLayout(hbox_clock);
    vbox->addSpacing(20);
    vbox->addLayout(hbox_time);
    vbox->addSpacing(20);
    vbox->addLayout(hbox_date);
    vbox->addStretch();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::update_dateTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str1 = time.toString("hh");
    QString str2 = time.toString("mm");
    QString str3 = time.toString("ss");
    label_time[0]->setText(str1);
    label_time[2]->setText(str2);
    label_time[4]->setText(str3);
    label_time[1]->setText(":");
    label_time[3]->setText(":");

    str1.clear();
    str2.clear();
    str3.clear();

    QString str4 = time.toString("yyyy-MM-dd  ddd");
    label_date->setText(str4);
    label_date->setAlignment(Qt::AlignCenter);
    str4.clear();
}
