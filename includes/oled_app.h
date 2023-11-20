#ifndef __OLED_APP_H_
#define __OLED_APP_H_

#define OLED_COMMEND_ADDR 0x00
#define OLED_DATA_ADDR 0x40

//static int i2c_write(int fd, u_int8_t addr,u_int8_t reg,u_int8_t val);

static int i2c_write(int fd, int Addr, int reg, u_int8_t valw);
static int i2c_read(int fd, int Addr, int reg, int* valr);
void OLED_Start(int fd, int Addr);
void OLED_Fill(int Addr,int fill_data);
void OLED_Set_Pos(int Addr, int x, int y);
void OLED_ShowStr(int Addr, int x, int y, unsigned char ch[]);
void OLED_ShowChinese(int Addr, int x, int y, int no);
#endif