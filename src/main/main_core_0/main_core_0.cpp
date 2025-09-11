#include "main_core_0.h"

#include "Madgwick.h"

namespace main {

void core_0_task(void* args) {
  // Initialise queue
  auto controlOutputQueue = static_cast<QueueHandle_t>(args);
  sensor_data my_data{};

  // I2C Bus definition
  i2c::i2c_bus my_bus(SDA, SCL);

  // I2C Device definitions
  bmp280::bmp280 my_bmp(&my_bus);
  icm20948::icm20948 my_icm(&my_bus);
  ina219::ina219 my_ina(&my_bus);
  pca9685::pca9685 my_pca(&my_bus);

  // FS-A8S Receiver definition
  fsa8s::receiver my_receiver(RC_RX);

  // Time handling definitons
  float dt{};
  int64_t last_time = esp_timer_get_time();
  int64_t delta_us{};
  int64_t now{};

  // Code for setup goes here

  while (true) {
    // Calculate delta time
    now = esp_timer_get_time();
    delta_us = now - last_time;
    last_time = now;
    dt = static_cast<float>(delta_us) / 1e6F;

    // Update necessary sensors
    my_icm.update();
    my_icm.update_mag();

    my_receiver.update();

    my_bmp.update();

    // Code for main loop goes here

    // Update queue to Core 1
    xQueueOverwrite(controlOutputQueue, &my_data);
  }
}

}  // namespace main
