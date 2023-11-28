#include "clockwgt.h"
#include <QPainter>
#include <QRadialGradient>
#include <QPainterPath>
#include <QTimer>
#include <QTime>

ClockWgt::ClockWgt(QWidget *parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));//定时调用绘制事件函数
    timer->start(500);//开启定时器，执行周期为1秒针

    setFixedSize(666,666);
}

void ClockWgt::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//设置抗锯齿
    Draw_Clock(&painter);
}

void ClockWgt::Draw_Clock(QPainter *painter)
{
    Init_Parameter();
    Set_Mask(painter);

    Draw_Broad(painter);
    Draw_Dial(painter);
    Draw_Text(painter);
    Draw_Pointer(painter);
}

void ClockWgt::Init_Parameter()
{
    Center_pos = QPoint(this->width()/2,this->height()/2);//以窗体中心为圆心
    R_Edge = this->height()/2;//以窗体高度的一半为半径
    R_Inside = R_Edge-10;
    R_Center = 15;
    R_Pointer = 6;
    QTime Time = QTime::currentTime();//获取当前系统时间

    H=Time.hour();//分别设置时、分、秒
    M=Time.minute();
    S=Time.second();
}

void ClockWgt::Set_Mask(QPainter *painter)
{
    painter->save();//保存

    QBrush brush = QBrush(Qt::transparent);//设置画刷为透明
    painter->setBrush(brush);
    painter->fillRect(this->rect(),QColor(0,0,0,0));

    painter->restore();//恢复
}

void ClockWgt::Draw_Broad(QPainter *painter)
{
    painter->save();

    QBrush brush = QBrush(Color_Edge);//绘制外部圆
    painter->setBrush(brush);
    painter->drawEllipse(Center_pos,R_Edge,R_Edge);

    brush = QBrush(Color_Inside);//绘制内部圆
    painter->setBrush(brush);
    painter->drawEllipse(Center_pos,R_Inside,R_Inside);

    brush = QBrush(Color_Center);//绘制中心小圆
    painter->setBrush(brush);
    painter->drawEllipse(Center_pos,R_Center,R_Center);

    painter->restore();
}

void ClockWgt::Draw_Dial(QPainter *painter)
{
    painter->save();

    for(int Loop = 0; Loop <= Div_Max*Div_Min; Loop++)
    {
        float Angle = BaseAngle + (360 / (Div_Max * Div_Min))*Loop;//从基础仰角开始绘制的每条刻度线对应的角度
        int R = R_Inside-1;
        int x_start = Center_pos.x() + R * cos((Angle / 180) * PI);
        int y_start = Center_pos.y() + R * sin((Angle / 180) * PI);

        if(Loop % Div_Min == 0)//判断是否为大刻度
        {
            QPen pen(qRgb(50,140,245));
            pen.setWidth(4);
            painter->setPen(pen);
            R = R_Inside-20;
        }
        else  //判断是否为小刻度
        {
            QPen pen(qRgb(90,215,240));
            pen.setWidth(2);
            painter->setPen(pen);
            R = R_Inside-15;
        }

        int x_end = Center_pos.x() + R * cos((Angle / 180) * PI);
        int y_end = Center_pos.y() + R * sin((Angle / 180) * PI);

        painter->drawLine(QPoint(x_start,y_start),QPoint(x_end,y_end));//绘制刻度线
    }


    painter->restore();
}

void ClockWgt::Draw_Text(QPainter *painter)
{
    painter->save();

    QPen qPen(qRgb(88,172,169));
    qPen.setWidth(5);   //设置画笔的粗细
    painter->setPen(qPen);
    QFont qFont("华文行楷",36,QFont::Bold,false);
    painter->setFont(qFont);

    int Dial_Text = 12;
    for(int Loop = 0;Loop < Div_Max;Loop++)
    {
        if(Dial_Text >12 )
            Dial_Text = 1;
        int R = R_Inside-60;
        float Angle = BaseAngle + (360 / Div_Max )*Loop;
        int x = Center_pos.x() + R * cos((Angle / 180) * PI);
        int y = Center_pos.y() + R * sin((Angle / 180) * PI);

        painter->drawText(QRect(x-20,y-20,80,80),QString::number(Dial_Text++));
    }
    painter->drawText(Center_pos.x()-60,Center_pos.y()+60,"WANG");


    painter->restore();
}

