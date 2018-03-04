#pragma once

#include "Arduino.h"
#ifdef ESP_H
//for ESP32
//�ŏ�����FreeRTOS�������Ă���̂Ń��C�u������ǉ�����K�v�Ȃ�
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#else//ESP_H
//for Arduino
//���C�u��������FreeRTOS���C���X�g�[�����鎖
#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif//ESP_H

//try-catch��L���ɂ���ꍇ��
//file::esp32\platform.txt
// -fno-exceptions �� -fexceptions�ɂ���
//#define __ENABLE_EXCEPTION__
