#pragma once
#ifdef ESP_H
//for ESP32
//最初からFreeRTOSが入っているのでライブラリを追加する必要なし
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#else//ESP_H
//for Arduino
//ライブラリからFreeRTOSをインストールする事
//#define configENABLE_BACKWARD_COMPATIBILITY 1
#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif//ESP_H

#define Delay1MS (1 / portTICK_PERIOD_MS)

//try-catchを有効にする場合は
//file::esp32\platform.txt
// -fno-exceptions を -fexceptionsにする
//#define __ENABLE_EXCEPTION__


//Job用共有変数
struct JobInfo {
  bool PauseFlg = false;
  SemaphoreHandle_t PauseEvent;
  bool IsCanceled = false;

  //コンストラクタ処理
  JobInfo() {
    IsCanceled = false;
    PauseFlg = false;
    PauseEvent = xSemaphoreCreateBinary();
  }

  //再開待ち
  void WaitPause() {
    if (PauseFlg) {
      xSemaphoreTake(PauseEvent, portMAX_DELAY);
    }
  }

  //一時停止
  void Pause() {
    PauseFlg = true;
    xSemaphoreTake(PauseEvent, 0);
  }

  //再開
  void Resume() {
    PauseFlg = false;
    xSemaphoreGive(PauseEvent);
  }

  //キャンセル
  void Cancel() {
    IsCanceled = true;
  }
};

struct Dispatcher;

//非同期処理
struct Job {
  SemaphoreHandle_t CompletedEvent = xSemaphoreCreateBinary();
  virtual void Execute() = 0;
  JobInfo* Info = NULL;       //ジョブの共有情報
  Dispatcher* Worker = NULL;  //登録したスケジューラ（本処理を実行する担当者）
  int Progress = 0;           //進捗コード
#ifdef __ENABLE_EXCEPTION__
  int ErrorCode = 0;          //エラーコード

  //処理実行
  void Dispatch() {
    try {
      if (Info->IsCanceled) {
        //キャンセル
      } else {
        Info->WaitPause();
        Execute();
      }
    } catch (int errorCode) {
      ErrorCode = errorCode;
    }
    xSemaphoreGive(CompletedEvent);
  }

  //完了待ち
  void Wait() {
    if (NULL != Worker) {
      //完了待ち
      xSemaphoreTake(CompletedEvent, portMAX_DELAY);
    } else {
      //自力で実行
      Dispatch();
    }

    //エラーがあれば例外スロー
    if (0 != ErrorCode) {
      throw ErrorCode;
    }
  }
#else//__ENABLE_EXCEPTION__
  //処理実行
  void Dispatch() {
    if (Info->IsCanceled) {
      //キャンセル
    } else {
      Info->WaitPause();
      Execute();
    }
    xSemaphoreGive(CompletedEvent);
  }

  //完了待ち
  void Wait() {
    if (NULL != Worker) {
      //完了待ち
      xSemaphoreTake(CompletedEvent, portMAX_DELAY);
    } else {
      //自力で実行
      Dispatch();
    }
  }
#endif//__ENABLE_EXCEPTION__

  //キャンセル
  void Cancel() {
    Info->Cancel();
  }

  //再開
  void Resume() {
    Info->Resume();
  }

  //一時低地
  void Pause() {
    Info->Pause();
  }

  //進捗報告
  void Report(int progress) {
    Progress = progress;
  }

  //同期処理
  void Action(Job* job) {
    job->Wait();
  }

  //並列処理
  void Fork(Job* job1, Job* job2) {
    job1->Wait();
    job2->Wait();
  }

  //並列処理
  void Fork(Job* job1, Job* job2, Job* job3) {
    job1->Wait();
    job2->Wait();
    job3->Wait();
  }

  //並列処理
  void Fork(Job* job1, Job* job2, Job* job3, Job* job4) {
    job1->Wait();
    job2->Wait();
    job3->Wait();
    job4->Wait();
  }

};

//レコード（モジュール）
struct Record {
  //処理移譲(といっても自分で処理する)
  Job* Invoke(Job* sender, Job* job) {
    job->Info = (NULL != sender) ? sender->Info : new JobInfo();
    return job;
  }

  virtual void Initialize() = 0;
  virtual void Dispose() = 0;
};

//非同期実行ディスパッチャ
struct Dispatcher {
#ifdef ESP_H
    //コンストラクタ処理
    Dispatcher(const char* name, int size, int priority, int coreId) {
      //キュー作成とタスク起動
      queueHandler = xQueueCreate(8, sizeof(Job*));  //8個 * 4byte(pointer分)
      xTaskCreatePinnedToCore(TaskMain, name, size, this, priority, &taskHandler, coreId);
    }
#else//ESP_H
    //コンストラクタ処理
    Dispatcher(const char* name, int size, int priority) {
      //キュー作成とタスク起動
      queueHandler = xQueueCreate(8, sizeof(Job*));  //8個 * 4byte(pointer分)
      xTaskCreate(TaskMain, name, size, this, priority, &taskHandler);
      Serial.print(name);
      Serial.println(" start.");
    }
#endif//ESP_H

    //処理をキューの後方に積み込む
    Job* Invoke(Job* sender, Job* job) {
      job->Info = (NULL != sender) ? sender->Info : new JobInfo();
      job->Worker = this;
      xQueueSend(queueHandler, &job, portMAX_DELAY);
      return job;
    }

    //処理をキューの前方に積み込む
    Job* Interrupt(Job* sender, Job* job) {
      job->Info = (NULL != sender) ? sender->Info : new JobInfo();
      job->Worker = this;
      xQueueSendToFront(queueHandler, &job, portMAX_DELAY);
      return job;
    }

  private:
    QueueHandle_t queueHandler;
    TaskHandle_t taskHandler;

    //タスクメイン処理
    static void TaskMain(void* arg) {
      Job* job;
      Dispatcher* self = (Dispatcher*)arg;
      QueueHandle_t queueHandler = self->queueHandler;

      while (true) {
        //キューが溜まったら実行
        xQueueReceive(queueHandler, &job, portMAX_DELAY);
        job->Dispatch();
      }
    }
};

