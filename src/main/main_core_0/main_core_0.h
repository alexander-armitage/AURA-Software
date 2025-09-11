#pragma once

#include "BMP280.h"
#include "FSA8S.h"
#include "I2C.h"
#include "ICM20948.h"
#include "INA219.h"
#include "PCA9685.h"
#include "PID.h"
#include "Quat.h"
#include "esp_timer.h"
#include "utility.h"

namespace main {

constexpr float RAD_TO_DEG = 57.3;
constexpr float DEG_TO_RAD = 0.017452F;

constexpr float PI = 3.1415;

void core_0_task(void* args);

}  // namespace main
