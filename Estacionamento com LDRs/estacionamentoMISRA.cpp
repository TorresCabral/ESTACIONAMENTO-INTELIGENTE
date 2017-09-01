#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "wiring_private.h"
#include "pins_arduino.h"

#define SREG_GLOBAL_INT_ENABLE 7

const uint8_t PS_16 = (1 << ADPS2);
const uint8_t PS_32 = (1 << ADPS2) | (1 << ADPS0);
const uint8_t PS_64 = (1 << ADPS2) | (1 << ADPS1);
const uint8_t PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

uint16_t ad0, ad1, ad2, ad3;
bool ledON = false;

ISR(TIMER1_COMPA_vect)
{
    if (ledON == true)
    {
        ledON = false;
        PORTB |= (1 << PORTB5); // Turn LED on
    }
    else
    {
        ledON = true;
        PORTB &= ~(1 << PORTB5); // Turn LED off
    }
}

/* Função de mostrar telas */
void telas(void)
{
    if (ad0 < 500 || ad1 < 500 || ad2 < 500 || ad3 < 500)
    {
        if (ad1 < 500) // pino a0
        {
            printf("\n[7] [X] [X] ");
        }
        else
        {
            printf("\n[X] [X] [X] ");
        }
        if (ad0 < 500) // pino a3
        {
            printf("[1]");
        }
        else
        {
            printf("[X]");
        }
        if (ad2 < 500) // pino a1
        {
            printf("\n[8] [X] [X] ");
        }
        else
        {
            printf("\n[X] [X] [X] ");
        }
        if (ad3 < 500) // pino a2
        {
            printf("[2]\n");
        }
        else
        {
            printf("[X]\n");
        }
    }
    else
    {
        printf("\n[X] [X] [X] [X]\n\n[X] [X] [X] [X]\n\n\n\n\n\n");
    }
}

/* Função de leitura dos sensores */
void leitura(void)
{
    ADMUX = (0 << MUX1) | (0 << MUX0);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADATE); // Auto Trigger Enable  (Pag. 263)
    ADCSRA |= (1 << ADEN); // Turn ON A/D converter
    ADCSRA |= (1 << ADSC); // Start Convertion
    ad0 = ADCL;  // ADCL must be read first, then ADCH
    ad0 = (ADCH << 8) + ad0;
    _delay_ms(500);

    ADMUX = (0 << MUX1) | (1 << MUX0);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADATE); // Auto Trigger Enable  (Pag. 263)
    ADCSRA |= (1 << ADEN); // Turn ON A/D converter
    ADCSRA |= (1 << ADSC); // Start Convertion
    ad1 = ADCL;  // ADCL must be read first, then ADCH
    ad1 = (ADCH << 8) + ad1;
    _delay_ms(500);

    ADMUX = (1 << MUX1) | (0 << MUX0);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADATE); // Auto Trigger Enable  (Pag. 263)
    ADCSRA |= (1 << ADEN); // Turn ON A/D converter
    ADCSRA |= (1 << ADSC); // Start Convertion
    ad2 = ADCL;  // ADCL must be read first, then ADCH
    ad2 = (ADCH << 8) + ad2;
    _delay_ms(500);

    ADMUX = (1 << MUX1) | (1 << MUX0);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADATE); // Auto Trigger Enable  (Pag. 263)
    ADCSRA |= (1 << ADEN); // Turn ON A/D converter
    ADCSRA |= (1 << ADSC); // Start Convertion
    ad3 = ADCL;  // ADCL must be read first, then ADCH
    ad3 = (ADCH << 8) + ad3;
    _delay_ms(500);
}

/* INIT */
void ad_init(void)
{
    // configura o preescaler do ADC
    ADCSRA |= PS_128; // 128 prescaler
    ADMUX |= (1 << REFS0); // REFS1 = 0 Para tomar o Vcc de 5V
    ADCSRA |= (1 << ADATE); // Auto Trigger Enable  (Pag. 263)
    ADCSRA |= (1 << ADEN); // Turn ON A/D converter
    ADCSRA |= (1 << ADSC); // Start Convertion
}

void InitializeTimer1(void)
{
    TIMSK1 &= ~((1 << ICIE1) | (1 << OCIE1B) | (1 << OCIE1A) | (1 << TOIE1));
    TIMSK1 |= (1 << OCIE1A);

    TCCR1B |= (1<<WGM12);

    /*  Prescaler Configuration */
    TCCR1B |= (1 << CS12) | (1 << CS10); // clk/1024 (From prescaler)
}

/* INTERRUPÇÃO */
ISR(TIMER1_OVF_vect)                              //interrupção do TIMER1
{
    TCNT1 = 0xC2F7; // Renicia TIMER
    printf("Bem Vindo\n"); // printa "Bem Vindo" na Serial
}

static int uart_putchar(char c, FILE *stream) // Direcionando o fluxo de dados de printf para a Serial (UART)
{
    Serial.write(c);
    return 0;
}

int main(void)
{
    DDRB |= (1 << DDB5); // habilita o pino 13 para saída digital (OUTPUT).

    InitializeTimer1();

    OCR1A = 15624; // 1Hz com clk/1024 (From prescaler)

    /* TIMER PARA INTERRUPÇÃO */
    TCCR1A = 0;                        //confira timer para operação normal pinos OC1A e OC1B desconectados
    TCCR1B = 0;                        //limpa registrador
    TCCR1B |= (1 << CS10) | (1 << CS12);   // configura prescaler para 1024: CS12 = 1 e CS10 = 1

    TCNT1 = 0xC2F7;                    // incia timer com valor para que estouro ocorra em 1 segundo
        // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7

    TIMSK1 |= (1 << TOIE1);           // habilita a interrupção do TIMER1*/

    /* CONFIGURAÇÃO DA SERIAL */
    static FILE uartout = {0} ;   // create a FILE structure to reference our UART output function
    fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE); // fill in the UART file descriptor with pointer to writer.
    stdout = &uartout;  // The uart is the standard output device STDOUT.
    ad_init();
    Serial.begin(9600); // Start the UART

    /* LAÇO */
    while (1)
    {
        leitura();
        telas();
        SREG |= (1 << SREG_GLOBAL_INT_ENABLE);
    }
}
