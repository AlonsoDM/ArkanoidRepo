#pragma once

#include "FuncionesJuego.h"

using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0

int juego(int nivel, int vida, bool modo)
{
	al_uninstall_mouse();
	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);
	const int RX = monitor.x2 - monitor.x1;
	const int RY = monitor.y2 - monitor.y1;

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FULLSCREEN);
	ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
	//al_set_window_position(pantalla, 0, 0);
	al_set_window_title(pantalla, "Space Invaders");

	if (!pantalla)
	{
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	ALLEGRO_FONT* fuente1;
	ALLEGRO_FONT* fuente2;
	ALLEGRO_KEYBOARD_STATE teclado;
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	int rapidez = 15;
	ALLEGRO_TIMER* timerD = al_create_timer(1.0 / rapidez);

	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
	ALLEGRO_BITMAP* naveE[7];
	naveE[0] = al_load_bitmap("Imagenes/TIE1.png");
	naveE[1] = al_load_bitmap("Imagenes/EXP1.png");
	naveE[2] = al_load_bitmap("Imagenes/EXP2.png");
	naveE[3] = al_load_bitmap("Imagenes/EXP3.png");
	naveE[4] = al_load_bitmap("Imagenes/EXP4.png");
	naveE[5] = al_load_bitmap("Imagenes/EXP5.png");
	naveE[6] = al_load_bitmap("Imagenes/EXP6.png");


	ALLEGRO_BITMAP* naveJ0 = al_load_bitmap("Imagenes/XWING0.png");
	ALLEGRO_BITMAP* naveJ1 = al_load_bitmap("Imagenes/XWING1.png");
	ALLEGRO_BITMAP* BalaE = al_load_bitmap("Imagenes/BALAE.png");
	ALLEGRO_BITMAP* BalaA = al_load_bitmap("Imagenes/BALAA.png");
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Fondo.png");

	ALLEGRO_BITMAP* ESCUDO = al_load_bitmap("Imagenes/ESCUDOS SEGMENTOS.png");

	ALLEGRO_SAMPLE* musica = al_load_sample("Musica/Musica.wav");
	ALLEGRO_SAMPLE* TIES = al_load_sample("Musica/Tie fighter blaster.wav");
	ALLEGRO_SAMPLE* XWINGS = al_load_sample("Musica/X wing fighter blaster.wav");
	ALLEGRO_SAMPLE* Boom = al_load_sample("Musica/explo.wav");


	fuente1 = al_load_font("pixel.ttf", 40, NULL);
	fuente2 = al_load_font("pixel.ttf", 30, NULL);

	al_register_event_source(cola_eventos, al_get_timer_event_source(timerD));
	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());

	//Se declaran las variables a utilizar
	enum Direccion { DERECHA, IZQUIERDA };
	int Dir = 3;
	bool hecho = true;
	bool dibujar = true;
	int ultimo;
	int cool = 0;
	int numEnemigos = 55;
	int listaNaves[57];
	listaNaves[55] = 55;//LLeva la cantidad de naves activas actualizada
	listaNaves[56] = 55;//Lleva la cantidad de naves activas sin actualizar
	char buffer[20];
	char buffer2[3];
	char buffer3[3];
	int baja;
	int bajax;
	int bajay;
	int cont = 0;
	int parpadeo = 0;

	int movF = 0;

	int movRand = 0;
	srand(time(NULL));
	movRand = RX / 2 - 350 + rand() % 660 + 40;
	_itoa_s(nivel, buffer3, 10);
	int salida = 3;

	nave jugador;
	bala Disp;
	bala BalaEnemigo;
	Disp.estado = false;
	PtrNave Enemigos = NULL;
	escudo ESCUDOS[30];


	//Se inicializa los enemigos
	inicializarEnemigos(Enemigos, RX);
	//Se inicializa el jugador
	inicializarJugador(jugador, RX);
	jugador.codigo = vida;
	//Se inicializa los escudos
	crearEscudo(ESCUDOS, RX, RY);


	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_rest(1);

	al_play_sample(musica, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	//Se inician los timers
	al_start_timer(timerD);
	al_start_timer(timer);

	while (hecho)
	{
		ALLEGRO_EVENT eventos;
		al_wait_for_event(cola_eventos, &eventos);

		//Con el condicional si el modo es false se inhabilita el teclado y la nave se mueve aleatoriamente
		if (modo == false) {//Si el modo es false la nave se mueve aleatoriamente
			//Se habilita la tecla ESCAPE para salir del demo
			if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (eventos.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					hecho = false;
					salida = 0;
					break;
				}
			}
			//Se llama a CrearBala cada vez que la bala se encuentre inactiva y el jugador este activo
			if (Disp.estado == false && jugador.estado == true) {
				CrearBala(Disp, jugador, 15);
				al_play_sample(XWINGS, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			if (jugador.x - 5 > movRand) {
				Dir = IZQUIERDA;
			}
			else if (jugador.x + 5 < movRand) {
				Dir = DERECHA;
			}
			else {
				Dir = 3;
			}
			if (Dir == 3) {
				movRand = RX / 2 - 350 + rand() % 660 + 40;
			}
		}
		else {//Si el modo es true se habilita el teclado
			if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
			{//Para las teclas presionadas:
				switch (eventos.keyboard.keycode) {
					//Si se preciona ESCAPE se sale del juego
				case ALLEGRO_KEY_ESCAPE:
					hecho = false;
					salida = 0;
					break;
					//Si se preciona la flecha izquierda se asigna a la direccion Izquierda
				case ALLEGRO_KEY_LEFT:
					Dir = IZQUIERDA;
					break;
					//Si se preciona la flecha derecha se asigna a la direccion Derecha
				case ALLEGRO_KEY_RIGHT:
					Dir = DERECHA;
					break;
					//Se se preciona espacio y la bala se encuentra inactiva y el jugador está activo, se llama a CrearBala y se reproduce el sonido de disparo del jugador
				case ALLEGRO_KEY_SPACE:
					if (Disp.estado == false && jugador.estado == true) {
						CrearBala(Disp, jugador, 15);
						al_play_sample(XWINGS, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					break;
				}
			}
			//Para las teclas que  o estan pulsadas (Las flechas) se asigna a la direccion un valor que no coincida con Derecha o Izquierda
			if (eventos.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (eventos.keyboard.keycode) {
				case ALLEGRO_KEY_LEFT:
					Dir = 3;
					break;
				case ALLEGRO_KEY_RIGHT:
					Dir = 3;
					break;
				}
			}
		}
		//Se ejecutan los eventos del timer
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			//Eventos de timerD
			if (eventos.timer.source == timerD) {
				moverEnemigos(Enemigos, numEnemigos, RX);
			}
			//Eventos de timer
			if (eventos.timer.source == timer) {
				switch (Dir) {
					//Si la direccion es Izquierda se mueve el jugador hacia la izquierda
				case IZQUIERDA:
					if (jugador.x >= RX / 2 - ResX / 2 + 15)
						jugador.x = jugador.x - 5;
					break;
					//Si la direccion es Derecha se mueve el jugador hacia la derecha
				case DERECHA:
					if (jugador.x <= RX / 2 + ResX / 2 - 15)
						jugador.x = jugador.x + 5;
					break;
				}

				//Se limpia la pantalla
				al_clear_to_color(al_map_rgb(0, 0, 0));
				//Se imprime el fondo que se va a desplazar hacia arriba por cada vez llamada
				al_draw_scaled_bitmap(Fondo, 0, movF, 1920, 1080, 0, 0, RX, RY, NULL);
				if (movF == 1080) {
					movF = 0;
				}
				movF++;


				//Se recorre la lista de naves y se muestra en pantalla las que esten activas
				PtrNave	Aux = NULL;
				Aux = Enemigos;
				while (Aux != NULL) {
					if (Aux->estado == true) {
						if (Aux->y >= 700) {
							hecho = false;
							salida = 0;
						}
						al_draw_bitmap(naveE[0], Aux->x - diametro / 2, Aux->y - diametro / 2, NULL);
					}
					Aux = Aux->Siguiente;
				}
				//Para el contador cool cuando sea mayor o igual a 100 se activa el jugador, se dirminuye la vida en 1 y se reinicia el contador
				if (cool >= 100) {
					jugador.estado = true;
					jugador.codigo = jugador.codigo - 1;
					cool = 0;
				}
				//Si cool es menor a 100, si por medio de modulos se muestra o no la nave en pantalla creando un parpadeo de la nave
				else {
					if (cool % 8 == 0 || cool % 8 == 7 || cool % 8 == 6 || cool % 8 == 5)
						al_draw_bitmap(naveJ0, jugador.x - 32, jugador.y - 32, NULL);
				}
				//Si la cantidad de naves activas actualizada es diferente a la cantudad de naves activas anterior se actualiza al contador de naves
				//Si la cantidad de naves activas es cero, termina el juego y declara salida como la cantidad de vidas del jugador
				if (listaNaves[55] != listaNaves[56]) {
					listaNaves[56] = listaNaves[55];
					if (listaNaves[55] != 0) {
						rapidez = 8 + (100 / listaNaves[55]);
					}
					else {
						salida = jugador.codigo;
						hecho = false;
					}
					//al_set_timer_speed(timerD, 1.0 / rapidez);
				}

				//******************Escudos******************
				//Se llama da dibujarEscudos, colisionEscudos y colisionEscudosBala
				dibujarEscudos(ESCUDOS, ESCUDO);
				colisionEscudo(ESCUDOS, BalaEnemigo);
				colisionEscudo(ESCUDOS, Disp);

				//******************Disparo Aliado******************
				//Se llama a colision y se asigna a la variable baja el codigo de la nave destruida
				baja = colision(*Enemigos, Disp);
				numEnemigos = contarEnemigos(Enemigos);
				//Se llama a pintarBala que muestra en pantalla la bala aliada disparada
				pintarBala(Disp, BalaA);

				//******************Disparo Enemigo******************
				//Se llama a disparoEnemigo que escoge aleatoriamente una nave que dispare, a pintarBala que muestra la bala enemiga disparada y a colsion entre el jugador y la bala disparada
				disparoEnemigo(Enemigos, BalaEnemigo, -5, jugador, listaNaves, TIES);
				pintarBala(BalaEnemigo, BalaE);
				colision(jugador, BalaEnemigo);

				//Si el codigo de la nave destruida es distinto a 56 (Si es 56 no se destruyo ninguna nave) se reproduce el sonido de explosion y se asingna a bajax y bajay las coordenadas de la nave donde exploto
				if (baja != 56) {
					al_play_sample(Boom, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					PtrNave Aux;
					Aux = Enemigos;
					while (Aux != NULL) {
						if (Aux->codigo == baja) {
							bajax = Aux->x;
							bajay = Aux->y;
						}
						Aux = Aux->Siguiente;
					}
					baja = 56;
					//Se inicia el contador cont en 12
					cont = 12;
				}

				//Para cont distinto a cero se imprime bitmaps de explosion en la posicion bajax, bajay
				if (cont != 0) {
					switch (cont) {
					case 12:
						al_draw_bitmap(naveE[1], bajax - 32, bajay - 32, NULL);
						break;
					case 11:
						al_draw_bitmap(naveE[1], bajax - 32, bajay - 32, NULL);
						break;
					case 10:
						al_draw_bitmap(naveE[2], bajax - 32, bajay - 32, NULL);
						break;
					case 9:
						al_draw_bitmap(naveE[2], bajax - 32, bajay - 32, NULL);
						break;
					case 8:
						al_draw_bitmap(naveE[3], bajax - 32, bajay - 32, NULL);
					case 7:
						al_draw_bitmap(naveE[3], bajax - 32, bajay - 32, NULL);
						break;
					case 6:
						al_draw_bitmap(naveE[4], bajax - 32, bajay - 32, NULL);
						break;
					case 5:
						al_draw_bitmap(naveE[4], bajax - 32, bajay - 32, NULL);
						break;
					case 4:
						al_draw_bitmap(naveE[5], bajax - 32, bajay - 32, NULL);
						break;
					case 3:
						al_draw_bitmap(naveE[5], bajax - 32, bajay - 32, NULL);
						break;
					case 2:
						al_draw_bitmap(naveE[6], bajax - 32, bajay - 32, NULL);
						break;
					case 1:
						al_draw_bitmap(naveE[6], bajax - 32, bajay - 32, NULL);
						break;
					}
					cont--;
				}

				//Colicion de naves
				//colisionNaves(Enemigos, jugador);
				if (jugador.codigo == 0) {
					salida = 0;
					hecho = false;
				}

				//********************Estadisticas***************************
				//Se muestra en pantalla las estadisticas de la partida
				al_draw_filled_rectangle(0, 0, RX, 45, al_map_rgb(0, 0, 0));
				al_draw_text(fuente1, al_map_rgb(255, 255, 255), RX / 2, 5, ALLEGRO_ALIGN_CENTRE, "SPACE INVADERS");


				_itoa_s((nivel - 1) * 550 + 10 * (55 - listaNaves[55]), buffer, 10);
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "PUNTAJE: ");
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), 100, 10, ALLEGRO_ALIGN_LEFT, buffer);

				al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX - 80, 10, ALLEGRO_ALIGN_RIGHT, "Nivel: ");
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX - 75, 10, ALLEGRO_ALIGN_LEFT, buffer3);


				_itoa_s(jugador.codigo, buffer2, 10);
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), 10, RY - 40, ALLEGRO_ALIGN_LEFT, "VIDAS: ");
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), 80, RY - 40, ALLEGRO_ALIGN_LEFT, buffer2);

				al_draw_text(fuente2, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "Presione ESCAPE para salir");

				//Se muestra en pantalla la cantidad de vidas restantes por medio de imagenes de la nave
				for (int i = jugador.codigo - 1; i >= 1; i--) {
					al_draw_bitmap(naveJ1, 60 + 40 * i, RY - 40, NULL);
				}

				//Si el jugador esta activo se muestra en pantalla
				if (jugador.estado == true) {
					al_draw_bitmap(naveJ0, jugador.x - 32, jugador.y - 32, NULL);
				}
				//Si el jugador esta inactivo se aumenta en uno el contador cool
				else {
					cool = cool + 1;
				}
				//Si la vida del jugador es 0, se sale del ciclo y termina el juego
				if (jugador.codigo == 0) {
					//Se asigna salida igual a cero
					salida = 0;
					hecho = false;
				}
			}
		}
		al_flip_display();


	}
	//Si la salida es igual a cero (El jugador perdio la partida)
	if (salida == 0) {
		//Si es el juego completo se procede con el guardado de los resultados
		if (modo == true) {
			int seguir = true;
			char puntaje[20] = "Puntaje: ";
			strcat_s(puntaje, 20, buffer);
			char nombre[40];
			nombre[0] = char(95);
			char letra;
			int pos = 0;
			int contador = 0;
			int contador2 = 0;
			//nombre[8 + pos] = NULL;
			while (seguir) {
				ALLEGRO_EVENT eventoos;
				al_wait_for_event(cola_eventos, &eventoos);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				if (eventoos.type == ALLEGRO_EVENT_KEY_CHAR) {
					//Para los eventos de teclado si pos, la ubiciacion del caracter es menor a 40, se procede a escribir el nombre
					if (pos != 40) {
						//Si se preciona la tecla BACKSPACE (Borrar) se porra el caracter de la posicion actual y se disminuye en 1 la posicion
						if (eventoos.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
							if (pos != 0) {
								nombre[pos] = NULL;
								pos = pos - 1;

							}
						}
						//Se se preciona ENTER se procede termina el ciclo
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_ENTER) {
							seguir = false;
						}
						//Se inhabilitan las teclas LEFT, RIGHT, TAB, ESCAPE
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_LEFT) {
						}
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
						}
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_TAB) {
						}
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						}
						//Si se preciona espacio se procede a imprimer "_"
						else if (eventoos.keyboard.keycode == ALLEGRO_KEY_SPACE) {
							nombre[pos] = char(95);
							pos = pos + 1;
						}
						else {
							//Se no se preciona alguna de las teclas anteriores (se preciona un caracter unicode) se procede a escribirlo en el nombre
							if (eventoos.keyboard.unichar != ALLEGRO_KEY_BACKSPACE) {
								nombre[pos] = eventoos.keyboard.unichar;
								pos = pos + 1;
							}
						}
					}
				}
				//Para señalar la ubicacion de escritura se coloca un caracter al final del nombre
				nombre[pos] = '<';
				nombre[pos + 1] = NULL;
				//Muestra en pantalla el puntaje y el nombre escrito
				al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2 - 250, ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2 - 150, ALLEGRO_ALIGN_CENTRE, puntaje);
				al_draw_text(fuente2, al_map_rgb(255, 255, 20), RX / 2, RY / 2, ALLEGRO_ALIGN_CENTRE, nombre);
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Presione Enter para finalizar");
				al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2 - 50, ALLEGRO_ALIGN_CENTRE, "Ingrese su nombre");
				if (nombre[0] == char(95)) {
					al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2, ALLEGRO_ALIGN_CENTRE, "Escriba su nombre");
				}
				al_flip_display();
			}
			//Se el nombre no es vacio se borra lo que no haya escrito el usuario (como el caracter para ubicar)
			if (pos != 0) {
				nombre[pos] = NULL;
			}
			else {//Si el usuario no escribe el nombre so coloca un signo de interrogacion
				nombre[0] = '?';
			}
			//Se llama a CrearArchivo con los datos obtenidos
			CrearArchivo(buffer, nombre);
		}
	}
	else {//Se salida es distinto a cero, el usuario debe presionar ENTER para continuar con el siguiente nivel
		int seguir = true;
		while (seguir) {
			ALLEGRO_EVENT eventoos;
			al_wait_for_event(cola_eventos, &eventoos);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2 - 50, ALLEGRO_ALIGN_CENTRE, "PRECIONE ENTER PARA CONTINUAR");
			if (eventoos.type == ALLEGRO_EVENT_KEY_UP) {
				if (eventoos.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					seguir = false;
				}
			}
			al_flip_display();

		}
	}
	//Se destruyen las variables incializadas de Allgero
	al_destroy_sample(musica);

	al_destroy_timer(timer);
	al_destroy_timer(timerD);

	al_destroy_font(fuente1);
	al_destroy_font(fuente2);

	al_destroy_display(pantalla);

	al_destroy_bitmap(naveE[0]);
	al_destroy_bitmap(naveE[1]);
	al_destroy_bitmap(naveE[2]);
	al_destroy_bitmap(naveE[3]);
	al_destroy_bitmap(naveE[4]);
	al_destroy_bitmap(naveE[5]);
	al_destroy_bitmap(naveE[6]);

	al_destroy_bitmap(naveJ0);
	al_destroy_bitmap(naveJ1);
	al_destroy_bitmap(BalaE);
	al_destroy_bitmap(BalaA);
	al_destroy_bitmap(Fondo);
	al_destroy_bitmap(ESCUDO);

	al_destroy_event_queue(cola_eventos);

	return salida;
}