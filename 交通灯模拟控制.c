#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;
sbit RED10 = P1 ^ 0;//上人行道红灯
sbit GREEN10 = P1 ^ 1;//上人行道绿灯
sbit RED11 = P1 ^ 2;
sbit YELLOW11 = P1 ^ 3;
sbit GREEN11 = P1 ^ 4;
sbit RED00 = P3 ^ 0;//右人行道红灯
sbit GREEN00 = P3 ^ 1;//右人行道绿灯
sbit RED01 = P1 ^ 5;
sbit YELLOW01 = P1 ^ 6;
sbit GREEN01 = P1 ^ 7;
sbit up = P2 ^ 5;
sbit right = P2 ^ 6;
uint k = 0;
uint l = 0;
sbit north = P3 ^ 6;
sbit east = P3 ^ 7;
uchar code smgduan[17] = {0x3f, 0x06, 0x5b, 0x4f,
                          0x66, 0x6d, 0x7d, 0x07,
                          0x7f, 0x6f, 0x77, 0x7c,
                          0x39, 0x5e, 0x79, 0x71};//显示0~F的值
uchar DisplayData[8];//显示数据
uchar Second;
void delay(uint t)
{
    while (t--);
}
void DigDisplay()
{
    uchar j;
    for(j = 0; j < 8; j++)
    {
        switch (j) //位选，选择点亮的数码管，
        {
        case 0:
            LSA = 0;
            LSB = 0;
            LSC = 0;
            break;//显示第0位
        case 1:
            LSA = 1;
            LSB = 0;
            LSC = 0;
            break;//显示第1位
        case 2:
            LSA = 0;
            LSB = 1;
            LSC = 0;
            break;//显示第2位
        case 3:
            LSA = 1;
            LSB = 1;
            LSC = 0;
            break;//显示第3位
        case 4:
            LSA = 0;
            LSB = 0;
            LSC = 1;
            break;//显示第4位
        case 5:
            LSA = 1;
            LSB = 0;
            LSC = 1;
            break;//显示第5位
        case 6:
            LSA = 0;
            LSB = 1;
            LSC = 1;
            break;//显示第6位
        case 7:
            LSA = 1;
            LSB = 1;
            LSC = 1;
            break;//显示第7位
        }
        P0 = DisplayData[j];//发送数据
        delay(100); //间隔一段时间扫描
        P0 = 0x00;//消隐
    }
}
void Time1Init()
{
    TMOD |= 0x10; //设置定时器模式
    TH1 = (65536 - 50000) / 256; //设置定时初值
    TL1 = (65536 - 50000) % 256;
    ET1 = 1; //打开定时器1中断允许
    EA = 1; //打开总中断
    TR1 = 1; //打开定时器
}
void zsd()
{
    if(up == 1)
        {north = 1;}
    if(up == 0)
        {north = 0;}
    if(right == 1)
        {east = 1;}
    if(right == 0)
        {east = 0;}
}
void main()
{
    uchar we = 1, pd = 1;
    Second = 1;
    Time1Init();
    while (1)
    {
        zsd();
        if(Second == 71)
        {
            Second = 1;
        }
        //南北方通行，30秒
        if (Second < 31)
        {
            DisplayData[0] = 0x00;
            DisplayData[1] = 0x00;
            DisplayData[2] = smgduan[(30 - Second) % 100 / 10];
            DisplayData[3] = smgduan[(30 - Second) % 10];
            DisplayData[4] = 0x00;
            DisplayData[5] = 0x00;
            DisplayData[6] = DisplayData[2];
            DisplayData[7] = DisplayData[3];
            DigDisplay();
            P1 = 0xFF;//将所有的灯熄灭
            RED00 = 1;
            GREEN00 = 1;
            GREEN11 = 0;//南北绿灯亮
            GREEN10 = 0;//上人行道绿灯亮
            RED01 = 0;//东西红灯亮
            RED00 = 0;//右人行道红灯亮
            if(up == 1)
            {
                k = 1;
                pd = 1;
            }
            else k = 0;
            if(Second > 30)//如果人流量多，再次倒计时30s
            {
                if(k == 0 && pd)
                {
                    Second = 1;
                    k = 1;
                    pd = 0;
                }
                else
                {
                    Second = 31;
                    pd = 1; 
                }
            }
        }
        //黄灯等待切换状态，5s
        else if(Second < 36)
        {
            DisplayData[0] = 0x00;
            DisplayData[1] = 0x00;
            DisplayData[2] = smgduan[(35 - Second) % 100 / 10];
            DisplayData[3] = smgduan[(35 - Second) % 10];
            DisplayData[4] = 0x00;
            DisplayData[5] = 0x00;
            DisplayData[6] = DisplayData[2];
            DisplayData[7] = DisplayData[3];
            DigDisplay();
            //黄灯阶段
            P1 = 0xFF;//将所有的灯熄灭
            RED00 = 1;
            GREEN00 = 1;
            YELLOW11 = 0;//南北黄灯亮
            RED10 = 0;//上人行道红灯亮
            YELLOW01 = 0;//东西红灯亮
            RED00 = 0;//右人行道红灯亮
        }
        //东西方向通行
        else if(Second < 66)
        {
            DisplayData[0] = 0x00;
            DisplayData[1] = 0x00;
            DisplayData[2] = smgduan[(65 - Second) % 100 / 10];
            DisplayData[3] = smgduan[(65 - Second) % 10];
            DisplayData[4] = 0x00;
            DisplayData[5] = 0x00;
            DisplayData[6] = DisplayData[2];
            DisplayData[7] = DisplayData[3];
            DigDisplay();
            P1 = 0xFF;
            RED00 = 1;
            GREEN00 = 1;
            RED11 = 0;
            RED10 = 0;
            GREEN01 = 0;
            GREEN00 = 0;
            if(right == 1)
            {
                l = 1;
                we = 1;
            }
            else l = 0;
            if(Second > 65)
            {
                if(l == 0 && we)
                {
                    Second = 36;
                    l = 1;
                    we = 0;
                }
                else
                {
                    Second = 66;
                    we = 1; 
                }
            }
        }
        else
        {
            DisplayData[0] = 0x00;
            DisplayData[1] = 0x00;
            DisplayData[2] = smgduan[(70 - Second) % 100 / 10];
            DisplayData[3] = smgduan[(70 - Second) % 10];
            DisplayData[4] = 0x00;
            DisplayData[5] = 0x00;
            DisplayData[6] = DisplayData[2];
            DisplayData[7] = DisplayData[3];
            DigDisplay();
            P1 = 0xFF;
            RED00 = 1;
            GREEN00 = 1;
            YELLOW11 = 0;
            RED10 = 0;
            YELLOW01 = 0;
            RED00 = 0;
        }
    }
}
void Time1() interrupt 3
{
    static uchar i;
    TH1 = (65536 - 50000) / 256; //设置定时初值
    TL1 = (65536 - 50000) % 256;
    i ++;
    if (i == 20)
    {
        i = 0;
        Second ++;
    }
}