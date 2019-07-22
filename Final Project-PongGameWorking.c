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
if(cnt==100){
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
			break;
				
			case DownLeftCounter:
			ballcolumn=ballcolumn>>1;
			ballrow=ballrow<<1;
			if(ballrow==0x80){
			stateClockwise=UpLeftClockwise;
			mode=0;
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
if(cnt==100){
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
/*
void resetgame(){

if(buttonReset){
while(buttonReset){
	DDRA = 0x00; PORTA = 0x10;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
}
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
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

	TimerSet(1);
	TimerOn();
	LeftPad=0x38;
	RightPad=0x38;
	ballcolumn=0x01;
	ballrow=0x01;
}
}
*/



void score(){
	if(cnt==100){
	ballcnt2=ballcnt2+1;
	if(ballcnt2==1){
	if(ballcolumn==0x01){
		PlayerTwoScore=(PlayerTwoScore+1);
		PORTB=PlayerTwoScore;
	}
	if(ballcolumn==0x80){
		PlayerOneScore=(PlayerOneScore+1);
		PORTB=PlayerOneScore;
	}
	//}
	ballcnt2=0;
	//}
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
	ballcnt4=ballcnt4+1;
	if(ballcnt4==3000){
	ballcolumn = 0x08;
	ballrow=0x08;
		ballcolumn=0x08;
		ballrow=0x08;
		ballcnt3=0;
		ballcnt4=0;
		}
	}
}

void Time(){
	while(!TimerFlag);
	TimerFlag=0;
	if(cnt==100){
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
	stateCounter=UpLeftCounter;
	stateClockwise=DownLeftClockwise;
	ballcolumn=0x04;
	ballrow=0x04;
	mode=1;
	balltime=3;
	
    while (1) {
	Transitions();
	LeftScroll();
	RightScroll();
	Padle();
	ballCounter();
	ballClockwise();
	score();
	ballreset();
	Adder();
	//resetgame();
	Time();
    }
}