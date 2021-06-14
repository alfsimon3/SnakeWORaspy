#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,1,1,0,1,0,0},
	{0,1,1,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,1,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,0,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,1,1,0,1,0,0},
	{0,0,1,0,0,1,0},
	{0,0,0,0,0,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	/*if (wiringPiSetupGpio () <0){
				printf("Unable to setup wiringPi\n");
				piUnlock(STD_IO_BUFFER_KEY);
				return;
			}*/

	led_display->pantalla=pantalla_inicial;
	led_display->columna_actual=0;

	 //Exploracion filas
		piLock(MATRIX_KEY);

		//Excitacion  filas y columnas
		pinMode (GPIO_LED_DISPLAY_ROW_1, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_2, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_2, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_3, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_3, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_4, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_4, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_5, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_5, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_6, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_6, HIGH);

		pinMode (GPIO_LED_DISPLAY_ROW_7, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_ROW_7, HIGH);

		pinMode (GPIO_LED_DISPLAY_COL_1, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);

		pinMode (GPIO_LED_DISPLAY_COL_2, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);

		pinMode (GPIO_LED_DISPLAY_COL_3, OUTPUT);
		digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);


	led_display->tmr_refresco_display= tmr_new(timer_refresco_display_isr);
	tmr_startms((tmr_t*)(led_display->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);
	printf("\nPULSE LA TECLA 5 PARA EMPEZAR, 0 PARA SALIR\n");
	printf("\nTECLA 1 -- PAUSA");
	printf("\nTECLA 2 -- ARRIBA");
	printf("\nTECLA 4 -- IZQUIERDA");
	printf("\nTECLA 5 -- ACTUALIZA/VOLVER A JUGAR");
	printf("\nTECLA 6 -- DERECHA");
	printf("\nTECLA 8 -- ABAJO");
	printf("\nTECLA 0 -- SALIR");
	printf("\n\n");
	fflush(stdout);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	//LAS FILAS SE APAGAN CON UN 1
	digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
}

void ExcitaColumnas(int columna) {
	switch(columna) {
		case 0:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
				 led_display.columna_actual= 0;
				 piUnlock(MATRIX_KEY);
				 break;
		case 1:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
				 led_display.columna_actual= 1;
				 piUnlock(MATRIX_KEY);
				 break;
		case 2:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
				 led_display.columna_actual= 2;
				 piUnlock(MATRIX_KEY);
				 break;
		case 3:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
				 led_display.columna_actual= 3;
				 piUnlock(MATRIX_KEY);
				 break;
		case 4:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
				 led_display.columna_actual= 4;
				 piUnlock(MATRIX_KEY);
				 break;
		case 5:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
				 led_display.columna_actual= 5;
				 piUnlock(MATRIX_KEY);
				 break;
		case 6:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
				 led_display.columna_actual= 6;
				 piUnlock(MATRIX_KEY);
				 break;
		case 7:
				 piLock(MATRIX_KEY);
				 digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
				 digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
				 led_display.columna_actual= 7;
				 piUnlock(MATRIX_KEY);
				 break;
	}
}


void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	ApagaFilas(led_display);
	if(led_display->columna_actual == 7) {
			led_display->columna_actual = 0;
	} else if (led_display->columna_actual == 0 || led_display->columna_actual == 1|| led_display->columna_actual == 2 || led_display->columna_actual == 3 || led_display->columna_actual == 4 || led_display->columna_actual == 5 || led_display->columna_actual == 6) {
			led_display->columna_actual += 1;
	}
	ExcitaColumnas(led_display->columna_actual);
	piLock(MATRIX_KEY);
	int row = 0;
		for(row = 0; row < 7; row++) {
				if (led_display->pantalla.matriz[led_display->columna_actual][row] == 1) {
					if(row == 0){
						digitalWrite(GPIO_LED_DISPLAY_ROW_1, LOW);
/*						digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);*/
						//ApagaFilas(led_display);
					}
					if(row == 1){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_2, LOW);
/*						digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);*/
						//ApagaFilas(led_display);
					}
					if(row == 2){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_3, LOW);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
						//ApagaFilas(led_display);
					}
					if(row == 3){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_4, LOW);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
						//ApagaFilas(led_display);
					}
					if(row == 4){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_5, LOW);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
						//ApagaFilas(led_display);
					}
					if(row == 5){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_6, LOW);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
						//ApagaFilas(led_display);
					}
					if(row == 6){
						//digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
						//digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
						digitalWrite(GPIO_LED_DISPLAY_ROW_7, LOW);
						//ApagaFilas(led_display);
					}
				//}
			}
		}
	piUnlock(MATRIX_KEY);
}

void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla) {
	int i=0, j=0;
#ifdef __SIN_PSEUDOWIRINGPI__
	printf("\n[PANTALLA]\n");
	fflush(stdout);
	for(j=0;j<NUM_FILAS_DISPLAY;j++) {
		for(i=0;i<NUM_COLUMNAS_DISPLAY;i++) {
			printf("%d", p_pantalla->matriz[i][j]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
	fflush(stdout);
#endif
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
/*	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);*/

	piLock(MATRIX_KEY);
	result = (led_display.flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	/*TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);*/
	//ApagaFilas(&(led_display));

	piLock(MATRIX_KEY);
	led_display.flags &=(~ FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	ActualizaLedDisplay(&(led_display));
	tmr_startms((tmr_t*)(led_display.tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	piLock (MATRIX_KEY);
	led_display.flags |= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock (MATRIX_KEY);
}
