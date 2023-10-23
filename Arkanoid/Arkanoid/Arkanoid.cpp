#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Juego.h"

using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0

const int maxFrame = 1;
int frameactual = 0;
int contadorDeFrames = 0;
int Delay = 25;
int alturaDeFrame = 1080;
int AnchoDeFrame = 1920;
//Se definen variables que serán usadas para la animación del menú
int main() {
	//Si no se inicializa Allegro se lanza el error
	if (!al_init()) {
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
		return -1;
	}
	//Se instalan los complementos y periféricos
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(7);
	al_install_mouse();

	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
	const int RX = monitor.x2 - monitor.x1;
	const int RY = monitor.y2 - monitor.y1;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);//Se modifica el display para que sea pantalla completa

	ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);

	al_set_window_title(pantalla, "Arkanoid");//Se le pone un título a la ventana
	if (!pantalla)
	{
		//Si el display no se ejecuta se lanza el mensaje de error
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	//Se crean todos los punteros propios de Allegro y se carga el multimedia y los timers
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* timerD = al_create_timer(1.0 / 15);

	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

	ALLEGRO_FONT* fuente1;
	ALLEGRO_FONT* fuente2;
	ALLEGRO_FONT* fuente3;

	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Espacio clone.png");
	ALLEGRO_BITMAP* Flecha = al_load_bitmap("Imagenes/Flecha.png");
	ALLEGRO_BITMAP* Instrucc = al_load_bitmap("Imagenes/Instrucc.png");

	ALLEGRO_SAMPLE* musica = al_load_sample("Musica/Musica2.wav");


	fuente1 = al_load_font("pixel.ttf", 80, NULL);
	fuente2 = al_load_font("pixel.ttf", 50, NULL);
	fuente3 = al_load_font("pixel.ttf", 30, NULL);

	//Se registran las fuentes de eventos(timers y periféricos) en la cola de eventos

	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_timer_event_source(timerD));
	al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());
	al_register_event_source(cola_eventos, al_get_mouse_event_source());


	bool hecho = true;
	int mousex = 0;
	int mousey = 0;
	int a = 0;
	int inicio = 0;
	bool modo;

	//Se obtienen las dimensiones de la pantalla en donde se proyecta el juego para que este se ajuste a las dimensiones
	int X = al_get_display_width(pantalla);
	int Y = al_get_display_height(pantalla);

	//Se reproduce la música del menú principal e inician timers
	al_play_sample(musica, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
	al_start_timer(timer);
	al_start_timer(timerD);
	while (hecho)//Se empieza la cola de eventos
	{
		ALLEGRO_EVENT eventos;
		al_wait_for_event(cola_eventos, &eventos);

		if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES) {
			//Se registra la posición en x y y del mouse
			mousex = eventos.mouse.x;
			mousey = eventos.mouse.y;
		}
		if (eventos.type == ALLEGRO_EVENT_TIMER)
		{
			if (eventos.timer.source == timer)
			{
				if (contadorDeFrames++ >= Delay) {//Se declara un contador de frames que logra avanzar la animación del menú principal
					if (frameactual++ >= maxFrame) {
						frameactual = 0;
					}
					contadorDeFrames = 0;
				}
				//Se dibuja el fondo animado y las opciones del menú en el display
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
				al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Arkanoid");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "DEMO");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (390.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (460.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
			}
		}
		//Si se posiciona el mouse en las coordenadas donde indica la opción jugar
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 260.0 / 768.0) && mousey <= (RY * 290.0 / 768.0))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					//Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
					al_destroy_sample(musica);
					al_destroy_display(pantalla);
					int vida = 3;
					int nivel = 1;
					modo = true;
					while (vida != 0) {
						vida = juego(nivel, vida, modo);
						nivel = nivel + 1;
					}
					main();//Se vuelve a lanzar el menú si se sale del juego
					hecho = false;
				}
			}
		}
		//Si el mouse se posiciona sobre DEMO se pinta de amarillo y si se hace click se lanza el juego pero en modo DEMO que significa máquina vs  máquina
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 330.0 / 768.0) && mousey <= (RY * 360.0 / 768.0))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "DEMO");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					al_destroy_sample(musica);
					al_destroy_display(pantalla);
					int vida = 3;
					int nivel = 1;
					modo = false;
					while (vida != 0) {
						vida = juego(nivel, vida, modo);
						nivel = nivel + 1;
					}
					main();
					hecho = false;
				}
			}
		}
		//Si se posiciona el mouse sobre resultados este se tinta de amarillo y si se hace click lanza el menu de resultados donde se pueden ver todas las partidas anteriores
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (400.0 / 768.0)) && mousey <= (RY * (430.0 / 768.0)))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, RY * (390.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					bool salir = false;
					while (!salir) {
						al_wait_for_event(cola_eventos, &eventos);
						if (eventos.type == ALLEGRO_EVENT_TIMER) {
							if (eventos.timer.source == timer) {
								if (contadorDeFrames++ >= Delay) {
									if (frameactual++ >= maxFrame) {
										frameactual = 0;
									}
									contadorDeFrames = 0;
								}
								//Se dibuja el fondo animado otra vez y se lanzan los resultado y felchas para navegar los resultados así como la tecla atrás para volver al menú principal
								al_clear_to_color(al_map_rgb(0, 0, 0));
								al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
								al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Arkanoid");
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
								al_draw_bitmap(Flecha, X / 2 - 200, (RY * (300.0 / 768.0)), NULL);
								al_draw_bitmap(Flecha, X / 2 - 200, (RY * (500 / 768.0)), ALLEGRO_FLIP_VERTICAL);
								CargarArchivo(X, Y, fuente3, inicio);
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2 - 300, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							}
						}
						if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
						{
							mousex = eventos.mouse.x;
							mousey = eventos.mouse.y;
						}
						//Si el mouse se posiciona en las flechas se pinta de amarillo la flecha y si se presiona se vanza arriba o abajo en los resultados
						if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 300 && mousey <= 330)) {
							al_draw_tinted_bitmap(Flecha, al_map_rgb(250, 250, 25), X / 2 - 200, (RY * (300.0 / 768.0)), NULL);
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									if (inicio >= 1)
										inicio = inicio - 1;

								}
							}
						}
						if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 510 && mousey <= 540)) {
							al_draw_tinted_bitmap(Flecha, al_map_rgb(250, 250, 25), X / 2 - 200, (RY * (500.0 / 768.0)), ALLEGRO_FLIP_VERTICAL);
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									inicio = inicio + 1;
								}
							}
						}
						if ((mousex >= X / 2 - 42 - 300 && mousex <= X / 2 + 42 - 300) && (mousey >= (RY * (520.0 / 768.0)) && mousey <= (RY * (550.0 / 768.0)))) {
							al_draw_text(fuente2, al_map_rgb(250, 250, 15), X / 2 - 300, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									salir = true;
								}
							}
						}
						al_flip_display();
					}
				}
			}
		}
		//Si se presionan las instrucciones se pueden ver dos bitmaps que indican como jugar el juegp
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (470.0 / 768.0)) && mousey <= (RY * (500.0 / 768.0)))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (460.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					bool salir = false;
					while (!salir) {
						al_wait_for_event(cola_eventos, &eventos);
						if (eventos.type == ALLEGRO_EVENT_TIMER) {
							if (eventos.timer.source == timer) {
								if (contadorDeFrames++ >= Delay) {//Se vuelve a lanzar la animación
									if (frameactual++ >= maxFrame) {
										frameactual = 0;
									}
									contadorDeFrames = 0;
								}
								//Se dibujan todos los bitmaps, las instrucciones y títulos
								al_clear_to_color(al_map_rgb(0, 0, 0));
								al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
								al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Arkanoid");
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
								al_draw_bitmap(Instrucc, X / 2 - 200, (RY * (350.0 / 768.0)), NULL);
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							}
						}
						if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
						{
							mousex = eventos.mouse.x;
							mousey = eventos.mouse.y;
						}
						if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * (520.0 / 768.0)) && mousey <= (RY * (550.0 / 768.0)))) {
							//Si se presiona atras sale al menú principal
							al_draw_text(fuente2, al_map_rgb(250, 250, 15), X / 2, 510, ALLEGRO_ALIGN_CENTRE, "Atras");
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									salir = true;
								}
							}
						}
						al_flip_display();
					}
				}
			}
		}
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (540.0 / 768.0)) && mousey <= (RY * (570.0 / 768.0)))) {
			//Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					hecho = false;
				}
			}
		}
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Si se presiona la tecla escape se sale del juego
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
			}
		}
		else if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			hecho = false;
		}
		//juego();
		al_flip_display();
	}
}