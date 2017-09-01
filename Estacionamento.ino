#include <Ultrasonic.h> //Inclusão de Ultrasonic.h.
#define echoPin1 6 // Define o Pino ECHO do sensor 1 no pino 6 do arduino   
#define trigPin1 7 // Define o Pino TRIG do sensor 1 no pino 7 do arduino 
#define echoPin2 8   // Define o Pino ECHO do sensor 2 no pino 8 do arduino 
#define trigPin2 9   // Define o Pino TRIG do sensor 2 no pino 9 do arduino 
#define echoPin3 10   // Define o Pino ECHO do sensor 3 no pino 10 do arduino 
#define trigPin3 11 // Define o Pino TRIG do sensor 3 no pino 11 do arduino 
#define echoPin4 12   // Define o Pino ECHO do sensor 4 no pino 12 do arduino  
#define trigPin4 13 // Define o Pino TRIG do sensor 4 no pino 13 do arduino 
float distancia1[2]; //Declaracao da variavel 'distancia' que vai ser medida pelo sensor 1
float distancia2[2]; //Declaracao da variavel 'distancia' que vai ser medida pelo sensor 2
float distancia3[2]; //Declaracao da variavel 'distancia' que vai ser medida pelo sensor 3
float distancia4[2]; //Declaracao da variavel 'distancia' que vai ser medida pelo sensor 4
int distancia=9; //Declaracao da distancia geral do chao ao teto
  void setup(){   // Funcao e configuracao
   Serial.begin(9600); //Configuracao da porta serial do arduino
   pinMode(trigPin1, OUTPUT); // Configura para que o pino TRIG do primeiro sensor envie dados  
   pinMode(echoPin1, INPUT);  // Configura para que o pino ECHO do primeiro sensor receba dados 
   pinMode(trigPin2, OUTPUT); // Configura para que o pino TRIG do segundo sensor envie dados  
   pinMode(echoPin2, INPUT);  // Configura para que o pino ECHO do segundo sensor receba dados 
   pinMode(trigPin3, OUTPUT); // Configura para que o pino TRIG do terceiro sensor envie dados   
   pinMode(echoPin3, INPUT);  // Configura para que o pino ECHO do terceiro sensor receba dados 
   pinMode(trigPin4, OUTPUT); // Configura para que o pino TRIG do quarto sensor envie dados   
   pinMode(echoPin4, INPUT);  // Configura para que o pino ECHO do quarto sensor receba dados 
 } 
  void loop(){ //Funcao principal do arduino
      Serial.print(""); // Mostra "" na tela
            Serial.print(""); // Mostra "" na tela
      for(int i=0; i<2; i++){ // Utilizaremos aqui um laco 'for' para medir a distancia duas vezes, em busca de ter certeza de que ha um carro parado ali, ao inves de alarmar como 'ocupado' ao passar qualquer coisa embaixo do sensor
       // Abaixo, em cada estrofe estaremos a fazer as medicoes individuais dos sensores//
       digitalWrite (trigPin1, LOW); // Inicia com o trigPin desligado.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin1, HIGH); // E liga o trigPin.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin1, LOW); // E volta a desligá-lo.
       unsigned long duracao1 = pulseIn(echoPin1, HIGH); // pulseIn já vem imbutida no arduino, no caso, ela verifica o echoPin.
       distancia1[i]= duracao1/58;  // A biblioteca Ultrasonic já realiza os cálculos internamente, por isso, só dividimos por 58. 
       
       digitalWrite (trigPin2, LOW); // Inicia com o trigPin desligado.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin2, HIGH); // E liga o trigPin.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin2, LOW); // E volta a desligá-lo.
       unsigned long duracao2 = pulseIn(echoPin2, HIGH); // pulseIn já vem imbutida no arduino, no caso, ela verifica o echoPin.
       distancia2[i]= duracao2/58;  // A biblioteca Ultrasonic já realiza os cálculos internamente, por isso, só dividimos por 58. //40    
       
       digitalWrite (trigPin3, LOW); // Inicia com o trigPin desligado.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin3, HIGH); // E liga o trigPin.
       delay(500); // Aguarda 0.5 segundos...
       digitalWrite (trigPin3, LOW); // E volta a desligá-lo.
       unsigned long duracao3 = pulseIn(echoPin3, HIGH); // pulseIn já vem imbutida no arduino, no caso, ela verifica o echoPin.
       distancia3[i]= duracao3/58;  // A biblioteca Ultrasonic já realiza os cálculos internamente, por isso, só dividimos por 58.
       
       digitalWrite (trigPin4, LOW); // Inicia com o trigPin desligado. //30
       delay(500); // Aguarda 0.5 segundos...   
       digitalWrite (trigPin4, HIGH); // E liga o trigPin. 
       delay(500); //Aguarda 0.5 segundos..//30
       digitalWrite (trigPin4, LOW); // E volta a desligá-lo. //40
       unsigned long duracao4 = pulseIn(echoPin4, HIGH); // pulseIn já vem imbutida no arduino, no caso, ela verifica o echoPin.
       distancia4[i]= duracao4/58;}  // A biblioteca Ultrasonic já realiza os cálculos internamente, por isso, só dividimos por 58.
       // Fim do laco 'for', eh interessante observar que foi usado o conceito de vetor em cada medicao, pois ja que o laco for sera repetido duas vezes, em i=0 guardamos a primeira medida e em i=1 guardamos a segunda medida
     if((distancia1[1]!=distancia)||(distancia2[1]!=distancia)||(distancia3[1]!=distancia)||(distancia4[1]!=distancia)){ // Se a ultima distancia medida em cada sensor for diferente da distancia comum do chao ate o teto, entramos no IF, pois quer dizer que algo passou embaixo de algum dos sensores
        if(distancia2[0]==distancia2[1]&&distancia2[1]==distancia){ // Se a primeira leitura e a ultima leitura do segundo sensor forem iguais e iguais a distancia normal..
          Serial.print( "        [   4   ]  [OCUPADA]  [OCUPADA]  " );} //Mostraremos que a vaga esta desocupada
          else{ //Se não..
            Serial.print( "        [OCUPADA]  [OCUPADA]  [OCUPADA]  " );} // A mostraremos ocupada
          if(distancia1[0]==distancia1[1]&&distancia1[1]==distancia){ //Se a primeira leitura e a ultima leitura do primeiro sensor forem iguais e iguais a distancia normal..
            Serial.println( "[   1   ]" );} //Mostraremos que a vaga esta desocupada
          else{ //Se não..
            Serial.println( "[OCUPADA]" );} // A mostraremos ocupada
          if(distancia3[0]==distancia3[1]&&distancia3[1]==distancia){ //Se a primeira leitura e a ultima leitura do terceiro sensor forem iguais e iguais a distancia normal..
          Serial.print( "        [   5   ]  [OCUPADA]  [OCUPADA]  " );} //Mostraremos que a vaga esta desocupada
          else{ //Se não..
            Serial.print( "        [OCUPADA]  [OCUPADA]  [OCUPADA]  " );} // A mostraremos ocupada
          if(distancia4[0]==distancia4[1]&&distancia4[1]==distancia){ //Se a primeira leitura e a ultima leitura do quarta sensor forem iguais e iguais a distancia normal..
          Serial.println( "[   8   ]\n" );} //Mostraremos que a vaga esta desocupada
          else{ //Se não..
          Serial.println( "[OCUPADA]\n" );}} // A mostraremos ocupada
          // Fim do primeiro IF
          else{ // Se a condicao do primeiro IF nao for verdadeira, ou seja, nao houver qualquer diferenca embaixo de qualquer sensor, entao sabemos que todas as vagas estao livres, dai...
     Serial.println( "       [   4   ]  [OCUPADA]  [OCUPADA]  [   1   ]"); // Printamos na tela as vagas livres
     Serial.println( "       [   5   ]  [OCUPADA]  [OCUPADA]  [   8   ]\n" );} // Sem nem passar pelo IF  
} //Fim do void loop
