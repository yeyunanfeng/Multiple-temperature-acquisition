#include <reg52.h> 
#include <string.h>	
#include "stdio.h"
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char   	//宏定义
#define LCD1602 P0
sbit SET=P2^5;    			//定义调整键
sbit DEC=P3^2;    			//定义减少键
sbit ADD=P3^3;    			//定义增加键
sbit BUZZ=P2^4;    			//定义蜂鸣器
sbit ALAM=P1^2;				//定义灯光报警
sbit ALAM1=P1^4;
sbit DQ=P2^0;     			//定义四个DS18B20温度传感器	
sbit DQ1=P2^1;
sbit DQ2=P2^2;
sbit DQ3=P2^3;
sbit RS = P2^7;
sbit EN = P2^6;
sbit led1 = P3^4;                                //定义四个LED灯
sbit led2 = P3^5;
sbit led3 = P3^6;
sbit led4 = P3^7;
sbit KEY1 = P1^1;  
sbit send = P1^0;
bit shanshuo_st;    			//闪烁间隔标志
bit beep_st;     				//蜂鸣器间隔标志
uchar x=0;      				//计数器
char Time[] = __TIME__;
void UsartInit();
void SendStr(char * str);

uchar code tab1[]={"T:  .  C    .  C"};
uchar code tab2[]={"  .  C      .  C"};
uchar code tab3[]={"TH:   C  TL:   C"};
uint c,c1,c2,c3,c4;
uchar Mode=0;     			//状态标志
signed char TH=40;  		//上限报警温度，默认值为40
signed char TL=10;   		//下限报警温度，默认值为10
//============================================================================================
//====================================DS18B20=================================================
//============================================================================================
/*****延时子程序*****/
void Delay_DS18B20(int num)
{
  while(num--) ;
}
void delay(uint xms)//延时函数，有参函数
{
	uint x,y;
	for(x=xms;x>0;x--)
	 for(y=110;y>0;y--);
}
/*****初始化DS18B20 温度传感器1*****/
void Init_DS18B20(void)
{
  unsigned char x=0;
  DQ = 1;         //DQ复位
  Delay_DS18B20(8);    //稍做延时
  DQ = 0;         //单片机将DQ拉低
  Delay_DS18B20(80);   //精确延时，大于480us
  DQ = 1;         //拉高总线
  Delay_DS18B20(14);
  x = DQ;           //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
  Delay_DS18B20(20);
}

/*****读一个字节*****/
unsigned char ReadOneChar(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ = 0;     // 给脉冲信号
    dat>>=1;
    DQ = 1;     // 给脉冲信号
    if(DQ)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
/*****写一个字节*****/
void WriteOneChar(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay_DS18B20(5);
    DQ = 1;
    dat>>=1;
  }
}
/*****读取温度*****/
unsigned int ReadTemperature(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init_DS18B20();
  WriteOneChar(0xCC);  //跳过读序号列号的操作
  WriteOneChar(0x44);  //启动温度转换
  Init_DS18B20();
  WriteOneChar(0xCC);  //跳过读序号列号的操作
  WriteOneChar(0xBE);  //读取温度寄存器
  a=ReadOneChar();     //读低8位
  b=ReadOneChar();    //读高8位
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //放大10倍输出并四舍五入
  t= tt*10+0.5; 
  return(t);
}


/*****初始化DS18B20  温度传感器2*****/
void Init1_DS18B20(void)
{
  unsigned char x=0;
  DQ1 = 1;         //DQ复位
  Delay_DS18B20(8);    //稍做延时
  DQ1 = 0;         //单片机将DQ拉低
  Delay_DS18B20(80);   //精确延时，大于480us
  DQ1 = 1;         //拉高总线
  Delay_DS18B20(14);
  x = DQ1;           //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
  Delay_DS18B20(20);
}

unsigned char ReadOneChar1(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ1 = 0;     // 给脉冲信号
    dat>>=1;
    DQ1 = 1;     // 给脉冲信号
    if(DQ1)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar1(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ1 = 0;
    DQ1 = dat&0x01;
    Delay_DS18B20(5);
    DQ1 = 1;
    dat>>=1;
  }
}

/*****读取温度*****/
unsigned int ReadTemperature1(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init1_DS18B20();
  WriteOneChar1(0xCC);  //跳过读序号列号的操作
  WriteOneChar1(0x44);  //启动温度转换
  Init1_DS18B20();
  WriteOneChar1(0xCC);  //跳过读序号列号的操作
  WriteOneChar1(0xBE);  //读取温度寄存器
  a=ReadOneChar1();     //读低8位
  b=ReadOneChar1();    //读高8位
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //放大10倍输出并四舍五入
  t= tt*10+0.5; 
  return(t);
}


