#include <stdio.h>

#include "inc/FreeRTOS.h"
#include "inc/button_driver.h"
#include "inc/driver_glue.h"
#include "inc/i2c_master.h"
#include "inc/led_driver.h"
#include "inc/pindef.h"
#include "inc/stm32f4xx.h"
#include "inc/task.h"

static portTASK_FUNCTION_PROTO(blinkTask, pvParameters) {
  uint8_t buf[32];

  while (1) {
    // Blink green LED
    // GPIO_BANK(PINDEF_GREEN_LED)->ODR ^=
    //   (1 << GPIO_PIN(PINDEF_GREEN_LED));

    // debug_uart_tx("Hello World!\r\n", 14);

    // vTaskDelay(1000);

    /*if (button_driver_get_button_state(0) ||
        button_driver_get_button_state(1)) {
      led_driver_set_mode(PATTERN_BACK_AND_FORTH);
    } else {
      led_driver_set_mode(PATTERN_DEFAULT_CHASER);
    }*/

    void *txn = i2c_issue_transaction(i2c1_driver, 0x1c, "\x60", 1, buf, 32);
    while ((i2c_transaction_status(i2c1_driver, txn) !=
        TRANSACTION_STATUS_DONE) &&
        (i2c_transaction_status(i2c1_driver, txn) !=
          TRANSACTION_STATUS_ERROR)) {}
    i2c_transaction_finish(i2c1_driver, txn);

    vTaskDelay(500);
  }
}

int main(int argc, char **argv) {
  led_driver_init();
  button_driver_init();
  // Enable GPIOD clock
  // RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  // Enable GPIOB clock
  // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  // TODO(rqou): Need some kind of io_init function

  // Set red and green LED GPIO output
  // CONFIGURE_IO(RED_LED);
  // CONFIGURE_IO(GREEN_LED);
  // Turn on red LED
  // GPIO_BANK(PINDEF_RED_LED)->ODR =
  //   (1 << GPIO_PIN(PINDEF_RED_LED));

  // debug_uart_setup();

  // Setup I2C
  i2c1_init();

  xTaskCreate(blinkTask, "Blink", 256, NULL, tskIDLE_PRIORITY, NULL);
  vTaskStartScheduler();
}