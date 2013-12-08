#include <avr/pgmspace.h> # Incluir <avr/pgmspace.h>

#define COLUMN 8        // Anzahl der Spalten # Define a coluna 8 / / Número de colunas
#define PLANE 5         // Anzahl der Ebenen # Define AVIÃO 5 / / número de níveis
#define PLANETIME 3333  // Anzeigedauer einer Ebene # Define plano temporal 3333 / tempo de exibição / um avião
#define TIMECONST 15    // Multiplikator für Anzeigedauer eines Bildes (33 LEDs) # Define Timecon 15 / / multiplicador por quanto tempo ele permanece (33 diodos emissores de luz)

// LED Muster: Eine Zeile besteht aus den 5 Ebenen mit je 8 LEDs (außer Ebene 5 mit einer LED) und der Anzeigedauer / / Padrão diodo emissor de luz: A linha é composta de cinco níveis, cada um com 8 LEDs (exceto o nível 5 com um diodo emissor de luz) eo tempo de exibição
prog_uchar PROGMEM PatternTable[] = { prog_uchar padrão PROGMEM tabela [] = {
B11111111, B11111111, B11111111, B11111111, B1, 5,  // Alle LEDs an B11111111, B11111111, B11111111, B11111111, B1, 5 / / Todos os LEDs
B00000000, B00000000, B00000000, B00000000, B0, 1,  // Alle LEDs aus B00000000, B00000000, B00000000, B00000000, B0, 1, / / Todos os LEDs desligados
B11111111, B11111111, B11111111, B11111111, B1, 5, B11111111, B11111111, B11111111, B11111111, B1, 5,
B00000000, B00000000, B00000000, B00000000, B0, 10, B00000000, B00000000, B00000000, B00000000, B0, 10,
// Bei Anzeigedauer 0 endet die Schleife / / Para o tempo de exibição 0 termina o loop
B00000000, B00000000, B00000000, B00000000, B0, 0 B00000000, B00000000, B00000000, B00000000, B0, 0
}; };

// Definition der Pins für Ebenen und Spalten (hängt davon ab, wie man sie verlötet hat) / / Define os pinos de camadas e colunas (dependendo de como você os soldados)
int LedPin[] = {7, 8, 6, 9, 10, 11, 12, 5}; ledPin int [] = {7, 8, 6, 9, 10, 11, 12, 5};
int PlanePin[] = {18, 17, 16, 15, 14}; PlanePin int [] = {18, 17, 16, 15, 14};

void setup() void setup ()
{ {
	int pin; int pino;
	// Pins als OUTPUT initialiseren / / Pins como OUTPUT isere inicial
	for (pin=0; pin<COLUMN; pin++) { para (pin = 0; COLUNA pin <; pinos + +) {
		pinMode( LedPin[pin], OUTPUT ); pinMode (ledPin [pin], OUTPUT);
	} }

	for (pin=0; pin<PLANE; pin++) { para (pin = 0; AVIÃO pin <; pinos + +) {
		pinMode( PlanePin[pin], OUTPUT ); pinMode (PlanePin [pin], OUTPUT);
	} }
} }

void loop() void loop ()
{ {
	byte PatternBuf[PLANE]; // speichert eine Zeile aus LED Muster PatternBuf byte [AVIÃO] / / armazena uma linha de padrões de LED
	int PatternIdx; PatternIdx int;
	byte DisplayTime; tempo de exibição de bytes;
	unsigned long EndTime; EndTime unsigned long;
	int plane; plano int;
	int patbufidx; patbufidx int;
	int ledpin; ledPin int;
	
	// Index in LED Muster setzen / / Índice do padrão definido LED
	PatternIdx = 0; PatternIdx = 0;
	// Wiederholen solange Anzeigedauer > 0 / / Repetir até que o tempo de exibição> 0
	do { do {
		// Zeile aus LED Muster lesen / / Lê linha de padrões de LED
		memcpy_P( PatternBuf, PatternTable+PatternIdx, PLANE ); memcpy_P (PatternBuf, mesa de teste + PatternIdx, avião);
		PatternIdx += PLANE; PatternIdx + = plano;
		// Anzeigedauer lesen / / Ler Timing
		DisplayTime = pgm_read_byte_near( PatternTable + PatternIdx++ ); tempo de exibição = pgm_read_byte_near (padrão da tabela PatternIdx + + +); 
		// Anzeigedauer für Bild berechnen / / Tempo de exposição para a carga da imagem
		EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST; EndTime millis = () + ((tempo de exibição unsigned long)) * Timecon;
		
		// LEDs der Ebenen immer wieder aufbauen, solange Anzeigedauer für Bild noch nicht erreicht ist / / LEDs dos níveis de novo a construir, enquanto o tempo de exibição da imagem não foi alcançado
		while ( millis() < EndTime ) { while (Millis () <EndTime) {
			patbufidx = 0; // Am Anfang der Zeile beginnen patbufidx = 0, / / No início do início da linha
			// Schleife über Ebenen / / Loop em relação aos níveis
			for (plane=0; plane<PLANE; plane++) { para (plano = 0; avião avião <AVIÃO + +) {
				// vorherige Ebene ausschalten / / Nível anterior fora
				if (plane==0) { if (plano == 0) {
				  digitalWrite( PlanePin[PLANE-1], HIGH ); digitalWrite (PlanePin [AVIÃO-1], HIGH);
				} else { Else {}
				  digitalWrite( PlanePin[plane-1], HIGH ); digitalWrite (PlanePin [plano 1], HIGH);
				} }
		
				// Spalten der aktuelle Ebene setzen / / Colunas do nível atual do conjunto

				// Ebene 5 gesondert behandeln, da nur eine LED / / Nível 5 tratadas separadamente, pois somente um diodo emissor de luz
 				if (plane==PLANE-1) { if (plano == PLANO-1) {
				    digitalWrite( LedPin[0], PatternBuf[plane] ); digitalWrite (ledPin [0], PatternBuf [plano]);
                                } else { Else {}
				    // Einzelne Spalten setzen / / Colunas Set individual
                                   for (ledpin=0; ledpin<COLUMN; ledpin++) { para (ledPin = 0; ledPin <COLUNA; ledPin + +) {
				      // Bitoperation zur Ermittlung der einzelnen Bitwerte Operação de E / / Bit para determinar os valores de bits individuais
				      // Beispiel: / / Exemplo:
				      // LED Zeile & ledpin (left shift (<<) schiebt ledpin um 1 nach rechts) / / LED & linha ledPin (deslocamento para a esquerda (<<) desloca ledPin por 1 a direita)
				      // B01010000 & B10000000 = 0 (LED leuchtet nicht) / / B01010000 & B10000000 = 0 (LED está desligado)
				      // B01010000 & B01000000 = 1 (LED leuchtet) / / B01010000 & B01000000 = 1 (LED)
				      // B01010000 & B00100000 = 0 (LED leuchtet nicht) / / B01010000 & B00100000 = 0 (LED está desligado)
 				      digitalWrite( LedPin[ledpin], PatternBuf[plane] & (1 << ledpin) ); digitalWrite (ledPin [ledPin] PatternBuf ledPin [plano] & (1 <<));
				    } } 
                                } }
		
				// aktuelle Ebene aktivieren / / O nível atual permite
				digitalWrite( PlanePin[plane], LOW ); digitalWrite (PlanePin [plano], LOW);
				// Anzeigedauer für Ebene / / Tempo de exposição para o nível
				delayMicroseconds( PLANETIME ); microssegundos atraso (plano temporal);
			} }
		} }
	} while (DisplayTime > 0); Enquanto} (tempo de exibição> 0);
} }
