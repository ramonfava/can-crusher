#define F_CPU 16000000UL
#include <avr/io.h>

void amassarLata();
int meuDelay(double delay);
void mostrarNum(int num);

int main(void)
{
	ADMUX |= _BV(REFS0);//selecionar entrada do analogico
	ADCSRA |= (1<<ADPS1) | _BV(ADEN);
	
	TCCR0B &= 0b11110101;//setando WGM02 e CS01 como 0
	TCCR0B |= 0b00000101;//setando CS02 e CS00 como 1
	TCCR0A &= 0b11111100;//setando WGM01 e WGM00 como 0;
	
	/*
		Pinos B: 5 - 0, leds do 7 segmentos
		Pino D: 7, led 7 segmentos
		Pinos D: 6 e 5, reles dos pistoes
	*/
	DDRB |= 0b00111111;
	DDRD |= 0b11100000;

	PORTB |= 0b00111111;//leds e reles ativam com sinal baixo
	PORTD |= 0b11100000;
	
	int numLatas = 0;
	float valor;
	mostrarNum(numLatas);
	
	while (1)
	{
		ADCSRA |= _BV(ADSC);
		while(!(ADCSRA & 0x10));
		valor = ADC;
		
		if(valor < 330 ){
			++numLatas;
			if(numLatas > 9)
				numLatas = 0;
				
			mostrarNum(numLatas);
			amassarLata();
		}
	}
}

int meuDelay(double delay){
	int contador = 0;
	int tempo = delay*61;//61 = 1s
	while(1){
		while((TIFR0 & (1 << 0)) == 0);
		TIFR0 |= 0b00000001;
		contador++;
		if (contador == tempo)
		{
			return 0;
		}
	}
}

void amassarLata(){
	PORTD &= 0b10111111;//pegar a lata
	meuDelay(0.5);
	PORTD |= 0b01000000;//volta para amassar
	meuDelay(0.5);
	PORTD &= 0b11011111;//amassa
	meuDelay(0.5);
	PORTD |= 0b00100000;//volta pistao de amassar
	meuDelay(0.5);
}

void mostrarNum(int num){
	PORTB |= 0b00111111;
	PORTD |= 0b10000000;
	switch(num){
		case 1:
			PORTB &= 0b11110110;
			break;
		case 2:
			PORTB &= 0b11100001;
			PORTD &= 0b01111111;
			break;
		case 3:
			PORTB &= 0b11100100;
			PORTD &= 0b01111111;
			break;
		case 4:
			PORTB &= 0b11010110;
			PORTD &= 0b01111111;
			break;
		case 5:
			PORTB &= 0b11001100;
			PORTD &= 0b01111111;
			break;
		case 6:
			PORTB &= 0b11001000;
			PORTD &= 0b01111111;
			break;
		case 7:
			PORTB &= 0b11100110;
			break;
		case 8:
			PORTB &= 0b11000000;
			PORTD &= 0b01111111;
			break;
		case 9:
			PORTB &= 0b11000100;
			PORTD &= 0b01111111;
			break;
		case 0:
			PORTB &= 0b11000000;
			break;
		default:
			break;
	}
}


