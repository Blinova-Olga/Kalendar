#pragma once
#define MAX_NAME_SIZE 40
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
using namespace std;

string weekday(int day);

class Case {
public:

    Case() {
        priority = 0;
        name = "";
        done = false;
        SetTime(time(NULL));
        SetRepeat(0);
    } // Конструктор

    tm when;// Временная структура

    bool done; // Маркер завершённости задачи

    tm repeat; //Повтор через. часов (+8) / дней (+1) / месяцев / лет (+70)

    int priority; // Приоритет задачи, 0 - нейтральная, 1 - важная, 2 - очень важная

    string name; // Строка, содержащая само название задачи
      
    void Write(ofstream& f); // Запись в конец файла
      
    void Ask(); // Заполнение пользователем. Перед ним обязательно нужно сделать SetTime !!!!

    void Print(int k);

    void SetTime(const time_t now) {
        localtime_s(&when, &now);
    }
    // Заполнение полей времени
    void SetRepeat(const time_t now) {
        localtime_s(&repeat, &now);
    }
};