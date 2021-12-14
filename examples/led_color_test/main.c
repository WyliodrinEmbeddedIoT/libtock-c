#include <led_rgb.h>
#include <timer.h>


void show_rainbow( unsigned int count);


int main(void) {
    delay_ms(10);
    show_rainbow(30);
    delay_ms(100);
    led_rgb_off();
     return 0;
}


void show_rainbow( unsigned int count){
    int rgb[3] = {0, 0, 0};
    unsigned int const n = 30; //brightness
    unsigned int const d = 1; //delay ms
    unsigned int j = 0;

    while (j < count) {
        unsigned int i = 0;
        while (i < n) {
            rgb[1] = i;
            rgb[0] = n - i; //red
            led_rgb_on(rgb[0], rgb[1], rgb[2]);
            delay_ms(d);
            i += 1;
        }
        i = 0;
        while (i < n) {
            rgb[2] = i;
            rgb[1] = n - i;
            led_rgb_on(rgb[0], rgb[1], rgb[2]);
            delay_ms(d);
            i += 1;
        }

        i = 0;
        while (i < n) {
            rgb[1] = i;
            rgb[2] = n - i;
            led_rgb_on(rgb[0], rgb[1], rgb[2]);
            delay_ms(d);
            i += 1;
        }
        i = 0;
        while (i < n) {
            rgb[0] = i;
            rgb[1] = n - i;
            led_rgb_on(rgb[0], rgb[1], rgb[2]);
            delay_ms(d);
            i += 1;
        }
        j+=1;
    }
}
