/**
 * The code to run on core 1 of the ESP32-S3 processor.
 *
 */

#include "main_core_1.h"

namespace main {

void core_1_task(void* args) {
  // Initialise queue
  QueueHandle_t controlOutputQueue = (QueueHandle_t)args;
  sensor_data my_data;

  // SPI Device definitions
  spi::create_spi_bus(SPI2_HOST, MOSI, MISO, SCLK);
  flash::flash my_flash(FLASH_CS, SPI2_HOST);
  sd_card::sd_card my_sd_card(SD_CARD_CS, SPI2_HOST);

  // Code for setup goes here

  while (true) {
    // Code for main loop goes here

    if (xQueueReceive(controlOutputQueue, &my_data, MAX_DELAY)) {
      // Code for data processing goes here
    }
  }
}

}  // namespace main
