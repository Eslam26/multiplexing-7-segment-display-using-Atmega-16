/*
 * Project_1.c
 *
 *  Created on: Aug 27, 2019
 *      Author: eslamelnaggar
 */

#include "micro_config.h"
#include "timer1.h"


void System_init(void);
void Interrupts_init(void);
void callBackFunction(void);


unsigned char g_sec_tick  = 0;
unsigned char g_min_tick  = 0;
unsigned char g_hour_tick = 0;
unsigned char g_flag = 0;
unsigned short g_temp=0;
ISR(INT0_vect)
{
	g_sec_tick  = 0;
	g_min_tick  = 0;
	g_hour_tick = 0;
	TCNT1 = 0;
}

ISR(INT1_vect)
{
	if(g_flag)
	{
		TCNT1 = g_temp;
		g_flag = 0;
		TIMSK ^= (1<<4);
	}
	else
	{
		TIMSK ^= (1<<4);
		g_temp = TCNT1;
		g_flag = 1;
	}



}

int main()
{
	unsigned char i=0;
	Timer1_configType configType = {CTC_OCR1A, F_CPU_64, 0};
	Timer1_compare_1A configTypecompare = {0, compareMode_1A, 15625};

	System_init();
	Interrupts_init();
	while(i<6)
	{
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| (0 & 0x0F);
		i++;
		_delay_ms(5);
	}
	Timer1_setCallBack(callBackFunction);
	Timer1_init(&configType);
	Timer1_compareMode_OCR1A(&configTypecompare);
	while(1)
	{
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_sec_tick%10) & 0x0F);
		i++;
		_delay_ms(6);
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_sec_tick/10) & 0x0F);
		i++;
		_delay_ms(6);
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_min_tick%10) & 0x0F);
		i++;
		_delay_ms(6);
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_min_tick/10) & 0x0F);
		i++;
		_delay_ms(6);
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_hour_tick%10) & 0x0F);
		i++;
		_delay_ms(6);
		PORTB = (PORTB &0xC0)| ~(1<<i);
		PORTC = (PORTC &0xF0)| ((g_hour_tick/10) & 0x0F);
		i=0;
		_delay_ms(6);

	}


}
void System_init(void)
{
	/* Enable pins init */
	DDRB |= 0x3F;
	PORTB &= ~(0x3F);

	/* 7-seg pins init */
	DDRC |= 0x0F;
	PORTC = (PORTC &0xF0)| (0 & 0x0F);

	/* Reset button pin init */
	DDRD &= ~(1<<2);
	PORTD |= (1<<2);

	/* Pause button pin init */
	DDRD &= ~(1<<3);
	PORTD |= (1<<3);
}

void Interrupts_init(void)
{

	SREG = SREG | (1<<7);
	GICR = (1<<7) | (1<<6);
	MCUCR = (1<<1) | (1<<3);

}
/*
void Timer1_init(void)
{
	TCCR1B = (1<<0) | (1<<1) | (1<<3);
	TIMSK = (1<<4);
	TCNT1 = 0;
	OCR1A = 15625;

*/
	/*
	 ------------ Calculations ------------
	 Ftimer = 1 Mhz / 64;
	 Tpertick = 1/Ftimer;
	 ticks to get 1 second interrupt => 1/Tpertick = 15625
	 */
//}


void callBackFunction(void){
	g_sec_tick++;
		if(g_sec_tick == 60)
		{
			g_sec_tick=0;
			g_min_tick++;
		}
		if(g_min_tick ==60)
		{
			g_min_tick=0;
			g_hour_tick++;
		}
		if(g_hour_tick==25)
		{
			g_hour_tick=0;
		}
}