/*****初始化DS18B20  温度传感器3*****/
void Init2_DS18B20(void)
{
  unsigned char x=0;
  DQ2 = 1;         //DQ复位
  Delay_DS18B20(8);    //稍做延时
  DQ2 = 0;         //单片机将DQ拉低
  Delay_DS18B20(80);   //精确延时，大于480us
  DQ2 = 1;         //拉高总线
  Delay_DS18B20(14);
  x = DQ2;           //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
  Delay_DS18B20(20);
}

unsigned char ReadOneChar2(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ2 = 0;     // 给脉冲信号
    dat>>=1;
    DQ2 = 1;     // 给脉冲信号
    if(DQ2)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar2(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ2 = 0;
    DQ2 = dat&0x01;
    Delay_DS18B20(5);
    DQ2 = 1;
    dat>>=1;
  }
}

/*****读取温度*****/
unsigned int ReadTemperature2(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init2_DS18B20();
  WriteOneChar2(0xCC);  //跳过读序号列号的操作
  WriteOneChar2(0x44);  //启动温度转换
  Init2_DS18B20();
  WriteOneChar2(0xCC);  //跳过读序号列号的操作
  WriteOneChar2(0xBE);  //读取温度寄存器
  a=ReadOneChar2();     //读低8位
  b=ReadOneChar2();    //读高8位
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //放大10倍输出并四舍五入
  t= tt*10+0.5; 
  return(t);
}

/*****初始化DS18B20  温度传感器4*****/
void Init3_DS18B20(void)
{
  unsigned char x=0;
  DQ3	= 1;         //DQ复位
  Delay_DS18B20(8);    //稍做延时
  DQ3 = 0;         //单片机将DQ拉低
  Delay_DS18B20(80);   //精确延时，大于480us
  DQ3 = 1;         //拉高总线
  Delay_DS18B20(14);
  x = DQ3;           //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
  Delay_DS18B20(20);
}

unsigned char ReadOneChar3(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ3 = 0;     // 给脉冲信号
    dat>>=1;
    DQ3 = 1;     // 给脉冲信号
    if(DQ3)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar3(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ3 = 0;
    DQ3 = dat&0x01;
    Delay_DS18B20(5);
    DQ3 = 1;
    dat>>=1;
  }
}

/*****读取温度*****/
unsigned int ReadTemperature3(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init3_DS18B20();
  WriteOneChar3(0xCC);  //跳过读序号列号的操作
  WriteOneChar3(0x44);  //启动温度转换
  Init3_DS18B20();
  WriteOneChar3(0xCC);  //跳过读序号列号的操作
  WriteOneChar3(0xBE);  //读取温度寄存器
  a=ReadOneChar3();     //读低8位
  b=ReadOneChar3();    //读高8位
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //放大10倍输出并四舍五入
  t= tt*10+0.5; 
  return(t);
}

/*****读取温度*****/
void check_wendu(void)
{
	c=ReadTemperature()-5;  			//获取温度值并减去DS18B20的温漂误差
	if(c>1200)
	c=1200;
	c1=ReadTemperature1()-5;  			//获取温度值并减去DS18B20的温漂误差
	if(c1>1200)
	c1=1200;
	c2=ReadTemperature2()-5;  			//获取温度值并减去DS18B20的温漂误差
	if(c2>1200)
	c2=1200;
	c3=ReadTemperature3()-5;  			//获取温度值并减去DS18B20的温漂误差
	if(c3>1200)
	c3=1200;
	c4=(c1+c2+c+c3)/4;
}



/********液晶写入指令函数与写入数据函数，以后可调用**************/

void write_1602com(uchar com)//****液晶写入指令函数****
{
	RS=0;//数据/指令选择置为指令
//	rw=0; //读写选择置为写
	LCD1602=com;//送入数据
	delay(1);
	EN=1;//拉高使能端，为制造有效的下降沿做准备
	delay(1);
	EN=0;//en由高变低，产生下降沿，液晶执行命令
}


void write_1602dat(uchar dat)//***液晶写入数据函数****
{
	RS=1;//数据/指令选择置为数据
//	rw=0; //读写选择置为写
	LCD1602=dat;//送入数据
	delay(1);
	EN=1; //en置高电平，为制造下降沿做准备
	delay(1);
	EN=0; //en由高变低，产生下降沿，液晶执行命令
}


