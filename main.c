/*
 * PIN Check.c
 *
 * Created: 7/18/2019 1:22:07 PM
 * Author : Ramiro
 */ 

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define buttonUpOne (~PINA&0x08)
#define buttonDownOne (~PINA&0x04)
#define buttonUpTwo (~PINA&0x02)
#define buttonDownTwo (~PINA&0x01)
#define buttonReset (~PINA&0x10)
#define AiSwitch (~PINA&0x20)

#include <avr/io.h>
#include <util/delay.h>
#define HC595_PORT   PORTB
#define HC595_DDR    DDRB
#define HC595_DS_POS PB0      //Data pin (DS) pin location
#define HC595_SH_CP_POS PB1      //Shift Clock (SH_CP) pin location 
#define HC595_ST_CP_POS PB2      //Store Clock (ST_CP) pin location

void HC595Init()
{HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));}

#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))
#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))

void HC595Pulse()
{
   HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH
   HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW
}

void HC595Latch()
{
   HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
   _delay_loop_1(1);
   HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
   _delay_loop_1(1);
}

void HC595Write(uint8_t data)
{
   for(uint8_t i=0;i<8;i++)
   {
      if(data & 0b10000000)
      {
         HC595DataHigh();
      }
      else
      {
         HC595DataLow();
      }

      HC595Pulse();
      data=data<<1; 

   }
   HC595Latch();
}

void Wait()
{
   for(uint8_t i=0;i<30;i++)
   {
      _delay_loop_2(0);
   }
}


#define HC595_PORT1   PORTB
#define HC595_DDR1    DDRB
#define HC595_DS_POS1 PB3      //Data pin (DS) pin location
#define HC595_SH_CP_POS1 PB4      //Shift Clock (SH_CP) pin location
#define HC595_ST_CP_POS1 PB5      //Store Clock (ST_CP) pin location

void HC595Init1()
{HC595_DDR1|=((1<<HC595_SH_CP_POS1)|(1<<HC595_ST_CP_POS1)|(1<<HC595_DS_POS1));}

#define HC595DataHigh1() (HC595_PORT1|=(1<<HC595_DS_POS1))
#define HC595DataLow1() (HC595_PORT1&=(~(1<<HC595_DS_POS1)))

void HC595Pulse1()
{
	HC595_PORT1|=(1<<HC595_SH_CP_POS1);//HIGH
	HC595_PORT1&=(~(1<<HC595_SH_CP_POS1));//LOW
}

void HC595Latch1()
{
	HC595_PORT1|=(1<<HC595_ST_CP_POS1);//HIGH
	_delay_loop_1(1);
	HC595_PORT1&=(~(1<<HC595_ST_CP_POS1));//LOW
	_delay_loop_1(1);
}

void HC595Write1(uint8_t data1)
{
	for(uint8_t i=0;i<8;i++)
	{
		if(data1 & 0b10000000)
		{
			HC595DataHigh1();
		}
		else
		{
			HC595DataLow1();
		}

		HC595Pulse1();
		data1=data1<<1;

	}
	HC595Latch1();
}

void Wait1()
{
	for(uint8_t i=0;i<30;i++)
	{
		_delay_loop_2(0);
	}
}



#include <avr/interrupt.h>
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M =1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 =0x02;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


unsigned char LeftPad=0x00;
unsigned char RightPad=0x00;
unsigned char ballcolumn=0x00;
unsigned char ballrow=0x00;
unsigned char column=0x00;
unsigned char row=0x00;
unsigned long cnt=0x00;
unsigned char tik=0x00;
unsigned char ballcnt=0x00;
unsigned char mode=0x00;
unsigned char balltime=0x00;
unsigned char LeftContact=0x00;
unsigned char LeftGate=0x00;
unsigned char RightContact=0x00;
unsigned char RightGate=0x00;
unsigned char PlayerTwoScore=0x00;
unsigned char PlayerOneScore=0x00;
unsigned char ballcnt2 = 0x00;
unsigned char ballcnt3 = 0x00;
unsigned long ballcnt4=0x00;
unsigned char Aimode=0x00;
unsigned long flashing=0x00;
unsigned long flashingoff=0x00;
unsigned long winner=0x00;
unsigned long displaytimer=0x00;
unsigned long displaytimertransition=0x00;
unsigned char displaytimeronoff=0x00;
unsigned char displaytimerscore=0x00;
unsigned char variable=0x00;
enum States{init, one, two, three}state;
enum stateCounter {initCounter, UpLeftCounter, DownLeftCounter, DownRightCounter, UpRightCounter} stateCounter;
enum stateClockwise {initClockwise, UpLeftClockwise, DownLeftClockwise, DownRightClockwise, UpRightClockwise } stateClockwise;

