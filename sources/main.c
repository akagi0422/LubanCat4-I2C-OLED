#include "main.h"
#include "oled_app.h"

int fd;

void main()
{
    int i = 1;  //用于循环

    fd = open("/dev/i2c-5", O_RDWR, 0);    //以读写模式打开i2c-5并将文件描述符存在fd中

    if (fd < 0)
    {
        perror("Can't open /dev/i2c-5 \n"); //i2c打开失败
        exit(1);
    }
    else
    {
        printf("Open /dev/i2c-5 successful \n");
    }

    OLED_Start(fd, Address);    //初始化OLED
    printf("OLED start successful \n");

    usleep(1000);   //延时1000us

    OLED_Fill(Address,0xff);  //充填
    printf("OLED fill successful \n");

    sleep(1);

    OLED_Fill(Address,0x00);  //清屏
    printf("OLED clean successful \n");

/*
    while (1)  //循环akagi想要头壳
    {
        OLED_ShowStr(Address,44,0,"Akagi");    //第一行
        OLED_ShowStr(Address,32,4,"19250422");    //第三行
        sleep(1);
        OLED_Fill(Address,0x00);  //清屏

        OLED_ShowChinese(Address, 0+16, 2, 4398); 
        OLED_ShowChinese(Address, 17+16, 2, 4615); 
        OLED_ShowChinese(Address, 33+16, 2, 2860); 
        OLED_ShowChinese(Address, 49+16, 2, 1423); 
        OLED_ShowChinese(Address, 65+16, 2, 4158); 
        OLED_ShowChinese(Address, 81+16, 2, 2858); 
        sleep(1);
        OLED_Fill(Address,0x00);  //清屏

        OLED_ShowStr(Address,44,0,"Akagi");    //第一行
        OLED_ShowStr(Address,32,4,"19250422");    //第三行
    }
*/

    //例程
    // OLED_ShowStr(Address,44,0,"Akagi");    //第一行
    // OLED_ShowStr(Address,32,2,"19250422");    //第二行
    // OLED_ShowStr(Address,32,4,"19250422");    //第三行
    // OLED_ShowStr(Address,32,4,"19250422");    //第四行
    // sleep(1);
    // OLED_ShowChinese(Address, 0, 6, 3766);  //骚哥屁股圆
    // OLED_ShowChinese(Address, 17, 6, 2232);
    // OLED_ShowChinese(Address, 33, 6, 3485);
    // OLED_ShowChinese(Address, 49, 6, 2296);
    // OLED_ShowChinese(Address, 65, 6, 4811);

    close(fd);
}