void lcd_init()//***液晶初始化函数****
{
	uchar a;
	write_1602com(0x38);//设置液晶工作模式，意思：16*2行显示，5*7点阵，8位数据
	write_1602com(0x0c);//开显示不显示光标
	write_1602com(0x06);//整屏不移动，光标自动右移
	write_1602com(0x01);//清显示

	write_1602com(0x80);//日历显示固定符号从第一行第1个位置之后开始显示
	for(a=0;a<16;a++)
	{
		write_1602dat(tab1[a]);//向液晶屏写日历显示的固定符号部分
		delay(3);
	}
	write_1602com(0x80+0x40);//时间显示固定符号写入位置，从第2个位置后开始显示
	for(a=0;a<16;a++)
	{
		write_1602dat(tab2[a]);//写显示时间固定符号，两个冒号
		delay(3);
	}

}

void display()//温度值显示
{
	if(Mode==0)
	{
		write_1602com(0x80+2);
		write_1602dat((c%1000)/100+0x30);
		write_1602dat(((c%1000)%100)/10+0x30);
		write_1602com(0x80+5);
		write_1602dat(((c%1000)%100)%10+0x30);
		write_1602com(0x80+6);
		write_1602dat(0xdf);
		write_1602com(0x80+10);
		write_1602dat((c1%1000)/100+0x30);
		write_1602dat(((c1%1000)%100)/10+0x30);
		write_1602com(0x80+13);
		write_1602dat(((c1%1000)%100)%10+0x30);
		write_1602com(0x80+14);
	  write_1602dat(0xdf);
		write_1602com(0x80+0x40);
		write_1602dat((c2%1000)/100+0x30);
		write_1602dat(((c2%1000)%100)/10+0x30);
		write_1602com(0x80+0x40+3);
		write_1602dat(((c2%1000)%100)%10+0x30);
		write_1602com(0x80+0x40+4);
	  write_1602dat(0xdf);
		write_1602com(0x80+0x40+10);
		write_1602dat((c3%1000)/100+0x30);
		write_1602dat(((c3%1000)%100)/10+0x30);
		write_1602com(0x80+0x40+13);
		write_1602dat(((c3%1000)%100)%10+0x30);
		write_1602com(0x80+0x40+14);
	  write_1602dat(0xdf);	
	}
}
//=====================================================================================

/*****初始化定时器0*****/
void InitTimer(void)
{
	TMOD=0x1;
	TH0=0x3c;
	TL0=0xb0;     //50ms（晶振12M）
	EA=1;      //全局中断开关
	TR0=1;
	ET0=1;      //开启定时器0
}

