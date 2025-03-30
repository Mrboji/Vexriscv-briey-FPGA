#ifndef UART_H_
#define UART_H_

#include <stdarg.h>

typedef struct
{
  volatile uint32_t DATA;
  volatile uint32_t STATUS;
  volatile uint32_t CLOCK_DIVIDER;
  volatile uint32_t FRAME_CONFIG;
} Uart_Reg;

enum UartParity {NONE = 0,EVEN = 1,ODD = 2};
enum UartStop {ONE = 0,TWO = 1};

typedef struct {
	uint32_t dataLength;
	enum UartParity parity;
	enum UartStop stop;
	uint32_t clockDivider;
} Uart_Config;

static uint32_t uart_writeAvailability(Uart_Reg *reg){
	return (reg->STATUS >> 16) & 0xFF;
}
static uint32_t uart_readOccupancy(Uart_Reg *reg){
	return reg->STATUS >> 24;
}

static void uart_write(Uart_Reg *reg, uint32_t data){
	while(uart_writeAvailability(reg) == 0);
	reg->DATA = data;
}

static void uart_applyConfig(Uart_Reg *reg, Uart_Config *config){
	reg->CLOCK_DIVIDER = config->clockDivider;
	reg->FRAME_CONFIG = ((config->dataLength-1) << 0) | (config->parity << 8) | (config->stop << 16);
}


static void myputchar(char ch){
	uart_write((Uart_Reg*)(0xF0010000),ch);

}


/* 功能函数声明 */
static void myPrintf(char *s, ...);				// 需要实现的目标函数
static void printNum(unsigned long num, int base); // 通用数字打印函数
static void printDeci(int dec);					// 打印十进制数
static void printOct(unsigned oct);				// 打印八进制数
static void printHex(unsigned hex);				// 打印十六进制数
static void printAddr(unsigned long addr);			// 打印地址
static void printStr(char *str);					// 打印字符串


/*
 * 函数名: myPrintf
 * 函数功能: 打印格式字符串
 * 参数: 1. 包含格式符的字符串地址 2.可变参
 * 返回值: 无
*/
static void myPrintf(char *s, ...)
{
    int i = 0;

	/* 可变参第一步 */
    va_list va_ptr;

	/* 可变参第二部 */
    va_start(va_ptr, s);

	/* 循环打印所有格式字符串 */
    while (s[i] != '\0')
    {
		/* 普通字符正常打印 */
		if (s[i] != '%')
		{
    	    myputchar(s[i++]);
			continue;
		}
		
		/* 格式字符特殊处理 */
		switch (s[++i])   // i先++是为了取'%'后面的格式字符
		{
		    /* 根据格式字符的不同来调用不同的函数 */
			case 'd': printDeci(va_arg(va_ptr,int));           
			  		  break; 
		    case 'o': printOct(va_arg(va_ptr,unsigned int));  
			  		  break;
		    case 'x': printHex(va_arg(va_ptr,unsigned int));  
			  		  break;
		    case 'c': myputchar(va_arg(va_ptr,int));            
			  		  break;
		    case 'p': printAddr(va_arg(va_ptr,unsigned long));
			  		  break;
		    case 's': printStr(va_arg(va_ptr,char *));
					  break;
			default : break;
		}
		i++; // 下一个字符
    }
	/* 可变参最后一步 */
    va_end(va_ptr);
}

/*
 * 函数名: printNum()
 * 函数功能: 通用数字打印函数可以把整型值打印成
 *           10进制数,8进制数,2进制数,16进制数
 * 参数: 1.需要打印的整数,无符号长整型是为了兼容
 *         地址格式打印; 2.打印的进制
 *  返回值: 无
*/
static void printNum(unsigned long num, int base)
{
    /* 递归结束条件 */
	if (num == 0)
    {
        return;
    }
    
    /* 继续递归 */
	printNum(num/base, base);

	/* 逆序打印结果 */
    myputchar("0123456789abcdef"[num%base]);
}


/*
 * 函数名: printDeci
 * 函数功能: 打印十进制数
 * 参数: 十进制整数
 * 返回值: 无
*/
static void printDeci(int dec)
{
    int num;

    /* 处理有符号整数为负数时的情况 */
	if (dec < 0)
    {
        myputchar('-');
		dec = -dec;  	   // 该操作存在溢出风险:最小的负数没有对应的正数
    }

    /* 处理整数为时0的情况 */
    if (dec == 0)
    {
        myputchar('0');
		return;
    }
    else
    {
        printNum(dec, 10); // 打印十进制数
    }
}

/*
 * 函数名: printOct
 * 函数功能: 打印八进制整数
 * 参数: 无符号整数
 * 返回值: 无
*/
static void printOct(unsigned oct)
{
    if (oct == 0)			// 处理整数为0的情况
    {
		myputchar('0');
		return;
    }
    else
    {
        printNum(oct,8);	// 打印8进制数
    }
}

/*
 * 函数名: printHex
 * 函数功能: 打印十六进制整数
 * 参数: 无符号整数
 * 返回值: 无
*/
static void printHex(unsigned hex)
{
    if (hex == 0)			// 处理整数为0的情况
    {
        myputchar('0');
		return;
    }
    else
    {
        printNum(hex,16);	// 打印十六进制数
    }
}

/*
 * 函数名: printAddr
 * 函数功能: 打印地址
 * 参数: 待打印的地址
 * 返回值: 无
*/
static void printAddr(unsigned long addr)
{
    /* 打印前导"0x" */
	myputchar('0');
    myputchar('x');

	/* 打印地址:格式和十六进制一样 */
    printNum(addr, 16);
}

/*
 * 函数名: printStr
 * 函数功能: 打印字符串
 * 参数: 字符串地址
 * 返回值: 无
*/
static void printStr(char *str){
	while(*str){
		uart_write((Uart_Reg*)(0xF0010000),*(str++));
	}
}

#endif /* UART_H_ */


