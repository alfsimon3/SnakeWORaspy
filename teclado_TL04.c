#include "teclado_TL04.h"
//#include <wiringPi.h>


int debounceTime[NUM_FILAS_TECLADO]= {0,0,0,0};
//extern TipoTeclado teclado;
//extern int flags;
char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};


// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	/*if (wiringPiSetupGpio () <0){
			printf("Unable to setup wiringPi\n");
			piUnlock(STD_IO_BUFFER_KEY);
			return;
		}*/
	//Comenzamos excitacion por primera columna
	p_teclado->columna_actual= COLUMNA_1;

	//Inicialmente no hay tecla pulsada
	p_teclado->teclaPulsada.col= -1;
	p_teclado->teclaPulsada.row= -1;

    //Exploracion filas
	piLock(KEYBOARD_KEY);

	pinMode (GPIO_KEYBOARD_ROW_1, INPUT);
	pullUpDnControl (GPIO_KEYBOARD_ROW_1, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_1, INT_EDGE_RISING, teclado_fila_1_isr);

	pinMode (GPIO_KEYBOARD_ROW_2, INPUT);
	pullUpDnControl (GPIO_KEYBOARD_ROW_2, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_2, INT_EDGE_RISING, teclado_fila_2_isr);

	pinMode (GPIO_KEYBOARD_ROW_3, INPUT);
	pullUpDnControl (GPIO_KEYBOARD_ROW_3, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_3, INT_EDGE_RISING, teclado_fila_3_isr);

	pinMode (GPIO_KEYBOARD_ROW_4, INPUT);
	pullUpDnControl (GPIO_KEYBOARD_ROW_4, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_4, INT_EDGE_RISING, teclado_fila_4_isr);

	//Excitacion columnas

	pinMode (GPIO_KEYBOARD_COL_1, OUTPUT);
	digitalWrite (GPIO_KEYBOARD_COL_1, HIGH);

	pinMode (GPIO_KEYBOARD_COL_2, OUTPUT);
	digitalWrite (GPIO_KEYBOARD_COL_2, LOW);

	pinMode (GPIO_KEYBOARD_COL_3, OUTPUT);
	digitalWrite (GPIO_KEYBOARD_COL_3, LOW);

	pinMode (GPIO_KEYBOARD_COL_4, OUTPUT);
	digitalWrite (GPIO_KEYBOARD_COL_4, LOW);

	p_teclado->tmr_duracion_columna= tmr_new(timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

	piUnlock(KEYBOARD_KEY);

}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	//TipoTeclado *p_teclado;
	//p_teclado = (TipoTeclado*)(this->user_data);
	switch(columna){
	case 0:
		 piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_1, HIGH);
		 digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_3, LOW);

		 teclado.flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

		 teclado.columna_actual= COLUMNA_1;

		 piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 tmr_startms((tmr_t*)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO); //cambiamos teclado.tmr_duracion_columna por
		 break;
	case 1:
		 piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_2, HIGH);
		 digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_4, LOW);

		 teclado.flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

		 teclado.columna_actual= COLUMNA_2;

		 piUnlock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 tmr_startms((tmr_t*)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
		 break;
	case 2:
		 piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_3, HIGH);
		 digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_1, LOW);

		 teclado.flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

		 teclado.columna_actual= COLUMNA_3;

		 piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 tmr_startms((tmr_t*)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
		 break;
	case 3:
		 piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_4, HIGH);
		 digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
		 digitalWrite(GPIO_KEYBOARD_COL_2, LOW);

		 teclado.flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

		 teclado.columna_actual= COLUMNA_4;

		 piUnlock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

		 tmr_startms((tmr_t*)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
		 break;
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
    piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	result = (teclado.flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
    piLock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	result = (teclado.flags & FLAG_TECLA_PULSADA);
	piUnlock(KEYBOARD_KEY);		//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	//TipoTeclado *p_teclado;
	//p_teclado = (TipoTeclado*)(this->user_data);
	if(teclado.columna_actual == COLUMNA_4) {
			ActualizaExcitacionTecladoGPIO(teclado.columna_actual);
			teclado.columna_actual = COLUMNA_1;

	} else if (teclado.columna_actual == COLUMNA_1 || teclado.columna_actual == COLUMNA_2 || teclado.columna_actual == COLUMNA_3) {
			ActualizaExcitacionTecladoGPIO(teclado.columna_actual);
			teclado.columna_actual += 1;

	}

	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	p_teclado->flags &= (~FLAG_TECLA_PULSADA);

	switch(p_teclado->teclaPulsada.col){
	case 1:
			if(p_teclado->teclaPulsada.row == FILA_1){
				//piLock (KEYBOARD_KEY);
				flags |= FLAG_PAUSA;
				//piUnlock (KEYBOARD_KEY);
/*				piLock(STD_IO_BUFFER_KEY);
				printf("Tecla 1 pulsada(PAUSA)\n");
			    fflush(stdout);
			   	piUnlock(STD_IO_BUFFER_KEY);*/
				p_teclado->teclaPulsada.col =-1;
				p_teclado->teclaPulsada.row =-1;
			}
			if(p_teclado->teclaPulsada.row == FILA_2){
				//piLock (KEYBOARD_KEY);
				flags |= FLAG_MOV_IZQUIERDA;
				//piUnlock (KEYBOARD_KEY);
				/*piLock(STD_IO_BUFFER_KEY);
				printf("Tecla 4 pulsada(IZQUIERDA)\n");
			   fflush(stdout);
			   piUnlock(STD_IO_BUFFER_KEY);*/
			   p_teclado->teclaPulsada.col =-1;
      		   p_teclado->teclaPulsada.row =-1;
			}
			break;
	case 2:
			if(p_teclado->teclaPulsada.row == FILA_1){
				//piLock (KEYBOARD_KEY);
				flags |= FLAG_MOV_ARRIBA;
				//piUnlock (KEYBOARD_KEY);
				/*piLock(STD_IO_BUFFER_KEY);
				printf("Tecla 2 pulsada(ARRIBA)\n");
				fflush(stdout);
				piUnlock(STD_IO_BUFFER_KEY);*/
				p_teclado->teclaPulsada.col =-1;
	      		p_teclado->teclaPulsada.row =-1;
			}
			if(p_teclado->teclaPulsada.row == FILA_2){
				//piLock (KEYBOARD_KEY);
				flags |= FLAG_TIMER_JUEGO;
				//piUnlock (KEYBOARD_KEY);
				/*piLock(STD_IO_BUFFER_KEY);
				printf("Tecla 5 pulsada(ACTUALIZA)\n");
				fflush(stdout);
				piUnlock(STD_IO_BUFFER_KEY);*/
				p_teclado->teclaPulsada.col =-1;
				p_teclado->teclaPulsada.row =-1;
	        }
			if(p_teclado->teclaPulsada.row == FILA_3){
				//piLock (KEYBOARD_KEY);					//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY)
				flags |= FLAG_MOV_ABAJO;
				//piUnlock (KEYBOARD_KEY);
				/*piLock(STD_IO_BUFFER_KEY);
			    printf("Tecla 8 pulsada(ABAJO)\n");
			    fflush(stdout);
			    piUnlock(STD_IO_BUFFER_KEY);*/
				p_teclado->teclaPulsada.col =-1;
				p_teclado->teclaPulsada.row =-1;
		    }
			if(p_teclado->teclaPulsada.row == FILA_4){
				piLock(STD_IO_BUFFER_KEY);
				/*printf("Tecla 0 pulsada(EXIT)\n");
			    fflush(stdout);*/
				exit(0);
			    piUnlock(STD_IO_BUFFER_KEY);
				p_teclado->teclaPulsada.col =-1;
				p_teclado->teclaPulsada.row =-1;
		    }
			break;
	case 3:
			if(p_teclado->teclaPulsada.row == FILA_2){
				//piLock (SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_DERECHA;
				//piUnlock (SYSTEM_FLAGS_KEY);
				//piLock(STD_IO_BUFFER_KEY);
				/*printf("Tecla 6 pulsada(DERECHA)\n");
				fflush(stdout);
				piUnlock(STD_IO_BUFFER_KEY);*/
				p_teclado->teclaPulsada.col =-1;
				p_teclado->teclaPulsada.row =-1;
			}
			break;
	case 4:
		printf("\nKeypress \"%c\"...\n",
				tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
		fflush(stdout);
		break;

	default:
		printf("\nERROR!!! numero invalido de columna (%d)!!!\n", p_teclado->teclaPulsada.col);
		printf("\nERROR!!!(%d) numero invalido de fila !!!\n", p_teclado->teclaPulsada.row);
		fflush(stdout);

		p_teclado->teclaPulsada.col =-1;
		p_teclado->teclaPulsada.row =-1;

		break;
	}

	piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
}





//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	if (millis () <  debounceTime[FILA_1]){
		debounceTime[FILA_1] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

	teclado.teclaPulsada.row = FILA_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;

	piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	debounceTime[FILA_1] = millis () + DEBOUNCE_TIME;
}

void teclado_fila_2_isr (void) {
	if (millis () <  debounceTime[FILA_2]){
		debounceTime[FILA_2] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

	teclado.teclaPulsada.row = FILA_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;

	piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	debounceTime[FILA_2] = millis () + DEBOUNCE_TIME;
	}


void teclado_fila_3_isr (void) {
	if (millis () <  debounceTime[FILA_3]){
		debounceTime[FILA_3] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

	teclado.teclaPulsada.row = FILA_3;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;

	piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	debounceTime[FILA_3] = millis () + DEBOUNCE_TIME;
	}

void teclado_fila_4_isr (void) {
	if (millis () <  debounceTime[FILA_4]){
		debounceTime[FILA_4] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY

	teclado.teclaPulsada.row = FILA_4;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;

	piUnlock(KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	debounceTime[FILA_4] = millis () + DEBOUNCE_TIME;
	}


void timer_duracion_columna_isr (union sigval value) {
	piLock (KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock (KEYBOARD_KEY);	//Cambio SYSTEM_FLAGS_KEY por KEYBOARD_KEY
}