void KEY()//按键扫描
{
	uchar a;
			//功能键
	if(SET==0)
	{	
		write_1602com(0x01);
		write_1602com(0x80+0x40);//时间显示固定符号写入位置，从第2个位置后开始显示
		for(a=0;a<16;a++)
		{
			write_1602dat(tab3[a]);//写显示时间固定符号，两个冒号
			delay(3);
		}	
		BUZZ=0;
		delay(10);
		if(SET==0)
		{
			Mode++;
			if(Mode==3)
			Mode=0;	
			BUZZ=1;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
		}
		while(SET==0)
		{
			if(Mode==0)
				{
				//	write_1602com(0x80+0x40+6);
					write_1602com(0x0c);
					lcd_init();										// 液晶显示为检测界面的
			
				}	
			else if(Mode==1)
				{
					write_1602com(0x80+0x40+3);
					write_1602com(0x0f);
				}	
			else
				{
					write_1602com(0x80+0x40+12);
					write_1602com(0x0f);
				}							
		}
	}
	//增加
	if(ADD==0&&Mode==1)
	{
		BUZZ=0;
		delay(10);
		if(ADD==0)	
		{
			TH++;
			if(TH>=99)	
			TH=99;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(ADD==0);
		
	}
	//减少
	if(DEC==0&&Mode==1)
	{
		BUZZ=0;
		delay(10);
		if(DEC==0)
		{
			TH--;
			if(TH==TL)	
			TH=TL+1;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(DEC==0);
	}
	if(ADD==0&&Mode==2)
	{
		BUZZ=0;
		delay(10);
		if(ADD==0)	
		{
			TL++;
			if(TL==TH)	
			TL=TH-1;
			BUZZ=1;
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
		}	
		while(ADD==0);
		
	}
	//减少
	if(DEC==0&&Mode==2)
	{
		BUZZ=0;
		delay(10);
		if(DEC==0)
		{
			TL--;
			if(TL<=0)	
			TL=0;	
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(DEC==0);		
	}
}

/*****报警子程序*****/
void Alarm()
{
	if(x>=10){beep_st=~beep_st;x=0;}
	if(Mode==0)
	{	
		led1=0;
		led2=0;
		led3=0;
		led4=0;
		if((c/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led1=1;
			SendStr("警告！！！温度1异常 \r\n");
		}
		if((c1/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led2=1;
			SendStr("警告！！！温度2异常 \r\n");
		}
		if((c2/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led3=1;
			SendStr("警告！！！温度3异常 \r\n");
		}
		if((c3/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led4=1;
			SendStr("警告！！！温度4异常 \r\n");
		}

		if((c/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led1=1;
			SendStr("警告！！！温度1异常 \r\n");
		}
		if((c1/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led2=1;
			SendStr("警告！！！温度2异常 \r\n");
		}
		if((c2/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led3=1;
			SendStr("警告！！！温度3异常 \r\n");
		}
		if((c3/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led4=1;
			SendStr("警告！！！温度4异常 \r\n");
		}
		
		if((c4/10)>=TH)
		{
			ALAM=0;
			ALAM1=1;
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			SendStr("警告！！！温度异常 \r\n");
		}
		else if((c4/10)<TL)
		{
			ALAM1=0;
			ALAM=1;
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			SendStr("警告！！！温度异常 \r\n");
		}
		else
		{
			BUZZ=1;
			ALAM=1;
			ALAM1=1;	
		}
	}
	else
	{
		BUZZ=1;
		ALAM=1;
		ALAM1=1;
	}
}

/*****时间函数*****/
void time(char *time){
	if(time[3]<'6'){
		if(time[4]<'9'){
			if(time[6]<'6'){
				if(time[7]<'9')
					time[7]=time[7]+1;
				else{
					time[7]='0';
					time[6]=time[6]+1;
				}
			}
				else{
					time[6]='0';
					time[4]=time[4]+1;
				}
			}
		else{
			time[4]='0';
			time[3]=time[3]+1;
		}
	}
	else{
		time[3]='0';
		time[2]=time[2]+1;
	}
					
}

/*****基础信息函数*****/
void BasicInfo(void){
			char string[50];
			sprintf(string,"时间:%s %s \r\n",__DATE__,Time);
			SendStr(string);  //串口发送
			sprintf(string,"温度1:%2.1f°C \r\n",c*1.0/10);
			SendStr(string);  //串口发送
			sprintf(string,"温度2:%2.1f°C \r\n",c1*1.0/10);
			SendStr(string);  //串口发送
			sprintf(string,"温度3:%2.1f°C \r\n",c2*1.0/10);
			SendStr(string);  //串口发送
			sprintf(string,"温度4:%2.1f°C \r\n",c3*1.0/10);
			SendStr(string);  //串口发送
			sprintf(string,"平均温度:%2.1f°C \r\n",c4*1.0/10);
			SendStr(string);  //串口发送
}

/*****主函数*****/
void main(void)
{
	uint z;
	delay(1);
	lcd_init();
	delay(1);
	InitTimer();    //初始化定时器
	
	for(z=0;z<100;z++)
	{
		check_wendu();
		delay(1);        
	} 
	UsartInit();
	while(1)
	{
		display();
		KEY();
		Alarm(); 
		check_wendu();
		time(Time);
		if(send==0&&Time[3]!='6'&&Time[6]!='6')
		{
			BasicInfo();
		}
		if(KEY1==0)
		{
			delay(10);
			if(KEY1==0)
			{
				BUZZ=0;
			}
		}
		else
		{
			BUZZ=1;
		}
	}
}

void UsartInit( void ) //串口中断
{
	SCON = 0X50;		//设置为工作方式1
	TMOD = 0X20;		//设置计数器工作方式2
	PCON &= 0XEF;		//波特率加倍
	TH1  = 0XFD;		//计数器初始值设置，注意波特率是9600的
	TL1  = 0XFD;
	
	IE = 0X00;
	TR1  = 1;			//打开计数器
}

/*******************************************************************************
* 函 数 名		: SendStr
* 函数功能		: 发送字符串
* 输	入			: 字符串
* 输	出			: 无
*******************************************************************************/
void SendStr(char * str)//串口发送函数
{
	
	while(*str != '\0')
	{
		SBUF = *str;	//发送一个字符
		while(!TI);		//等待串口发送完成，发送完成后 TI 自动置1
		TI = 0;				//TI 置0
		str++;				//指针加一
	}
}