#include <avr/pgmspac.h>


#define COLUMN 8        // Numero de colunas (Spalten)
#define PLANE 5         // Numero de niveis (Ebenen)
#define PLANETIME 3333  // tempo de exibicao
#define TIMECONST 15    // Multiplicado por quanto tempo ele fica (33 LEDs)

// LED padrao: linha composta por 5 Ebenen, cada uma com 8 LEDs (exceto Ebene 5 que tem um LED)
prog_uchar PROGMEM PatternTable[] = {
B11111111, B11111111, B11111111, B11111111, B1, 5,  // todos os LEDs ligados
B00000000, B00000000, B00000000, B00000000, B0, 1,  // todos os LEDs desligados
B11111111, B11111111, B11111111, B11111111, B1, 5,
B00000000, B00000000, B00000000, B00000000, B0, 10,
// Para o tempo de exibição 0 temina o loop
B00000000, B00000000, B00000000, B00000000, B0, 0
};

// define os pinos de camadas e colunas (depende como foi conectado na placa)
int LedPin[] = {2, 4, 7, 8, 10, 11, 12, 13};
int PlanePin[] = {1, 2, 3, 4, 5};

void setup()
{
	int pin;
	// Pins als OUTPUT initialiseren
	for (pin=0; pin<COLUMN; pin++) {
		pinMode( LedPin[pin], OUTPUT );
	}

	for (pin=0; pin<PLANE; pin++) {
		pinMode( PlanePin[pin], OUTPUT );
	}
}

void loop()
{
	byte PatternBuf[PLANE]; // armazena um linha de padrões de LED
	int PatternIdx;
	byte DisplayTime;
	unsigned long EndTime;
	int plane;
	int patbufidx;
	int ledpin;
	
	// Index padrão do LED
	PatternIdx = 0;
	// repetir até que tempo de exibição > 0
	do {
		// Lê linha de padrões de LED
		memcpy_P( PatternBuf, PatternTable+PatternIdx, PLANE );
		PatternIdx += PLANE;
		// Ler Timing
		DisplayTime = pgm_read_byte_near( PatternTable + PatternIdx++ ); 
		// tempo de exposição para carga imagem
		EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST;
		
		// LEDs dos níveis de novo a construir, enquanto o tempo de exibição da imagem não foi alcançado
		while ( millis() < EndTime ) {
			patbufidx = 0; // No início do início da linha
			// Loop em relação aos níveis
			for (plane=0; plane<PLANE; plane++) {
				// Nível anterior fora
				if (plane==0) {
				  digitalWrite( PlanePin[PLANE-1], HIGH );
				} else {
				  digitalWrite( PlanePin[plane-1], HIGH );
				}
		
				// Colunas do nível atual do conjunto

				// Nível 5 tratadas separadamente, pois tem somente um diodo emissor de luz
 				if (plane==PLANE-1) {
				    digitalWrite( LedPin[0], PatternBuf[plane] );
                                } else {
				    // Colunas Set individual
                                   for (ledpin=0; ledpin<COLUMN; ledpin++) {
				      // Bit para determinar os valores de bits individuais
				      // Exemplo:
				      // LED linha & ledpin (left shift (<<) desloca ledpin por 1 a direita)
				      // B01010000 & B10000000 = 0 (LED deligado)
				      // B01010000 & B01000000 = 1 (LED ligado)
				      // B01010000 & B00100000 = 0 (LED desligado)
 				      digitalWrite( LedPin[ledpin], PatternBuf[plane] & (1 << ledpin) );
				    } 
                                }
		
				// O nível atual permite
				digitalWrite( PlanePin[plane], LOW );
				// Tempo de exposição para o nível
				delayMicroseconds( PLANETIME );
			}
		}
	} while (DisplayTime > 0);
}