void Transitions(){
	switch(state){
		case init:
		state=one;
		break;
	
		case one:
		state=two;
		break;
	
		case two:
		state=three;
		break;
		
		case three:
		state=one;
		break;
		}
	switch(state){
		case init:
		break;
		
		case one:
		tik=0;
		break;
		
		case two:
		tik=1;
		break;
		
		case three:
		tik=2;
	}
}


	
void LeftScroll(){
if(cnt==1){
	if(buttonUpOne){
	if(LeftPad<0xE0){
		LeftPad=(LeftPad<<1);
	}
	}
	if(buttonDownOne){
	if(LeftPad>0x07){
		LeftPad=(LeftPad>>1);
	}
	}
}
}

void RightScroll(){
if(cnt==1){	
	if(buttonUpTwo){
	if(RightPad<0xE0){
		RightPad=(RightPad<<1);
	}
	}
	if(buttonDownTwo){
	if(RightPad>0x07){
		RightPad=(RightPad>>1);
	}
	}
}
}

void Padle(){
	LeftContact=(RightPad&ballrow);
	if(LeftContact>=1){
		LeftGate=0x01;
	}
	else {
		LeftGate=0x00;
	}
	
	RightContact=(LeftPad&ballrow);
	if(RightContact>=1){
		RightGate=0x01;
	}
	else{
		RightGate=0x00;
	}
}


void ballCounter(){
if(mode==1){
if(cnt==50){
ballcnt=ballcnt+1;
if(ballcnt==balltime){
		switch(stateCounter){
			case initCounter:
			stateCounter=UpLeftCounter;
			break;
				
			case UpLeftCounter:
			if(ballrow==0x01){
			stateCounter=DownLeftCounter;
			}
			break;
		
			case DownLeftCounter:
			if(ballcolumn==0x02){
			if(LeftGate==0x01){
			stateCounter=DownRightCounter;
			}
			}
			break;
				
			case DownRightCounter:
			if(ballrow==0x80){
			stateCounter=UpRightCounter;
			}
			break;
				
			case UpRightCounter:
			if(ballcolumn==0x40){
			if(RightGate==0x01){
			stateCounter=UpLeftCounter;
			}
			}
			break;		
		}
			
	switch(stateCounter){
			case initCounter:
			break;
					
			case UpLeftCounter:
			ballcolumn=ballcolumn>>1;
			ballrow=ballrow>>1;
			if(ballcolumn==0x02){
			if(LeftGate){
			stateClockwise=UpRightClockwise;
			mode=0;
			}
			}
			break;
					
			case UpRightCounter:
			ballcolumn=ballcolumn<<1;
			ballrow=ballrow>>1;
			if(ballrow==0x01){
			stateClockwise=DownRightCounter;
			mode=0;
			}
			variable=(LeftPad&ballrow);
			if(variable==0){
			if(ballrow>=LeftPad){
			variable=ballrow;
			variable=variable>>1;
			if(ballcolumn==0x40){
			if(((variable)<=LeftPad)){
			stateClockwise=DownLeftClockwise;
			mode=0;
			}
			}
			}
			}
			break;
				
			case DownLeftCounter:
			ballcolumn=ballcolumn>>1;
			ballrow=ballrow<<1;
			if(ballrow==0x80){
			stateClockwise=UpLeftClockwise;
			mode=0;
			}
			variable=(RightPad&ballrow);
			if(variable==0){
			if(ballrow<=RightPad){
				variable=RightPad;
				variable=variable>>4;
				if(ballcolumn==0x02){
				if(ballrow>=variable){
				stateClockwise=UpRightCounter;
				mode=0;
				}
				}
			}
			}
			break;
				
			case DownRightCounter:
			ballcolumn=ballcolumn<<1;
			ballrow=ballrow<<1;
			if(ballcolumn==0x40){
			if(RightGate){
			stateClockwise=DownLeftClockwise;
			mode=0;
			}
			}
			break;
		}
ballcnt=0;
}
}
}
}

