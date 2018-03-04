#pragma once

#include "Arduino.h"
#ifdef ESP_H
//for ESP32
//最初からFreeRTOSが入っているのでライブラリを追加する必要なし
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#else//ESP_H
//for Arduino
//ライブラリからFreeRTOSをインストールする事
#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif//ESP_H

//try-catchを有効にする場合は
//file::esp32\platform.txt
// -fno-exceptions を -fexceptionsにする
//#define __ENABLE_EXCEPTION__
