#结构
#目标1：依赖1 依赖2
#	命令1
#	命令2
#目标2：依赖1 依赖2
#	命令1
#	命令2
#......

#比如可以这样简单的编译.c和.h放在一个文件夹内的工程
#OLED:main.c oled_app.c
#	gcc -o OLED main.c oled_app.c -I .

#更大的工程（如本工程得这样）

#定义变量 OLED是最终的目标文件名
TARGET = OLED
#存放编译文件的路径
BUILD_DIR = build
#存放源文件的文件夹
SRC_DIR = sources
#存放头文件的文件夹 路径为includes和当前文件夹
INC_DIR = includes .
#源文件 定义了变量SRCS用于存储所有需要编译的源文件，它的值为wildcard函数的输出，这里为SRC_DIR中的所有c文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
#目标文件（*.o）  定义了OBJS变量用于存储所有要生成的的.o文件，它的值为patsubst函数的输出
#本例子中该函数是把所有c文件名替换为同名的.o文件，并放在添加的build目录中
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
#头文件 定义一个DEPS变量存储所有依赖的头文件，它的值为wildcard函数的输出，这里为INC_DIR中的所有h文件
DEPS = $(wildcard $(INC_DIR)/*.h)
#指定头文件的路径 CFLAGS变量，用于存储包含的头文件路径 它的值为patsubst函数的输出
#本例子中该函数是把includes目录添加到“-I”后面，函数的输出为“-Iincludes”
CFLAGS = $(patsubst %, -I%, $(INC_DIR))
#目标文件 相对于之前的Makefile，我们在$(TARGET)前增加了$(BUILD_DIR)路径，使得最终的可执行程序放在build目录下
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
#*.o文件的生成规则 与上面类似，给.o目标文件添加$(BUILD_DIR)路径。
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
#创建一个编译目录，用于存放过程文件
#命令前带“@”,表示不在终端上输出
#在执行编译前先创建build目录，以存放后面的.o文件，命令前的“@”表示执行该命令时不在终端上输出
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)
#伪目标
.PHONY: clean cleanall

#删除输出文件夹
clean:
	rm -rf $(BUILD_DIR)
      
#全部删除
cleanall:
	rm -rf $(BUILD_DIR)