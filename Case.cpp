#include "Header.h"

void Case::Write(ofstream& f) { // ������ � ����� ����� (when during repeat done doneTime priority name)
    f << mktime(&when) << ' ';
    f << during << ' ';
    f << mktime(&repeat) << ' ';
    f << done << ' ';
    f << mktime(&doneTime) << ' ';
    f << priority << ' ';
    f << name;
}

void Case::Ask() {
    cout << "���: ";
    getline(cin, name);
    cout<<"��������� (0 - ������, 1 - �������, 2 - �������): ";
    if (cin >> priority) {
        if (priority > 2 || priority < 0) {
            cout << "��������� ���. � ��������: [0,2]" << endl;
            priority = 0;
        }
    }
    else {
        cout << "�� ����� ��������� �����������." << endl;
        cin.clear();
    }

    cout<<"����� ������ ( ��� ��� ). ����� �� ������� �������� ����������, ����������� \"-\": "<<endl;
    int p=0;
    if (cin >> p) { 
        if (p <= 23 && p >= 0) when.tm_hour = p;
        else cout << "���� ���. � ��������: [0,23]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) { 
        if(p <= 59 && p >= 0) when.tm_min = p;
        else cout << "������ ���. � ��������: [0,59]" << endl;
    }
    else cin.clear();
    p = 0;

    cout << "���� ( ���� ����� ��� ): " << endl;
    if (cin >> p) {
        if (p <= 31 && p >= 1) when.tm_mday = p;
        else cout << "��� ���. � ��������: [1,31]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 12 && p >= 1) when.tm_mon = p - 1;
        else cout << "������ ���. � ��������: [1,12]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p >= 1900) when.tm_year = p - 1900;
        else cout << "��� >= 1900" << endl;
    }
    else cin.clear();
    p = 0;

    cout << "������������ ( ���� ����� ����� ): " << endl;
    int u=0;
    if (cin >> p) {
         u = p*86400;
    }
    else cin.clear();
    if (cin >> p) {
        u = u + p * 3600;
    }
    else cin.clear();
    if (cin >> p) {
        u = u + p * 60;
    }
    else cin.clear();
    if (u != 0) during = u;
    p = 0;


    cout << "������ ����� ������ (����� ���� ������� ���). ��� ���������� ��������� \"- - - -\": " << endl;
    if (cin >> p) {
        if (p <= 23 && p >= 0) repeat.tm_hour = p + 8;
        else cout << "���� ���. � ��������: [0,23]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 30 && p >= 0) repeat.tm_mday = p + 1;
        else cout << "��� ���. � ��������: [0,30]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 11 && p >= 0) repeat.tm_mon = p;
        else cout << "������ ���. � ��������: [0,11]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p >= 0) repeat.tm_year = 70 + p;
        else cout << "��� >= 0" << endl;
    }
    else {
        cin.clear();
        cin.unget(); // ��� ���� ����� ��� ����� "-" cin �� ���� ���.
        cin.ignore(1, ' ');
    }
    p = 0;
    cout << "������!" << endl;
} 

void Case::PrintName() {
    if (name.size() <= MAX_NAME_SIZE) {
        cout.setf(ios::left);
        cout << setfill('-') << setw(MAX_NAME_SIZE) << name << " ; ";
        cout.unsetf(ios::left);
    }
    else cout << name << " ; "; //����� �����
}

void Case::PrintTime(char sw) {
    int k=0;
    switch (sw)
    {
    case 'm': // 23.05
        cout << setfill('0') << setw(2) << when.tm_mday << ".";
        cout << setfill('0') << setw(2) << when.tm_mon+1 << " ";
        k += 6;
    case 'w': // ��
        cout << weekday(when.tm_wday) << " ";
        k += 3;
    case 'h': // 12:00
        cout << setfill('0') << setw(2) << when.tm_hour << ":";
        cout << setfill('0') << setw(2) << when.tm_min << " ";
        k += 6;
        break;
    case 'y': // 31.01.2022
        cout << setfill('0') << setw(2) << when.tm_mday << ".";
        cout << setfill('0') << setw(2) << when.tm_mon + 1 << ".";
        cout << setfill('0') << setw(2) << when.tm_year + 1900 << " ";
        k = 11;
        break;
    default:
        break;
    } 
    cout << setfill(' ') << setw((int)15-k) <<"";
}
void Case::PrintDuring() {
    int d = mktime(&when)+during-(time(NULL)/(int)60)*(int)60;
    cout << "��������: ";
    if (d >= 86400) {
        cout << d / 86400 << " �. ";
        d = d % 86400;
    }
    if (d >= 3600) {
        cout << d / 3600 << " �. ";
        d = d % 3600;
    }
    if (d >= 60) {
        cout << d / 60 << " ���. ";
    }
}

void Case::Print() {
    switch (priority)
    {
    case 0:
        cout << "������ ��������� ";
        break;
    case 1:
        cout << "������� ���������";
        break;
    case 2:
        cout << "������� ���������";
        break;
    default:
        break;
    } // ���������
    cout << " ; ������: ";
    if (mktime(&repeat) == 0) cout << "���";
    else {
        cout << "����� "; 
        if (repeat.tm_year != 70) cout << repeat.tm_year - 70 << " �. ";
        if (repeat.tm_mon != 0) cout << repeat.tm_mon << " ���. ";
        if (repeat.tm_mday != 1) cout << repeat.tm_mday - 1 << "�. ";
        if (repeat.tm_hour != 8) cout << repeat.tm_hour-8<< " �.";
    }
}