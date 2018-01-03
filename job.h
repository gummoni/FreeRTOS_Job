#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define Delay1MS (1 / portTICK_RATE_MS)

class job {
  private:
    bool isPause;                       //一時停止
    bool isCompleted;                   //完了
    bool isCancel;                        //キャンセル
    void (*func )(job* );               //実行する処理

  public:
    void* sender;                       //呼び出し元インスタンス
    void* arg;                          //引数
    void* result;                       //戻り値
    int state;                          //状態
    int errorCode;                      //エラーコード

    //受け側
    void invoke(void* sender, void (*func)(job*), void* arg, void* result);
    void dispatch();                    //実行

    //送り側
    void cancel();			                  //キャンセル
    void pause();			                  //一時停止
    void resume();			                //再開
    void report(int state);             //状態通知
    void wait();			                  //完了待ち
    void action(job* job);              //実行
    void fork(job** jobs, int length);  //実行
};