void ballClockwise(){
if(mode==0){
if(cnt==50){
ballcnt=ballcnt+1;
if(ballcnt==balltime){
	switch(stateClockwise){
		case initClockwise:
		stateClockwise=UpLeftClockwise;
		break;
		
		case UpLeftClockwise:
		if(ballcolumn==0x02){
		if(LeftGate){
		stateClockwise=UpRightClockwise;
		}
		}
		break;
		
		case DownLeftClockwise:
		if(ballrow==0x80){
		stateClockwise=UpLeftClockwise;
		}
		break;
		
		case DownRightClockwise:
		if(ballcolumn==0x40){
		if(RightGate){
		stateClockwise=DownLeftClockwise;
		}
		}
		break;
		
		case UpRightClockwise:
		if(ballrow==0x01){
		stateClockwise=DownRightClockwise;
		}
		break;
	}
switch(stateClockwise){
		case initClockwise:
		break;
		
		case UpLeftClockwise:
		ballcolumn=ballcolumn>>1;
		ballrow=ballrow>>1;
		if(ballrow==0x01){
		stateCounter=DownLeftCounter;
		mode=1;
		}
		variable=(RightPad&ballrow);
		if(variable==0){
		if(ballrow>=RightPad){
		variable=ballrow;
		variable=variable>>1;
		if(ballcolumn==0x02){
		if(((variable)<=RightPad)){
		stateCounter=DownRightCounter;
		mode=1;
		}
		}
		}
		}
		break;
		
		case UpRightClockwise:
		ballcolumn=ballcolumn<<1;
		ballrow=ballrow>>1;
		if(ballcolumn==0x40){
		if(RightGate){
		stateCounter=UpLeftCounter;
		mode=1;
		}
		}
		break;
					
		case DownLeftClockwise:
		ballcolumn=ballcolumn>>1;
		ballrow=ballrow<<1;
		if(ballcolumn==0x02){
		if(LeftGate){
		stateCounter=DownRightCounter;
		mode=1;
		}
		}
		break;
					
		case DownRightClockwise:
		ballcolumn=ballcolumn<<1;
		ballrow=ballrow<<1;
		if(ballrow==0x80){
		stateCounter=UpRightCounter;
		mode=1;
		}
		variable=(LeftPad&ballrow);
		if(variable==0){
		if(ballrow<=LeftPad){
		variable=LeftPad;
		variable=variable>>4;
		if(ballcolumn==0x40){
		if(ballrow>=variable){
			stateCounter=UpLeftCounter;
			mode=1;
			}
			}
		}
		}
		break;
	
	}
	ballcnt=0;
}
}
}
}
	
void Adder(){
	if (tik==0){
		column=LeftPad;
		row=0x80;
	}
	if(tik==1){
		column=RightPad;
		row=0x01;
	}
	if(tik==2){
		row=ballcolumn;
		column=ballrow;
	}
	PORTC=~column;
	PORTD=row;
}

void resetgame(){

if(buttonReset){
while(buttonReset){
		DDRA = 0x00; PORTA = 0xFF;
		DDRB = 0xFF; PORTB = 0x00;
		DDRC = 0xFF; PORTC = 0x00;
		DDRD = 0xFF; PORTD = 0xFF;

	LeftPad=0x00;
	RightPad=0x00;
	ballcolumn=0x00;
	ballrow=0x00;
	column=0x00;
	row=0x00;
	cnt=0x00;
	tik=0x00;
	ballcnt=0x00;

	LeftPad=0x38;
	RightPad=0x38;
	ballcolumn=0x01;
	ballrow=0x01;
	HC595Write(0x00);
	HC595Write1(0x00);
	PlayerTwoScore=0;
	PlayerOneScore=0;
	winner=0;
	flashingoff=0;
	
	displaytimeronoff=0;
}
}
}




void score(){
	
	if(cnt==50){
	ballcnt2=ballcnt2+1;
	if(ballcnt2==1){
	if(ballcolumn==0x01){
		PlayerTwoScore=(PlayerTwoScore+1);
		HC595Write(PlayerTwoScore);
		displaytimeronoff=0;
	}

	if(ballcolumn==0x80){
		PlayerOneScore=(PlayerOneScore+1);
		HC595Write1(PlayerOneScore);
		displaytimeronoff=0;
	}
	ballcnt2=0;
	}
}
}



