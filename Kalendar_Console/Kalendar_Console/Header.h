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
    } // �����������

    tm when;// ��������� ���������

    bool done; // ������ ������������� ������

    tm repeat; //������ �����. ����� (+8) / ���� (+1) / ������� / ��� (+70)

    int priority; // ��������� ������, 0 - �����������, 1 - ������, 2 - ����� ������

    string name; // ������, ���������� ���� �������� ������
      
    void Write(ofstream& f); // ������ � ����� �����
      
    void Ask(); // ���������� �������������. ����� ��� ����������� ����� ������� SetTime !!!!

    void Print(int k);

    void SetTime(const time_t now) {
        localtime_s(&when, &now);
    }
    // ���������� ����� �������
    void SetRepeat(const time_t now) {
        localtime_s(&repeat, &now);
    }
};