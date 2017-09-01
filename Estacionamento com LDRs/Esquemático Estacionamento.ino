#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "wiring_private.h"
#include "pins_arduino.h"

/*const unsigned char PS_16 = (1 << ADPS2); 
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0); 
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1); */
const unsigned char PS_128=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
uint16_t ad0, ad1, ad2, ad3;

/////////////////////////////////////////////////////////////////////////Fun??o de mostrar telas//////////////////////////////////////////////////////////////////////////////////////

      void telas()
      {
        if((ad0<650)||(ad1<650)||(ad2<650)||(ad3<650)){
          if(ad1<650){                                   //pino a0
            printf( "\n[2]      " );}
             else {
              printf( "\n[OCUPADA]" );}
          if(ad0<650){                                   //pino a3 
            printf( "  [1]" );}
             else {
              printf("  [OCUPADA]");}             
          if(ad2<650){                                   //pino a1
            printf( "\n[4]      " );}
             else {
              printf("\n[OCUPADA]");} 
          if(ad3<650){                                   //pino a2
            printf("  [3]\n");}
             else {
              printf("  [OCUPADA]\n");
             }
             }
          else{
     printf( "\n[OCUPADA]  [OCUPADA]\n\n[OCUPADA]  [OCUPADA]\n\n\n\n\n\n" );}
      }



/////////////////////////////////////////////////////////////Fun??o de leitura dos sensores////////////////////////////////////////////////////////////////////////////////////

      
       void leitura(){
        ADMUX=(0<<MUX1)|(0<<MUX0); 
        ADMUX |= ( 1 << REFS0 ); 
        ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)   
        ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter   
        ADCSRA |= ( 1 << ADSC ); // Start Convertion 
        ad0 = ADCL;  // ADCL must be read first, then ADCH          
        ad0 = (ADCH << 8) + ad0;
        _delay_ms(1000);

        ADMUX=(0<<MUX1)|(1<<MUX0); 
        ADMUX |= ( 1 << REFS0 ); 
        ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)   
        ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter   
        ADCSRA |= ( 1 << ADSC ); // Start Convertion 
        ad1 = ADCL;  // ADCL must be read first, then ADCH          
        ad1 = (ADCH << 8) + ad1; 
        _delay_ms(1000);   

        ADMUX=(1<<MUX1)|(0<<MUX0); 
        ADMUX |= ( 1 << REFS0 ); 
        ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)   
        ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter   
        ADCSRA |= ( 1 << ADSC ); // Start Convertion 
        ad2 = ADCL;  // ADCL must be read first, then ADCH          
        ad2 = (ADCH << 8) + ad2; 
        _delay_ms(1000);

        ADMUX=(1<<MUX1)|(1<<MUX0); 
        ADMUX |= ( 1 << REFS0 ); 
        ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)   
        ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter   
        ADCSRA |= ( 1 << ADSC ); // Start Convertion 
        ad3 = ADCL;  // ADCL must be read first, then ADCH          
        ad3 = (ADCH << 8) + ad3; 
        _delay_ms(1000);
       /*printf("O valor do A/D 0: %d\r\n", ad0); 
       printf("O valor do A/D 1: %d\r\n", ad1); 
       printf("O valor do A/D 2: %d\r\n", ad2);
       printf("O valor do A/D 3: %d\r\n", ad3);*/ 
     }

     ////////////////////////////////////////////////////////////////////////////////INIT////////////////////////////////////////////////////////////////////////
     
  void ad_init(void)
  {
   // configura o preescaler do ADC   
   ADCSRA |= PS_128; // 128 prescaler   
   ADMUX |= ( 1 << REFS0 ); 
   ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)   
   ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter   
   ADCSRA |= ( 1 << ADSC ); // Start Convertion 
  }
       
/*//////////////////////////////////////////////////////////////////////////////////INTERRUP??O///////////////////////////////////////////////////////////////////////
      ISR(TIMER1_OVF_vect)                              //interrup??o do TIMER1 
{
  TCNT1 = 0xC2F7; // Renicia TIMER
  printf( "Bem Vindo"); // printa "Bem Vindo" na Serial
  }*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  static int uart_putchar(char c, FILE *stream) ////---- Direcionando o fluxo de dados de printf para a Serial (UART) ---- static int uart_putchar (char c, FILE *stream)
  {
  Serial.write(c);
  return 0;
  }  
  
  int main(void) {

/*////////////////////////////////////////////////////////////////////////TIMER PARA INTERRUP??O/////////////////////////////////////////////////////////////////////
  TCCR1A = 0;                        //confira timer para opera??o normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;                        //limpa registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);   // configura prescaler para 1024: CS12 = 1 e CS10 = 1
 
  TCNT1 = 0xC2F7;                    // inicia timer com valor para que estouro ocorra em 1 segundo
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  
  TIMSK1 |= (1 << TOIE1);           // habilita a interrup??o do TIMER1*/
  
///////////////////////////////////////////////////////////////////////CONFIGURA??O DA SERIAL///////////////////////////////////////////////////////////////////////////////////
    static FILE uartout = {0} ;   // create a FILE structure to reference our UART output function             
    fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE); // fill in the UART file descriptor with pointer to writer. 
    stdout = &uartout ;  // The uart is the standard output device STDOUT.              
    ad_init();
    Serial.begin(9600) ; // Start the UART
     
 ///////////////////////////////////////////////////////////////////////////////////////LA?O////////////////////////////////////////////////////////////////////     
     while(1){           // Cria la?o infinito
           leitura();
           telas();  
     }
  }