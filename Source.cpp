#define _CRT_SECURE_NO_WARNINGS
#define __STDC_WANT_LIB_EXT1__ 1 
// Для функции localtime_s

#include "Header.h"

int main() {
    system("chcp 1251");
    system("cls");
    const time_t TheTime = time(NULL);
    tm CurTime;
    localtime_s(&CurTime, &TheTime); // Запоминаем время запуска программы

    int NUM=100; // Количество задач (элементов массива)
    int N = 0; // Количество непустых задач в массиве

    Case* a = new Case[NUM]; // Создание массива

    fstream file;  
    file.open("data.txt");
    if (!file) { cout << "Error. File \"data.dat\" is removed." << endl; return 1;}

    int TTime; // Чтение файла
    while (!file.eof()&&N<NUM) { // Цикл до конца файла
        file >> TTime;
        a[N].SetTime(TTime);
        file >> a[N].done;
        file >> TTime;
        a[N].SetRepeat(TTime);
        file >> a[N].priority;
        file.seekg(1, ios::cur); // Пропускает пробел в начале строки
        getline(file,a[N].name);
        N++;
    }
    file.close();
    // Основная часть
    int if_year=0;
    int n=0;
    char c = ' ';
    int Mk;
    string key;
    int* Group = new int[NUM]; // Группа: -1 - просроченные, 0 - сегодня, 1 - неделя, 2 - долгосрочные 
    for (int i = 0; i < NUM; i++) Group[i] = -5;
    cout << "Здравствуйте! Команды:" << endl << "выход - завершить использование " << endl << "создать - начать новую задачу" << endl << "вывод - отобразить все записи" << endl;
    cout << "сделано n - пометить n-ую задачу как выполненную" << endl << "удалить n - удалить n-ую задачу" << endl << "изменить n - редактировать n-ую задачу" << endl;
    while (1) {
        cout << ">> ";
        cin >> key;
        if (key == "выход") break;
        else if (key == "создать") {
            if (N < NUM - 3) {
                a[N].Ask();
                N++;
            }
            else cout << "Превышен лимит на количество задач." << endl;
            cout << endl;
        }
        else if (key == "вывод") {
            cout << "\tЗавершённые задачи:" << endl;
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
                Mk = mktime(&a[i].when); // mktime преобразует из tm  в секунды
                if (Mk < TheTime) Group[i] = -1; // Просроченные задачи
                else if (Mk > TheTime + 604800) Group[i] = 2; // 604800 - количество секунд в 1 неделе
                else  if (Mk > TheTime + 86400 || a[i].when.tm_mday != CurTime.tm_mday) Group[i] = 1; // 86400 - количество секунд в 1 дне. Если дни не совпадают.
                else Group[i] = 0;
            } // Определение группы для каждой задачи

            cout << "\tПросроченные:" << endl;
            cout << endl;
            for (int i = 0; i < N; i++) {
                if (Group[i] == -1) {
                    cout << i << ") ";
                    a[i].Print(0);
                    cout << endl;
                }
            }
            cout << endl;
            cout << "\tНа сегодня:" << endl;
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
            cout << "\tНа неделю:" << endl;
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
            cout << "\tДолговременные:" << endl;
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
        else if (key == "сделано") {
            if (cin >> n && n < N && n >= 0) {
                // В случае повтора, создать ещё одну задачу
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
                cout << "Вводите число из диапазона [0," << N - 1 << "]" << endl;
                cin.clear(); // Очищает флаг ошибки cin
                continue;
            }

        }
        else if (key == "удалить") {
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
                cout << "Вводите число из диапазона [0," << N - 1 << "]" << endl;
                cin.clear();
                continue;
            }

        }
        else if (key == "изменить") {
            if (cin >> n && n < N && n >= 0) {
                cout << "Введите:" << endl;
                cout << "\"в\" чтобы изменить важность" << endl;
                cout << "\"т\" чтобы изменить время" << endl;
                cout << "\"д\" чтобы изменить дату" << endl;
                cout << "\"п\" чтобы изменить повтор" << endl;
                cout << "\"и\" чтобы изменить имя" << endl;
                cout << ">> ";
                cin >> c;
                int p = 0;
                switch (c)
                {
                case 'в':
                    cout << "Приоритет (0 - низкий, 1 - средний, 2 - высокий): ";
                    if (cin >> a[n].priority) {
                        if (a[n].priority > 2 || a[n].priority < 0) {
                            cout << "Приоритет изм. в пределах: [0,2]" << endl;
                            a[n].priority = 0;
                        }
                    }
                    else {
                        cout << "Вы ввели приоритет некорректно." << endl;
                        cin.clear();
                    }
                    break;
                case 'т':
                    cout << "Время ( час мин ). Чтобы не вводить излишнюю информацию, используйте \"-\": " << endl;
                    
                    if (cin >> p) {
                        if (p <= 23 && p >= 0) a[n].when.tm_hour = p;
                        else cout << "Часы изм. в пределах: [0,23]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 59 && p >= 0) a[n].when.tm_min = p;
                        else cout << "Минуты изм. в пределах: [0,59]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    break;
                case 'д':
                    cout << "Дата ( день месяц год ): " << endl;
                    if (cin >> p) {
                        if (p <= 31 && p >= 1) a[n].when.tm_mday = p;
                        else cout << "Дни изм. в пределах: [1,31]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 12 && p >= 1) a[n].when.tm_mon = p - 1;
                        else cout << "Месяцы изм. в пределах: [1,12]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p >= 1900) a[n].when.tm_year = p - 1900;
                        else cout << "Год >= 1900" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    break;
                case 'п':
                    cout << "Повтор через каждые (часов дней месяцев лет). При отсутствии поставьте \"- - - -\": " << endl;
                    if (cin >> p) {
                        if (p <= 23 && p >= 0) a[n].repeat.tm_hour = p + 8;
                        else cout << "Часы изм. в пределах: [0,23]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 30 && p >= 0) a[n].repeat.tm_mday = p + 1;
                        else cout << "Дни изм. в пределах: [0,30]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p <= 11 && p >= 0) a[n].repeat.tm_mon = p;
                        else cout << "Месяцы изм. в пределах: [0,11]" << endl;
                    }
                    else cin.clear();
                    p = 0;
                    if (cin >> p) {
                        if (p >= 0) a[n].repeat.tm_year = 70 + p;
                        else cout << "Год >= 0" << endl;
                    }
                    else {
                        cin.clear();
                        cin.unget(); // Для того чтобы при вводе "-" cin не ждал ещё.
                        cin.ignore(1, ' ');
                    }
                    p = 0;
                    break;
                case 'и':
                    cout << "Имя: ";
                    cin.get();
                    getline(cin, a[n].name);
                    cout << endl;
                    break;
                default:
                    cout << "Вы ввели неверный символ!" << endl;
                    break;
                }
                
            }
            else {
            cout << "Вводите число из диапазона [0," << N - 1 << "]" << endl;
            continue;
            }
        }
        else {
            cout << "Введена неправильная команда."<<endl;
            cin.ignore(256, '\n');
        }
    }
    
    ofstream ouf;
    ouf.open("data.txt", std::ofstream::out | std::ofstream::trunc); // открывает файл с удалением всего предыдущего содержимого
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