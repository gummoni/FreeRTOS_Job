#pragma once

#include "Arduino.h"
#ifdef ESP_H
//for ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#else//ESP_H
//for Arduino
#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif//ESP_H

//最大ジョブ発行数
#define MAX_JOB_COUNT	(32)

//最大ジョブキュー数
#define MAX_QUEUE_COUNT	(8)