void ballreset(){
	if(ballcolumn==0x01){
		ballcnt3=1;
	}
	if(ballcolumn==0x80){
		ballcnt3=1;
	}
	if(ballcnt3==1){
		mode=3;
		ballcnt4=ballcnt4+1;
		ballcolumn=0x08;
		ballrow=0x08;
		if(ballcnt4==1000){
			mode=0;
			ballcnt3=0;
			ballcnt4=0;
		}
	}
}

void Ai(){
if(AiSwitch){
if(cnt==50){
if(RightGate==0){
if(LeftPad>ballrow){
	LeftPad=LeftPad>>1;
}
}
if(RightGate==0){
if(LeftPad<ballrow){
	LeftPad=LeftPad<<1;
}
}
}
}
}

void timescreen(){
	while(displaytimeronoff==0){
		if(displaytimertransition<=500){
	displaytimertransition=displaytimertransition+1;
	displaytimer=displaytimer+1;
	if(displaytimer==1){
	PORTD=(0x3C);
	PORTC=~(0x2A);
	}
	if(displaytimer==2){
	PORTD=(0x20);
	PORTC=~(0x14);
	}
	while(!TimerFlag){
		if(displaytimer==3){
			displaytimer=0;
		}
	}
	TimerFlag=0;
	}
	if(displaytimertransition<=1000){
	if(displaytimertransition>=500){
		displaytimertransition=displaytimertransition+1;
		displaytimer=displaytimer+1;
		if(displaytimer==1){
			PORTD=(0x3C);
			PORTC=~(0x2A);
		}
		if(displaytimer==2){
			PORTD=(0x04);
			PORTC=~(0x10);
		}
		if(displaytimer==3){
			PORTD=(0x20);
			PORTC=~(0x04);
		}
		while(!TimerFlag){
			if(displaytimer==4){
				displaytimer=0;
			}
		}
	}
	
		TimerFlag=0;
	}
	if(displaytimertransition>=1000){
		displaytimertransition=displaytimertransition+1;
		PORTD=(0x10);
		PORTC=~(0x3E);
		while(!TimerFlag);
		TimerFlag=0;
	}
	if(displaytimertransition==1500){
		displaytimeronoff=1;
		displaytimertransition=0x00;
		displaytimer=0;
	}
	
}
}

void WinnerLoserFlash(){
	if(winner==0){
		if(PlayerOneScore>=0x0A){
			displaytimeronoff=1;
			if(flashing<=100){
				PIND=0x50;
				PINC=0xAA;
			}
			if(flashing>=100){
				PIND=0xA0;
				PINC=0x55;
			}
			if(flashing==200){
				flashing=0;
			}
			flashing=flashing+1;
			flashingoff=flashingoff+1;
			mode=3;
			if(flashingoff==5000){
				winner=1;
				mode=1;
				state=init;
				ballcolumn=0x10;
				ballrow=0x10;
				displaytimeronoff=0;
				}
		}
		if(PlayerTwoScore>=0x0A){
			displaytimeronoff=1;
			if(flashing<=100){
				PIND= 0x05;
				PINC=0xAA;
				
			}
			if(flashing>=100){
				PIND=0x0A;
				PINC=0x55;
				
			}
			if(flashing==200){
				flashing=0;
		}
		flashing=flashing+1;
		flashingoff=flashingoff+1;
		mode=3;
		if(flashingoff==5000){
			winner=1;
			mode=1;
			state=init;
			ballcolumn=0x10;
			ballrow=0x10;
			displaytimeronoff=0;
		}
		}

	}
}

void Time(){
	while(!TimerFlag);
	TimerFlag=0;
	if(cnt==50){
		cnt=0;
	}
	cnt=(cnt+1);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0xFF;
	TimerSet(1);
	TimerOn();
	LeftPad=0x38;
	RightPad=0x38;
	stateCounter=DownLeftCounter;
	stateClockwise=DownRightClockwise;
	ballcolumn=0x01;
	ballrow=0x01;
	mode=1;
	balltime=10;
	HC595Init();
	HC595Write(0x00);
	HC595Write1(0x00);
	
    while (1) {

	Transitions();
	LeftScroll();
	RightScroll();
	Padle();
	ballCounter();
	ballClockwise();
	WinnerLoserFlash();
	timescreen();
	score();
	ballreset();
	Ai();
		//	attempt();
	Adder();
	resetgame();

	Time();
	
    }
	return 0;
}