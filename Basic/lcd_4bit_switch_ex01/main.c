/*
IN THE NAME OF ALLAH THE MOST BENEFICIAL THE MOST MERCIFUL


10/OCT/2013.......... 3:19 AM



lcd D7=Portc_pin7
lcd D6=PortC_pin6
lcd D5=PortC_pin5
lcd D4=PortC_pin4


lcd_en = PortC_pin3
lcd_rw = GND
lcd_rs = PortC_pin2



one digit counter

 */

#include <avr/io.h>
#include <util/delay.h>


#define LCD_PORT PORTC
#define LCD_DDR  DDRC
#define RS PC2
#define EN PC3



#define SW_UP   PA0
#define INPUT_DDR DDRA
#define INPUT_PORT PORTA
#define INPUT_PIN   PINA


#define LCD_CURSOR_OFF	0x0C
#define	LCD_CLEAR		0x01

#define	LINE_1			0x80
#define	LINE_2			0xC0
#define	LINE_3			0x94
#define	LINE_4			0x0D4
#define Delay_ms        _delay_ms


#define	SHIFT_CURSOR_RIGHT	0x06


//==============================================
void Lcd_init_4bit();
void Lcd_Cmd(unsigned char);
void Lcd_Chr_CP(unsigned char);
void strob();
void lcd_nibble(unsigned char);
void lcd_puts(const unsigned char*);
//-------------------------------------------



unsigned char digit1=0;

int main(void)
{

  INPUT_DDR &= ~_BV(SW_UP);  //MAKE SW_UP AN INPUT PIN BY MAKING IT ZERO
  INPUT_PORT |= _BV(SW_UP);     //ENABLE INTERNAL


  Lcd_init_4bit();
  lcd_puts("Hello AVR");
    while(1)
    {
        if((INPUT_PIN&_BV(SW_UP))==0)
        {
        Lcd_Cmd(LINE_2);

        digit1++;
        if(digit1>9)digit1=0;

        Lcd_Chr_CP(digit1+48);
        _delay_ms(300);

        }

     //very short delay
        asm volatile ("nop");
        asm volatile ("nop");


    }


    return 0;
}

//======================================================
void Lcd_init_4bit()
{

LCD_DDR=255;    // MAKE DATA DIRECTION REGISTER OUTPUT

LCD_PORT  &= ~( 1 << RS );  //CLEAR RS
LCD_PORT  &= ~( 1 << EN );  //CLEAR EN

Delay_ms(250);
lcd_nibble(40);
Lcd_Cmd(40);		//init. LCD 2 lines, 5x7 matrix
Lcd_Cmd(12);
Lcd_Cmd(0x0F);		//disp on cursor on, Blink
Delay_ms(250);

}

void strob()
{
	LCD_PORT  |= ( 1 << EN );  //SET EN
	Delay_ms(1);
	LCD_PORT  &= ~( 1 << EN );  //CLEAR EN
}

//======================================================================

void Lcd_Cmd(unsigned char value)
{

	LCD_PORT  &= ~( 1 << RS );  //CLEAR RS
	lcd_nibble(value);
	LCD_PORT = (LCD_PORT & 0x0F) | (value << 4);
	strob();
	Delay_ms(15)    ;

}

//======================================================================

void Lcd_Chr_CP(unsigned char value)
{
	LCD_PORT  |= ( 1 << RS );  //SET RS
	lcd_nibble(value);
	value = (value & 0x0F) |  (value << 4);
	lcd_nibble(value);
	Delay_ms(15);


}

//========================================================================
void lcd_nibble(unsigned char c)
{

LCD_PORT = (LCD_PORT & 0x0F) | (c & 0xF0); //in higher nibble 4-7
strob();

}

void lcd_puts(const unsigned char* str)
{
	while(*str)
			{
				Lcd_Chr_CP(*(str++));

			}


}
//==================================================================
