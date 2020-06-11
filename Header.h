#pragma once
#define MAX_NAME_SIZE 40 // ������ ������� � ������
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
        SetTime(time(NULL)); // �� ���������, ���� ������� = ������� �������� �������
        SetRepeat(0); // �� ��������� �� ���������
    } // �����������

    tm when;// ��������� ��������� = ����

    time_t during; // ������������

    bool done; // ������ ������������� ������

    tm repeat; //������ �����. ����� (+8) / ���� (+1) / ������� / ��� (+70)

    int priority; // ��������� ������, 0 - �����������, 1 - ������, 2 - ����� ������

    string name; // ������, ���������� ���� �������� ������

    tm doneTime;
      
    void Write(ofstream& f); // ������ � ����� �����
      
    void Ask(); // ���������� �������������. ����� ��� ����������� ����� ������� SetTime !!!!

    void PrintName();

    void Print();

    void PrintDuring();

    void PrintTime(char sw); // ������� ����� � ���������� ���������� �������������� �� ����� ������� ��������

    void SetTime(const time_t now) {
        localtime_s(&when, &now); // ���������� ��������� ��������� tm �� ������� � �������
        when.tm_sec = 0;
        during = ((short)24-when.tm_hour) * (short)3600 - (when.tm_min) * (short)60;  // �� ����� ��� - �� ���������
    }
    // ���������� ����� �������
    void SetRepeat(const time_t now) {
        localtime_s(&repeat, &now);
        repeat.tm_sec = 0;
    }
    void SetDone(const time_t now) {
        localtime_s(&doneTime, &now);
    }
};

