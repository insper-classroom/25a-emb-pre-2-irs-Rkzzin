#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int red_flag = 0;
volatile int green_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    red_flag = 1;
  } else if (events == 0x8) {
    green_flag = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);

  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  int led_r = 0;
  int led_g = 0;


  while (true) {
    if (green_flag) {
      led_g = !led_g;
      gpio_put(LED_PIN_G, led_g);

      green_flag = 0;
    }
    if (red_flag) {
      led_r = !led_r;
      gpio_put(LED_PIN_R, led_r);

      red_flag = 0;
    }
  }
}
