#define _CRT_SECURE_NO_WARNINGS
#define __STDC_WANT_LIB_EXT1__ 1 
// ��� ������� localtime_s

#include "Header.h"

int main() {
    system("chcp 1251");
    system("cls");
    const time_t TheTime = time(NULL);
    tm CurTime;
    localtime_s(&CurTime, &TheTime); // ���������� ����� ������� ���������

    int NUM=100; // ���������� ����� (��������� �������)
    int N = 0; // ���������� �������� ����� � �������

    Case* a = new Case[NUM]; // �������� �������

    fstream file;  
    file.open("data.txt");
    if (!file) { cout << "Error. File \"data.dat\" is removed." << endl; return 1;}

    int TTime; // ������ �����
    while (!file.eof()&&N<NUM) { // ���� �� ����� �����
        file >> TTime;
        a[N].SetTime(TTime);
        file >> a[N].done;
        file >> TTime;
        a[N].SetRepeat(TTime);
        file >> a[N].priority;
        file.seekg(1, ios::cur); // ���������� ������ � ������ ������
        getline(file,a[N].name);
        N++;
    }
    file.close();
    // �������� �����
    int if_year=0;
    int n=0;
    char c = ' ';
    int Mk;
    string key;
    int* Group = new int[NUM]; // ������: -1 - ������������, 0 - �������, 1 - ������, 2 - ������������ 
    for (int i = 0; i < NUM; i++) Group[i] = -5;
    cout << "������������! �������:" << endl << "����� - ��������� ������������� " << endl << "������� - ������ ����� ������" << endl << "����� - ���������� ��� ������" << endl;
    cout << "������� n - �������� n-�� ������ ��� �����������" << endl << "������� n - ������� n-�� ������" << endl << "�������� n - ������������� n-�� ������" << endl;
    while (1) {
        cout << ">> ";
        cin >> key;
        if (key == "�����") break;
        else if (key == "�������") {
            if (N < NUM - 3) {
                a[N].Ask();
                N++;
            }
            else cout << "�������� ����� �� ���������� �����." << endl;
            cout << endl;
        }
        else if (key == "�����") {
            cout << "\t����������� ������:" << endl;
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (a[i].done) {
                    cout << i << ") ";
                    a[i].Print(0);
                    cout << endl;
                }
            }
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (a[i].done) continue;
                Mk = mktime(&a[i].when); // mktime ����������� �� tm  � �������
                if (Mk < TheTime) Group[i] = -1; // ������������ ������
                else if (Mk > TheTime + 604800) Group[i] = 2; // 604800 - ���������� ������ � 1 ������
                else  if (Mk > TheTime + 86400 || a[i].when.tm_mday != CurTime.tm_mday) Group[i] = 1; // 86400 - ���������� ������ � 1 ���. ���� ��� �� ���������.
                else Group[i] = 0;
            } // ����������� ������ ��� ������ ������

            cout << "\t������������:" << endl;
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (Group[i] == -1) {
                    cout << i << ") ";
                    a[i].Print(0);
                    cout << endl;
                }
            }
            cout << endl;
            cout << "\t�� �������:" << endl;
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (Group[i] == 0) {
                    cout << i << ") ";
                    cout << setfill('0') << setw(2) << a[i].when.tm_hour << ":";
                    cout << setfill('0') << setw(2) << a[i].when.tm_min << " ; ";
                    a[i].Print(8);
                    cout << endl;
                }
            }
            cout << endl;
            cout << "\t�� ������:" << endl;
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (Group[i] == 1) {
                    cout << i << ") ";
                    cout << weekday(a[i].when.tm_wday) << " ; ";
                    a[i].Print(5);
                    cout << endl;
                }
            }
            cout << endl;
            cout << "\t��������������:" << endl;
            cout << endl;
            for (int i = 0; i < N ; i++) {
                if (Group[i] == 2) {
                    cout << i << ") ";
                    cout << setfill('0') << setw(2) << a[i].when.tm_mday << ".";
                    cout << setfill('0') << setw(2) << a[i].when.tm_mon + 1;
                    if_year = 0;
                    if (a[i].when.tm_year != CurTime.tm_year) {
                        cout << "." << a[i].when.tm_year + 1900 << " ; ";
                        if_year = 5;
                    }
                    else cout<< " ; ";
                    a[i].Print(8+if_year);
                    cout << endl;
                }
            }
            cout << endl;
        }
        else if (key == "�������") {
            if (cin >> n && n < N && n >= 0) {
                // � ������ �������, ������� ��� ���� ������
                if (mktime(&a[n].repeat) != 0 && !a[n].done) {
                    a[N] = a[n];
                    a[N].when.tm_hour = a[N].when.tm_hour+a[n].repeat.tm_hour-8;
                    a[N].when.tm_mday = a[N].when.tm_mday+a[n].repeat.tm_mday-1;
                    a[N].when.tm_mon = a[N].when.tm_mon+a[n].repeat.tm_mon;
                    a[N].when.tm_year = a[N].when.tm_year+a[n].repeat.tm_year-70;
                    N++;
            
                }
                a[n].done = true;
                Group[n] = -5;
            }
            else
            {
                cout << "������� ����� �� ��������� [0," << N - 1 << "]" << endl;
                cin.clear(); // ������� ���� ������ cin
                continue;
            }

        }
        else if (key == "�������") {
            if (cin >> n && n < N && n >= 0) {
                if (n == N - 1) N--;
                else {
                    a[n] = a[N];
                    a[N] = Case();
                    N--;
                }
            }
            else
            {
                cout << "������� ����� �� ��������� [0," << N - 1 << "]" << endl;
                cin.clear();
                continue;
            }

        }
        else if (key == "��������") {
            if (cin >> n && n < N && n >= 0) {
                cout << "�������:" << endl;
                cout << "\"�\" ����� �������� ��������" << endl;
                cout << "\"�\" ����� �������� �����" << endl;
                cout << "\"�\" ����� �������� ����" << endl;
                cout << "\"�\" ����� �������� ������" << endl;
                cout << "\"�\" ����� �������� ���" << endl;
                cout << ">> ";
                cin >> c;
                int p = 0;
                switch (c)
                {
                case '�':
                    cout << "��������� (0 - ������, 1 - �������, 2 - �������): ";
                    if (cin >> a[n].priority) {
                        if (a[n].priority > 2 || a[n].priority < 0) {
                            cout << "��������� ���. � ��������: [0,2]" << endl;
                            a[n].priority = 0;
                        }
                    }
                    else {
                        cout << "�� ����� ��������� �����������." << endl;
                        cin.clear();
                    }
                    break;
                case '�':
                    cout << "����� ( ��� ��� ). ����� �� ������� �������� ����������, ����������� \"-\": " << endl;
                    
                    if (cin >> p) {
                        if (p <= 23 && p >= 0) a[n].when.tm_hour = p;
                        else cout << "���� ���. � ��������: [0,23]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 59 && p >= 0) a[n].when.tm_min = p;
                        else cout << "������ ���. � ��������: [0,59]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    break;
                case '�':
                    cout << "���� ( ���� ����� ��� ): " << endl;
                    if (cin >> p) {
                        if (p <= 31 && p >= 1) a[n].when.tm_mday = p;
                        else cout << "��� ���. � ��������: [1,31]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 12 && p >= 1) a[n].when.tm_mon = p - 1;
                        else cout << "������ ���. � ��������: [1,12]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p >= 1900) a[n].when.tm_year = p - 1900;
                        else cout << "��� >= 1900" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    break;
                case '�':
                    cout << "������ ����� ������ (����� ���� ������� ���). ��� ���������� ��������� \"- - - -\": " << endl;
                    if (cin >> p) {
                        if (p <= 23 && p >= 0) a[n].repeat.tm_hour = p + 8;
                        else cout << "���� ���. � ��������: [0,23]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 30 && p >= 0) a[n].repeat.tm_mday = p + 1;
                        else cout << "��� ���. � ��������: [0,30]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 11 && p >= 0) a[n].repeat.tm_mon = p;
                        else cout << "������ ���. � ��������: [0,11]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p >= 0) a[n].repeat.tm_year = 70 + p;
                        else cout << "��� >= 0" << endl;
                    }
                    else {
                        cin.clear();
                        cin.unget(); // ��� ���� ����� ��� ����� "-" cin �� ���� ���.
                        cin.ignore(1, ' ');
                    }
                    p = 0;
                    break;
                case '�':
                    cout << "���: ";
                    cin.get();
                    getline(cin, a[n].name);
                    cout << endl;
                    break;
                default:
                    cout << "�� ����� �������� ������!" << endl;
                    break;
                }
                
            }
            else {
            cout << "������� ����� �� ��������� [0," << N - 1 << "]" << endl;
            continue;
            }
        }
        else {
            cout << "������� ������������ �������."<<endl;
            cin.ignore(256, '\n');
        }
    }
    
    ofstream ouf;
    ouf.open("data.txt", std::ofstream::out | std::ofstream::trunc); // ��������� ���� � ��������� ����� ����������� �����������
    for (int i = 0; i < N-1; i++) {
        a[i].Write(ouf);
        ouf << endl;
    }
    a[N-1].Write(ouf);
    ouf.close();
    delete[] Group;
    delete[] a;
	return 0;
}