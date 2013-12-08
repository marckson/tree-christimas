#include <avr/pgmspace.h> # Incluir <avr/pgmspace.h>

#define COLUMN 8        // Anzahl der Spalten # Define a coluna 8 / / N�mero de colunas
#define PLANE 5         // Anzahl der Ebenen # Define AVI�O 5 / / n�mero de n�veis
#define PLANETIME 3333  // Anzeigedauer einer Ebene # Define plano temporal 3333 / tempo de exibi��o / um avi�o
#define TIMECONST 15    // Multiplikator f�r Anzeigedauer eines Bildes (33 LEDs) # Define Timecon 15 / / multiplicador por quanto tempo ele permanece (33 diodos emissores de luz)

// LED Muster: Eine Zeile besteht aus den 5 Ebenen mit je 8 LEDs (au�er Ebene 5 mit einer LED) und der Anzeigedauer / / Padr�o diodo emissor de luz: A linha � composta de cinco n�veis, cada um com 8 LEDs (exceto o n�vel 5 com um diodo emissor de luz) eo tempo de exibi��o
prog_uchar PROGMEM PatternTable[] = { prog_uchar padr�o PROGMEM tabela [] = {
B11111111, B11111111, B11111111, B11111111, B1, 5,  // Alle LEDs an B11111111, B11111111, B11111111, B11111111, B1, 5 / / Todos os LEDs
B00000000, B00000000, B00000000, B00000000, B0, 1,  // Alle LEDs aus B00000000, B00000000, B00000000, B00000000, B0, 1, / / Todos os LEDs desligados
B11111111, B11111111, B11111111, B11111111, B1, 5, B11111111, B11111111, B11111111, B11111111, B1, 5,
B00000000, B00000000, B00000000, B00000000, B0, 10, B00000000, B00000000, B00000000, B00000000, B0, 10,
// Bei Anzeigedauer 0 endet die Schleife / / Para o tempo de exibi��o 0 termina o loop
B00000000, B00000000, B00000000, B00000000, B0, 0 B00000000, B00000000, B00000000, B00000000, B0, 0
}; };

// Definition der Pins f�r Ebenen und Spalten (h�ngt davon ab, wie man sie verl�tet hat) / / Define os pinos de camadas e colunas (dependendo de como voc� os soldados)
int LedPin[] = {7, 8, 6, 9, 10, 11, 12, 5}; ledPin int [] = {7, 8, 6, 9, 10, 11, 12, 5};
int PlanePin[] = {18, 17, 16, 15, 14}; PlanePin int [] = {18, 17, 16, 15, 14};

void setup() void setup ()
{ {
	int pin; int pino;
	// Pins als OUTPUT initialiseren / / Pins como OUTPUT isere inicial
	for (pin=0; pin<COLUMN; pin++) { para (pin = 0; COLUNA pin <; pinos + +) {
		pinMode( LedPin[pin], OUTPUT ); pinMode (ledPin [pin], OUTPUT);
	} }

	for (pin=0; pin<PLANE; pin++) { para (pin = 0; AVI�O pin <; pinos + +) {
		pinMode( PlanePin[pin], OUTPUT ); pinMode (PlanePin [pin], OUTPUT);
	} }
} }

