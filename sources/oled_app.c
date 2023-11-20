#include "main.h"
#include "oled_app.h"
#include "str.h"
#include "ch.h"

extern int fd;  //声明在其他地方存在已经定义好的变量fd

/*static int i2c_write(int fd, u_int8_t addr,u_int8_t reg,u_int8_t val)  //这是绝对好用的写入函数
{
    int retries;
    u_int8_t data[2];

    data[0] = reg;
    data[1] = val;

    //设置地址长度：0为7位地址
    ioctl(fd,I2C_TENBIT,0);

    //设置从机地址
    if (ioctl(fd,I2C_SLAVE,addr) < 0)
    {
        printf("fail to set i2c device slave address!\n");
        close(fd);
        return -1;
    }

    //设置收不到ACK时的重试次数
    ioctl(fd,I2C_RETRIES,5);

    if (write(fd, data, 2) == 2)
    {
        return 0;
    }
    else{
        return -1;
    }

}
*/

static int i2c_write(int fd, int Addr, int reg, u_int8_t valw)    //文件描述符，从机地址，寄存器地址，写入值（必须用u_int8_t格式）
{
    int retries = 2;  //重试次数
    u_int8_t wdata[2];    //发送数组

    wdata[0] = reg;    //将寄存器地址装入数组
    wdata[1] = valw;    //将数据装入数组

    //printf("Command data = 0x%X\n",wdata[1]);  //查看赋值是否正确

    ioctl(fd,I2C_TENBIT,0);    //设置地址长度为7位

    if(ioctl(fd,I2C_SLAVE,Addr) < 0)    //使用ioclt函数设置从机地址并判断是否设置成功
    {
        printf("Fail to set I2C slave address!\n");
        close(fd);
        return -1;
    }

    ioctl(fd, I2C_RETRIES, retries);    //设置收不到ack的重试次数

    if(write(fd, wdata, 2) == 2)    //文件描述符，写入的数组，写入的字节数--成功返回值为写入的字节数--失败返回值-1并设置errno
    {
        return 0;
    }                              //linux把i2c也视为文件，发送即写入文件
    else
    {
       return -1;
    }
}

static int i2c_read(int fd, int Addr, int reg, int* valr)    //文件描述符，从机地址，读出寄存器地址，读出值（可以写作int* valr代表valr是数组）
{                                                              //使用数组时在函数中不需要声明valr数组的大小，可在.h中定义
    int retries = 2;    //设置重试次数

    ioctl(fd,I2C_TENBIT,0);    //设置7位地址

    if(ioctl(fd,I2C_SLAVE,Addr) < 0)    //使用ioclt函数设置从机地址并判断是否设置成功
    {
        printf("Fail to set I2C slave address!\n");
        close(fd);
        return -1;
    }

    ioctl(fd,I2C_RETRIES,retries);    //设置收不到ack时的重试次数

    if(write(fd,&reg,1) == 1)    //&reg?写入打算读取的寄存器的地址
    {
        if(read(fd,valr,1) == 1)    //读取一字节并存入valr中
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

void OLED_Start(int fd, int Addr)    //OLED初始化
{
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x11);i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x3F);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xD3);i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x00);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x40);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xA1);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xC8);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xDA);i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x12);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x81);i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xFF);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xA4);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xA6);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xD5);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x80);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x8D);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0x14);
    i2c_write(fd, Addr, OLED_COMMEND_ADDR, 0xAF);
}

void OLED_Fill(int Addr,int fill_data)    //全屏充填
{
    int m = 0;
    int n = 0;
    for(m = 0;m < 8;m++)
    {
        i2c_write(fd,Addr,OLED_COMMEND_ADDR,0xb0 + m);    //page0-page7页寻址模式下起始页地址  这三行规定了从哪里开始充填屏幕
        i2c_write(fd,Addr,OLED_COMMEND_ADDR,0x00);        //页寻址模式下起始列地址的低四位
        i2c_write(fd,Addr,OLED_COMMEND_ADDR,0x10);        //页寻址模式下起始列地址的高四位

        for(n = 0;n < 128;n++)
        {
            i2c_write(fd, Addr, OLED_DATA_ADDR, fill_data);
            usleep(20);
        }
    }
}

void OLED_Set_Pos(int Addr, int x, int y)
{
    i2c_write(fd,Addr,OLED_COMMEND_ADDR,0xb0+y);    //page0-page7页寻址模式下起始页地址  这三行规定了从哪里开始充填屏幕
    i2c_write(fd,Addr,OLED_COMMEND_ADDR,(x&0x0f)|0x01);        //页寻址模式下起始列地址的低四位
    i2c_write(fd,Addr,OLED_COMMEND_ADDR,((x&0xf0)>>4)|0x10);        //页寻址模式下起始列地址的高四位  x&0xf0——取高四位的值；
}

void OLED_ShowStr(int Addr, int x, int y, unsigned char ch[])
{
    unsigned char c = 0,i = 0,j = 0;

    while(ch[j] != '\0')
	{
		c = ch[j];
		if(x > 120)  //一行写满就换行
		{
			x = 0;
			y = y+2;
		}

		OLED_Set_Pos(Addr,x,y);  //字母占两行，写字母的上半部分
		for(i=0;i<8;i++)
        {
            i2c_write(fd,Addr, OLED_DATA_ADDR,F8X16[c*16+i]);
        }

		OLED_Set_Pos(Addr,x,y+1);  //字母占两行，写字母的下半部分
		for(i=0;i<8;i++)
        {
		    i2c_write(fd, Addr,OLED_DATA_ADDR,F8X16[c*16+i+8]);
        }

        x += 8;
		j++;
	}
}

void OLED_ShowChinese(int Addr, int x, int y, int no)
{
    int i = 0;
    //int adder = 0;

    if(x > 113)  //一行写满就换行
	{
		x = 0;
		y = y+2;
	}

    OLED_Set_Pos(Addr,x,y);  //汉字占两行，写汉字上半部分
    for(i=0;i<16;i++)
	{
		i2c_write(fd,Addr, OLED_DATA_ADDR,F16X16[2*no][i]);
		//adder+=1;
    }

    OLED_Set_Pos(Addr,x,y+1);  //汉字占两行，写汉字下半部分
    for(i=0;i<16;i++)
	{
		i2c_write(fd,Addr, OLED_DATA_ADDR,F16X16[2*no+1][i]);
		//adder+=1;
    }
}