void ClockWgt::Draw_Pointer(QPainter *painter)
{
    painter->save();

    QPen qPen(QColor(215,180,200));
    qPen.setWidth(2);   //设置画笔的粗细
    painter->setPen(qPen);
    //**********绘制秒针***********************************************************************************
    QBrush qBrush = QBrush(QColor(225,210,210));
    painter->setBrush(qBrush);
    float Angle = BaseAngle + (360 / (Div_Max * Div_Min))*S;
    float RightAngle = Angle + 90;//右侧角度
    float LeftAngle = Angle - 90;//左侧角度
    int R = R_Inside-1;
    float x_start = Center_pos.x() + R * cos((Angle / 180) * PI);
    float y_start = Center_pos.y() + R * sin((Angle / 180) * PI);

    R = R_Pointer-1;
    float x_end1 = Center_pos.x() + R * cos(RightAngle * 3.14 / 180);//过圆心的右侧切点
    float y_end1 = Center_pos.y() + R * sin(RightAngle * 3.14 / 180);
    float x_end2 = Center_pos.x() + R * cos(LeftAngle * 3.14 / 180);//过圆心的左侧切点
    float y_end2 = Center_pos.y() + R * sin(LeftAngle * 3.14 / 180);

    QPointF qTriangle_S[3] = {QPoint(x_start,y_start),QPoint(x_end1,y_end1),QPoint(x_end2,y_end2)};
    painter->drawPolygon(qTriangle_S,3);
    //**********绘制分针***********************************************************************************
    QBrush qBrush1 = QBrush(QColor(160,220,170));
    painter->setBrush(qBrush1);
    Angle = BaseAngle + (360 / (Div_Max * Div_Min))*M + (360 / (Div_Max * Div_Min))*(S / 60);
    RightAngle = Angle + 90;//右侧角度
    LeftAngle = Angle - 90;//左侧角度
    R = R_Inside-60;
    x_start = Center_pos.x() + R * cos((Angle / 180) * PI);
    y_start = Center_pos.y() + R * sin((Angle / 180) * PI);

    R = R_Pointer-1;
    x_end1 = Center_pos.x() + R * cos(RightAngle * 3.14 / 180);//过圆心的右侧切点
    y_end1 = Center_pos.y() + R * sin(RightAngle * 3.14 / 180);
    x_end2 = Center_pos.x() + R * cos(LeftAngle * 3.14 / 180);//过圆心的左侧切点
    y_end2 = Center_pos.y() + R * sin(LeftAngle * 3.14 / 180);

    QPointF qTriangle_M[3] = {QPoint(x_start,y_start),QPoint(x_end1,y_end1),QPoint(x_end2,y_end2)};
    painter->drawPolygon(qTriangle_M,3);
    //**********绘制时针***********************************************************************************
    QBrush qBrush2 = QBrush(QColor(150,220,220));
    painter->setBrush(qBrush2);
    Angle = BaseAngle + (360 / Div_Max)*H + (360 / Div_Max)*(M / 60);
    RightAngle = Angle + 90;//右侧角度
    LeftAngle = Angle - 90;//左侧角度
    R = R_Inside-120;
    x_start = Center_pos.x() + R * cos((Angle / 180) * PI);
    y_start = Center_pos.y() + R * sin((Angle / 180) * PI);

    R = R_Pointer-1;
    x_end1 = Center_pos.x() + R * cos(RightAngle * 3.14 / 180);//过圆心的右侧切点
    y_end1 = Center_pos.y() + R * sin(RightAngle * 3.14 / 180);
    x_end2 = Center_pos.x() + R * cos(LeftAngle * 3.14 / 180);//过圆心的左侧切点
    y_end2 = Center_pos.y() + R * sin(LeftAngle * 3.14 / 180);

    QPointF qTriangle_H[3] = {QPoint(x_start,y_start),QPoint(x_end1,y_end1),QPoint(x_end2,y_end2)};
    painter->drawPolygon(qTriangle_H,3);

    painter->restore();
}
