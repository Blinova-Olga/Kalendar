#pragma once
#define MAX_NAME_SIZE 40 // Ширина колонки с именем
#define NUM 100
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
using namespace std;

string weekday(int day);

void Quick_sort(int a[NUM][2], int l, int r);

class Case {
public:
    Case() {
        priority = 0;
        name = "";
        done = false;
        SetTime(time(NULL)); // По умолчанию, срок задаётся = времени создания объекта
        SetRepeat(0); // По умолчанию не повторять
    } // Конструктор

    tm when;// Временная структура = срок

    time_t during; // Длительность

    bool done; // Маркер завершённости задачи

    tm repeat; //Повтор через. часов (+8) / дней (+1) / месяцев / лет (+70)

    int priority; // Приоритет задачи, 0 - нейтральная, 1 - важная, 2 - очень важная

    string name; // Строка, содержащая само название задачи

    tm doneTime;
      
    void Write(ofstream& f); // Запись в конец файла
      
    void Ask(); // Заполнение пользователем. Перед ним обязательно нужно сделать SetTime !!!!

    void PrintName();

    void Print();

    void PrintDuring();

    void PrintTime(char sw); // Выводит время и Возвращает количество использованных на вывод времени символов

    void SetTime(const time_t now) {
        localtime_s(&when, &now); // Заполнение временной структуры tm по времени в секндах
        when.tm_sec = 0;
        during = ((short)24-when.tm_hour) * (short)3600 - (when.tm_min) * (short)60;  // До конца дня - по умолчанию
    }
    // Заполнение полей времени
    void SetRepeat(const time_t now) {
        localtime_s(&repeat, &now);
        repeat.tm_sec = 0;
    }
    void SetDone(const time_t now) {
        localtime_s(&doneTime, &now);
    }
};

