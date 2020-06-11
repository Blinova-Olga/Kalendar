#define _CRT_SECURE_NO_WARNINGS
#define __STDC_WANT_LIB_EXT1__ 1 
// Для функции localtime_s

#include "Header.h"

int main(int argc, char* argv[]) {
    system("chcp 1251");
    system("cls");
    const time_t TheTime = time(NULL);
    tm CurTime;
    localtime_s(&CurTime, &TheTime); // Запоминаем время запуска программы

    int N = 0; // Количество непустых задач в массиве

    Case* a = new Case[NUM]; // Создание массива

    string filename = "data.txt";
    if (argc > 1) { filename = argv[1]; }
    if (!filename.find(".txt")) {
        cout << "Ошибка! Вы ввели имя файла некорректно. Пример корректного ввода: Kalendar \"data.txt\" " << endl;
        return 1;
    }
    fstream file;  
    file.open(filename);
    if (!file) { cout << "Ошибка! невозможно открыть файл \""<<filename<<"\"." << endl; return 1;}
    int TTime; // Чтение файла (when during repeat done doneTime priority name)
    while (!file.eof()&&N<NUM) { // Цикл до конца файла
        file >> TTime;
        a[N].SetTime(TTime);

        file >> a[N].during;

        file >> TTime;
        a[N].SetRepeat(TTime);

        file >> a[N].done;

        file >> TTime;
        a[N].SetDone(TTime);

        file >> a[N].priority;

        file.seekg(1, ios::cur); // Пропускает пробел в начале строки
        getline(file,a[N].name);
        N++;
    }
    file.close();
    // Основная часть
    tm TmStruct; // Вспомогательная структура tm
    int if_year=0;
    int n=0;
    char sort = 't';
    char c = ' ';
    int Mk;
    string key="";
    // [0] - сегодня, [1] - на неделю, [2] - долговременные, [3] - просроченные
    short Priority[NUM][4];
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < NUM; i++) {
            Priority[i][j] = -1;
        }
    }
    // [0] - сегодня, [1] - на неделю, [2] - долговременные, [3] - просроченные
    int NumTime[4];
    for (int i = 0; i < 4; i++) {
        NumTime[i] = 0;
    }
    // ByTime[][][0] - время в сек, ByTime[][][1] - индекс задачи
    int ByTime[4][NUM][2];
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < NUM; i++) {
            ByTime[j][i][0] = -1;
            ByTime[j][i][1] = -1;
        }
    }
    int n1, n2;
    if (argc > 2) {
        string arg = argv[2];
        if (arg=="--out") {
            key = "--out";
            for (int i = 3; i < argc; i++) {
                key = key +" "+ argv[i];
            }
        }
    }

    while (1) {
        if (key.find("-sort") != string::npos) {
            if (key.find("/t") != string::npos) {
                sort = 't';
                cout << "Установлена сортировка по времени." << endl;
            }
            else if (key.find("/p") != string::npos) {
                sort = 'p';
                cout << "Установлена сортировка по приоритету." << endl;
            } else cout << "Нераспознана сортировка." << endl;
        }
        if (key.find("--help")!=string::npos) {
            cout << "Команды:" << endl << "--exit - выйти " << endl << "--new - начать новую запись" << endl << "--out - вывести все записи" << endl;
            cout << "--do n - пометить n-ую задачу как выполненную" << endl << "--delete n - удалить n-ую задачу" << endl << "--change n - редактировать n-ую задачу" << endl;
        }else if (key.find("--exit") != string::npos) break; //флаг -e = exit
        else if (key.find("--new") != string::npos) { // флаг создания новой задачи
            
            if (N < NUM - 3) {
                a[N].Ask();
                N++;
            }
            else cout << "Превышен лимит на количество задач." << endl;
            cout << endl;
        }
        else if (key.find("--out") != string::npos) { // -o = out вывод списка задач
            if (key.find("help") != string::npos) {
                cout << "Введите:" << endl;
                cout << "\"-t\" на сегодня" << endl;
                cout << "\"-w\" на неделю" << endl;
                cout << "\"-l\" долговременные" << endl;
                cout << "\"-d\" завершённые" << endl;
                cout << "\"-b\" просроченные" << endl;
            }
           
            if (key.length() == 5 || key.find("-d") != string::npos) {
                cout << "\tЗавершённые задачи:" << endl;
                cout << endl;
                for (int i = 0; i < N; i++) {
                    if (a[i].done) {
                        cout << i << ") ";
                        a[i].PrintName();
                        cout << "Дата выполнения: ";
                        cout << setfill('0') << setw(2) << a[i].doneTime.tm_mday << ".";
                        cout << setfill('0') << setw(2) << a[i].doneTime.tm_mon + (int)1;
                        if (a[i].doneTime.tm_year != CurTime.tm_year) { cout << "." << a[i].doneTime.tm_year + (int)1900; }
                        cout << " ";
                        cout << setfill('0') << setw(2) << a[i].doneTime.tm_hour << ":";
                        cout << setfill('0') << setw(2) << a[i].doneTime.tm_min << " ";
                        cout << endl;
                    }
                }
                cout << endl;
            }
            if (sort == 't') {
                for (int i = 0; i < N; i++) {
                    if (a[i].done) continue;
                    Mk = mktime(&a[i].when); // mktime преобразует из tm  в секунды
                    if (Mk + a[i].during < TheTime) { 
                        ByTime[3][NumTime[3]][0] = mktime(&a[i].when); 
                        ByTime[3][NumTime[3]][1] = i;
                        NumTime[3]++;
                    } // Просроченные задачи
                    else if (Mk > TheTime + 604800) {
                        ByTime[2][NumTime[2]][0] = mktime(&a[i].when); // Долговременные
                        ByTime[2][NumTime[2]][1] = i; // 604800 - количество секунд в 1 неделе
                        NumTime[2]++;
                    }
                    else  if (Mk > TheTime + 86400 || a[i].when.tm_mday != CurTime.tm_mday) {
                        ByTime[1][NumTime[1]][0] = mktime(&a[i].when); // Неделя
                        ByTime[1][NumTime[1]][1] = i; // 86400 - количество секунд в 1 дне. Если дни не совпадают.
                        NumTime[1]++;
                    }
                    else {
                        ByTime[0][NumTime[0]][0] = mktime(&a[i].when);
                        ByTime[0][NumTime[0]][1] = i; // сегодня
                        NumTime[0]++;
                    }
                }
                for (int i = 0; i < 4; i++) {
                    if(NumTime[i]>1)
                    Quick_sort(ByTime[i], 0, NumTime[i]-1);
                }
            }
            else {
                for (int i = 0; i < N; i++) {
                    if (a[i].done) continue;
                        Mk = mktime(&a[i].when); // mktime преобразует из tm  в секунды
                        if (Mk + a[i].during < TheTime) Priority[i][3] = a[i].priority; // Просроченные задачи
                        else if (Mk > TheTime + (int)604800) Priority[i][2] = a[i].priority; // 604800 - количество секунд в 1 неделе
                        else  if (Mk > TheTime + (int)86400 || a[i].when.tm_mday != CurTime.tm_mday) Priority[i][1] = a[i].priority; // 86400 - количество секунд в 1 дне. Если дни не совпадают.
                        else Priority[i][0] = a[i].priority; // сегодня
                   
                }
            }

            if (key.length() == 5 || key.find("-b") != string::npos) {
                cout << "\tПросроченные:" << endl;
                cout << endl;
                if (sort == 'p') {
                    for (int j = 2; j >= 0; j--) {
                        for (int i = 0; i < N;i++) {
                            if (Priority[i][3]==j) {
                                cout << i << ") ";
                                a[i].PrintName();
                                cout << "Истекло: ";
                                const time_t Time = mktime(&a[i].when) + a[i].during;
                                localtime_s(&TmStruct, &Time);
                                cout << setfill('0') << setw(2) << TmStruct.tm_mday << ".";
                                cout << setfill('0') << setw(2) << TmStruct.tm_mon + (int)1;
                                if (a[i].doneTime.tm_year != CurTime.tm_year) { cout << "." << TmStruct.tm_year + (int)1900; }
                                cout << " ";
                                cout << setfill('0') << setw(2) << TmStruct.tm_hour << ":";
                                cout << setfill('0') << setw(2) << TmStruct.tm_min << " ";
                                cout << endl;
                            }
                        }
                    }
                    cout << endl;
                }
                else {
                    for (int u = 0; u < NumTime[3]; u++) {
                            cout << ByTime[3][u][1] << ") ";
                            a[ByTime[3][u][1]].PrintName();
                            cout << "Истекло: ";
                            const time_t Time = mktime(&a[ByTime[3][u][1]].when) + a[ByTime[3][u][1]].during;
                            localtime_s(&TmStruct, &Time);
                            cout << setfill('0') << setw(2) << TmStruct.tm_mday << ".";
                            cout << setfill('0') << setw(2) << TmStruct.tm_mon + (int)1;
                            if (a[ByTime[3][u][1]].doneTime.tm_year != CurTime.tm_year) { cout << "." << TmStruct.tm_year + (int)1900; }
                            cout << " ";
                            cout << setfill('0') << setw(2) << TmStruct.tm_hour << ":";
                            cout << setfill('0') << setw(2) << TmStruct.tm_min << " ";
                            cout << endl;
                        
                    }
                    cout << endl;
                }
            }
            if (key.length() == 5 || key.find("-t") != string::npos) {
                cout << "\tНа сегодня:" << endl;
                cout << endl;
                if (sort == 'p') {
                    for (int j = 2; j >= 0; j--) {
                        for (int i = 0; i < N; i++) {
                            if (Priority[i][0] == j) {
                                cout << i << ") ";
                                a[i].PrintName();
                                a[i].PrintTime('h');
                                a[i].PrintDuring();
                                a[i].Print();
                                cout << endl;
                            }
                        }
                    }
                    cout << endl;
                }
                else {
                    for (int u = 0; u < NumTime[0]; u++) {
                        cout << ByTime[0][u][1] << ") ";
                        a[ByTime[0][u][1]].PrintName();
                        a[ByTime[0][u][1]].PrintTime('h');
                        a[ByTime[0][u][1]].PrintDuring();
                        a[ByTime[0][u][1]].Print();
                        cout << endl;
                    }
                    cout << endl;
                }

            }
            if (key.length() == 5 || key.find("-w") != string::npos) {
                cout << "\tНа неделю:" << endl;
                cout << endl;
                if (sort == 'p') {
                    for (int j = 2; j >= 0; j--) {
                        for (int i = 0; i < N; i++) {
                            if (Priority[i][1] == j) {
                                cout << i << ") ";
                                a[i].PrintName();
                                a[i].PrintTime('w');
                                a[i].PrintDuring();
                                a[i].Print();
                                cout << endl;
                            }
                        }
                    }
                    cout << endl;
                }
                else {
                    for (int u = 0; u < NumTime[1]; u++) {
                        cout << ByTime[1][u][1] << ") ";
                        a[ByTime[1][u][1]].PrintName();
                        a[ByTime[1][u][1]].PrintTime('w');
                        a[ByTime[1][u][1]].PrintDuring();
                        a[ByTime[1][u][1]].Print();
                        cout << endl;
                    }
                    cout << endl;
                }
            }
            if (key.length() == 5 || key.find("-l") != string::npos) {
                cout << "\tДолговременные:" << endl;
                cout << endl;
                if (sort == 'p') {
                    for (int j = 2; j >= 0; j--) {
                        for (int i = 0; i < N; i++) {
                            if (Priority[i][2] == j) {
                                cout << i << ") ";
                                a[i].PrintName();
                                a[i].PrintTime('y');
                                a[i].Print();
                                cout << endl;
                            }
                        }
                    }
                    cout << endl;
                }
                else {
                    for (int u = 0; u < NumTime[2]; u++) {
                        cout << ByTime[2][u][1] << ") ";
                        a[ByTime[2][u][1]].PrintName();
                        a[ByTime[2][u][1]].PrintTime('y');
                        a[ByTime[2][u][1]].Print();
                        cout << endl;

                    }
                    cout << endl;
                }
            }
        }
        else if (key.find("--do")!= string::npos) { //-d = done отметить сделанную задачу
        n1 = key.find(" ");
        if (key.find(" ", n1 + 1) != string::npos) {
            n2 = key.find(" ", n1 + 1);
        }
        else n2 = key.length();
            n = 0;
            for (int i = n1 + 1; i < n2; i++) {
                n = n * 10 + (key[i] + 0) - 48;
            }
            if (n < N && n >= 0&& key.length()>5) {
                // В случае повтора, создать ещё одну задачу
                if (mktime(&a[n].repeat) != 0 && !a[n].done) {
                    a[N] = a[n];
                    a[N].when.tm_hour = a[N].when.tm_hour+a[n].repeat.tm_hour- (int)8;
                    a[N].when.tm_mday = a[N].when.tm_mday+a[n].repeat.tm_mday- (int)1;
                    a[N].when.tm_mon = a[N].when.tm_mon+a[n].repeat.tm_mon;
                    a[N].when.tm_year = a[N].when.tm_year+a[n].repeat.tm_year- (int)70;
                    N++;
                }
                a[n].done = true;
                a[n].SetDone(time(NULL));
            }
            else
            {
                cout << "Вводите число из диапазона [0," << N - 1 << "]" << endl;
                cin.clear();
                continue;
            }

        }
        else if (key.find("--delete") != string::npos) { //-d = delete удалить задачу из списка
            n1 = key.find(" ");
            if (key.find(" ", n1 + 1) != string::npos) {
                n2 = key.find(" ", n1 + 1);
            }
            else n2 = key.length();
            n = 0;
            for (int i = n1 + 1; i < n2; i++) {
                n = n * 10 + (key[i] + 0) - 48;
            }
            if (n < N && n >= 0) {
                if (n == N - 1) N--;
                else {
                    a[n] = a[N-1];
                    a[N-1] = Case();
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
        else if (key.find("--change") != string::npos) {
        if (key.find("help") != string::npos) {
            cout << "Введите:" << endl;
            cout << "\"-p\" чтобы изменить приоритет" << endl;
            cout << "\"-t\" чтобы изменить время" << endl;
            cout << "\"-d\" чтобы изменить дату" << endl;
            cout << "\"-r\" чтобы изменить повтор" << endl;
            cout << "\"-n\" чтобы изменить имя" << endl;
        }
        n1 = key.find(" ");
        if (key.find(" ", n1 + 1) != string::npos) {
            n2 = key.find(" ", n1 + 1);
        }
        else n2 = key.length();
        n = 0;
        for (int i = n1 + 1; i < n2; i++) {
            n = n * 10 + (key[i] + 0) - 48;
        }
            if (n < N && n >= 0) {
                
                c = key[key.find('-', 8) + 1];
                int p = 0;
                switch (c)
                {
                case 'p':
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
                case 't':
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
                case 'd':
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
                case 'r':
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
                case 'n':
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
            cin.clear();
            continue;
            }
        }
        else if(key!="" && key.find("-sort") == string::npos){
            cout << "Введена неправильная команда."<<endl;
            cin.clear();
        }
        getline(cin, key);
    }
    
    ofstream ouf;
    ouf.open("data.txt", std::ofstream::out | std::ofstream::trunc); // открывает файл с удалением всего предыдущего содержимого
    for (int i = 0; i < N-1; i++) {
        a[i].Write(ouf);
        ouf << endl;
    }
    a[N-1].Write(ouf);
    ouf.close();
    delete[] a;
	return 0;
}