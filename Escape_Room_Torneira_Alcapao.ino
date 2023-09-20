/* Distribuição dos pinos do Arduino */

#define FLOWSENSOR    2 // Precisa ser um pino com interrupção 2 ou 3
#define MAGLOCK       6 // Fechadura
#define VALVE         7 // Válvula 
#define INDICATOR     13 // Led on board indicador de funcionamento

#define AJUSTE        A0 // Botando um potenciometro de 10k para regular a sensibilidade

/* O limite de disparo do sensor é regulado aqui */

#define FLUXO         1000 // Limite de disparo do sensor

volatile int freqFluxo;
int litrosHora; // litros por hora
unsigned long tempoAtual;
unsigned long duracaoLoop;
int ajusteFluxo;

void medicaoFluxo () // Interrupt function

{
  freqFluxo++;
}

void setup()
{
  pinMode(FLOWSENSOR, INPUT);
  
  pinMode (MAGLOCK, OUTPUT);
  pinMode (VALVE, OUTPUT);
  pinMode (INDICATOR, OUTPUT);
  
  digitalWrite (MAGLOCK, LOW);
  digitalWrite (VALVE, LOW);
  digitalWrite (INDICATOR, LOW);
  Serial.begin(115200);
  attachInterrupt(0, medicaoFluxo, RISING); // Setup Interrupt
  sei(); // Habilita as interrupções
  tempoAtual = millis();
  duracaoLoop = tempoAtual;
}

void loop ()
{
  tempoAtual = millis();
  // A cada 1000 milisegundos (1 segundo) imprime o fluxo atual
   if (tempoAtual >= (duracaoLoop + 1000))
  {
    /* criei um potenciometro para ajustar a intensidade do fluxo */
    int pot = analogRead (AJUSTE);
    ajusteFluxo = map (pot, 140, 1024, 0, FLUXO);

    // ajusteFluxo = FLUXO; // se quiser remover o potenciometro e manter um valor fixo
    
    duracaoLoop = tempoAtual; // Updates duracaoLoop
    // A frequencia do pulso é (Hz) = 7.5Q, Q e a taxa de fluxo em L/minutos.
    litrosHora = (freqFluxo * 60 / 7.5); // (Frquencia de pulso x 60 min) / 7.5Q = taxa de fluxo em L/hora
    freqFluxo = 0; // Reset Counter
    Serial.print(litrosHora, DEC); // Exibe o fluxo atual em litros por hora
    Serial.println(" L/hora");
    Serial.print ("Ajuste Fluxo: ");
    Serial.println (ajusteFluxo);
    if (litrosHora > ajusteFluxo) { // alterando para ler o potenciometro
      Serial.println ("OPEN");
      digitalWrite (MAGLOCK, HIGH);
      digitalWrite (INDICATOR, HIGH);
      // digitalWrite (VALVE, HIGH);
      delay (3000);
    } else {
      digitalWrite (MAGLOCK, LOW);
      digitalWrite (INDICATOR, LOW);
      // digitalWrite (VALVE, LOW);
    }
  }
}
