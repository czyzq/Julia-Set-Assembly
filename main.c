#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_color.h"
#include "julia.h"

#define A 800

int al_initialization();

int al_destroy(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* bitmap, ALLEGRO_DISPLAY* display);

void draw(ALLEGRO_BITMAP* bitmap, float x, float y, float przesuniecie);

int main()
{
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_BITMAP* bitmap;

    if(!al_initialization())
        return 1;

    
    display = al_create_display(A, A);	// Utwórz ekran o wymiarach A x A
    if(!display)
    {
        printf("Błąd przy tworzeniu ekranu.\n");
        return 1;
    }

    queue = al_create_event_queue();
    if(!queue)
    {
        printf("Kolejka klawiszy.\n");
        return 1;
    }
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    
    bitmap = al_create_bitmap(A, A);	// Zaladuj bmp na ekran
    if(!bitmap)
    {
        printf("Błąd załadowania bitmapy.\n");
        return 1;
    }
    al_set_target_bitmap(al_get_backbuffer(display));
   
    float x = -1.5, y = -1.5, przesuniecie = 3.0/A;
    
	draw(bitmap, x, y, przesuniecie);	

    while(true)	//pętla oczekująca na klawisze
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
		
		
        if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)	// esc zamyka okno
        {
            al_destroy(queue, bitmap, display);
            return 0;
        }

        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)	// klikniecie przybliza w danym miejscu
		{
			x += event.mouse.x * przesuniecie;
			y += przesuniecie * A;
			y -= event.mouse.y * przesuniecie;	

			przesuniecie /= 2.0; // 2.0 zoom

			x -= przesuniecie * A / 2;
			y -= przesuniecie * A / 2;

			draw(bitmap, x, y, przesuniecie);
		}

	if(event.type==ALLEGRO_EVENT_DISPLAY_CLOSE){ 
		al_destroy(queue,bitmap,display); return 0;
		//break;
	}
    }
}

int al_initialization()
{
    if(!al_init())
    {
        printf("Blad inicjalizacji Allegro.\n");
        return 0;
    }

    if(!al_install_keyboard())
    {
        printf("Blad inicjalizacji klawiatury.\n");
        return 0;
    }

    if(!al_install_mouse())
    {
        printf("Blad inicjalizacji myszy.\n");
        return 0;
    }

    return 1;
}

int al_destroy(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* bitmap, ALLEGRO_DISPLAY* display)
{
    al_destroy_event_queue(queue);
    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
}

void draw(ALLEGRO_BITMAP* bitmap, float x, float y, float przesuniecie)
{
    ALLEGRO_LOCKED_REGION *region = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);

    unsigned* pixels_array;		// pixels_array 32b/4B na pixel
    pixels_array = (unsigned*) region->data;
    pixels_array += A;
    pixels_array -= A * A;

    julia(pixels_array, A, x, y, przesuniecie); //funkcja asm

    al_unlock_bitmap(bitmap);		//zapisac zmiany i wyswitlic zmieniona bitmape
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();
}