void loop() void loop ()
{ {
	byte PatternBuf[PLANE]; // speichert eine Zeile aus LED Muster PatternBuf byte [AVI�O] / / armazena uma linha de padr�es de LED
	int PatternIdx; PatternIdx int;
	byte DisplayTime; tempo de exibi��o de bytes;
	unsigned long EndTime; EndTime unsigned long;
	int plane; plano int;
	int patbufidx; patbufidx int;
	int ledpin; ledPin int;
	
	// Index in LED Muster setzen / / �ndice do padr�o definido LED
	PatternIdx = 0; PatternIdx = 0;
	// Wiederholen solange Anzeigedauer > 0 / / Repetir at� que o tempo de exibi��o> 0
	do { do {
		// Zeile aus LED Muster lesen / / L� linha de padr�es de LED
		memcpy_P( PatternBuf, PatternTable+PatternIdx, PLANE ); memcpy_P (PatternBuf, mesa de teste + PatternIdx, avi�o);
		PatternIdx += PLANE; PatternIdx + = plano;
		// Anzeigedauer lesen / / Ler Timing
		DisplayTime = pgm_read_byte_near( PatternTable + PatternIdx++ ); tempo de exibi��o = pgm_read_byte_near (padr�o da tabela PatternIdx + + +); 
		// Anzeigedauer f�r Bild berechnen / / Tempo de exposi��o para a carga da imagem
		EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST; EndTime millis = () + ((tempo de exibi��o unsigned long)) * Timecon;
		
		// LEDs der Ebenen immer wieder aufbauen, solange Anzeigedauer f�r Bild noch nicht erreicht ist / / LEDs dos n�veis de novo a construir, enquanto o tempo de exibi��o da imagem n�o foi alcan�ado
		while ( millis() < EndTime ) { while (Millis () <EndTime) {
			patbufidx = 0; // Am Anfang der Zeile beginnen patbufidx = 0, / / No in�cio do in�cio da linha
			// Schleife �ber Ebenen / / Loop em rela��o aos n�veis
			for (plane=0; plane<PLANE; plane++) { para (plano = 0; avi�o avi�o <AVI�O + +) {
				// vorherige Ebene ausschalten / / N�vel anterior fora
				if (plane==0) { if (plano == 0) {
				  digitalWrite( PlanePin[PLANE-1], HIGH ); digitalWrite (PlanePin [AVI�O-1], HIGH);
				} else { Else {}
				  digitalWrite( PlanePin[plane-1], HIGH ); digitalWrite (PlanePin [plano 1], HIGH);
				} }
		
				// Spalten der aktuelle Ebene setzen / / Colunas do n�vel atual do conjunto

				// Ebene 5 gesondert behandeln, da nur eine LED / / N�vel 5 tratadas separadamente, pois somente um diodo emissor de luz
 				if (plane==PLANE-1) { if (plano == PLANO-1) {
				    digitalWrite( LedPin[0], PatternBuf[plane] ); digitalWrite (ledPin [0], PatternBuf [plano]);
                                } else { Else {}
				    // Einzelne Spalten setzen / / Colunas Set individual
                                   for (ledpin=0; ledpin<COLUMN; ledpin++) { para (ledPin = 0; ledPin <COLUNA; ledPin + +) {
				      // Bitoperation zur Ermittlung der einzelnen Bitwerte Opera��o de E / / Bit para determinar os valores de bits individuais
				      // Beispiel: / / Exemplo:
				      // LED Zeile & ledpin (left shift (<<) schiebt ledpin um 1 nach rechts) / / LED & linha ledPin (deslocamento para a esquerda (<<) desloca ledPin por 1 a direita)
				      // B01010000 & B10000000 = 0 (LED leuchtet nicht) / / B01010000 & B10000000 = 0 (LED est� desligado)
				      // B01010000 & B01000000 = 1 (LED leuchtet) / / B01010000 & B01000000 = 1 (LED)
				      // B01010000 & B00100000 = 0 (LED leuchtet nicht) / / B01010000 & B00100000 = 0 (LED est� desligado)
 				      digitalWrite( LedPin[ledpin], PatternBuf[plane] & (1 << ledpin) ); digitalWrite (ledPin [ledPin] PatternBuf ledPin [plano] & (1 <<));
				    } } 
                                } }
		
				// aktuelle Ebene aktivieren / / O n�vel atual permite
				digitalWrite( PlanePin[plane], LOW ); digitalWrite (PlanePin [plano], LOW);
				// Anzeigedauer f�r Ebene / / Tempo de exposi��o para o n�vel
				delayMicroseconds( PLANETIME ); microssegundos atraso (plano temporal);
			} }
		} }
	} while (DisplayTime > 0); Enquanto} (tempo de exibi��o> 0);
} }
