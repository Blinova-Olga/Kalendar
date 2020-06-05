#include "Header.h"

void Case::Write(ofstream& f) { // Запись в конец файла
    f << mktime(&when) << ' ';
    f << done << ' ';
    f << mktime(&repeat) << ' ';
    f << priority << ' ';
    f << name;
}

void Case::Ask() {
    cout << "Создание новой задачи!" << endl;
    cout << "Имя: ";
    cin.get();
    getline(cin, name);
    cout<<"Приоритет (0 - низкий, 1 - средний, 2 - высокий): ";
    if (cin >> priority) {
        if (priority > 2 || priority < 0) {
            cout << "Приоритет изм. в пределах: [0,2]" << endl;
            priority = 0;
        }
    }
    else {
        cout << "Вы ввели приоритет некорректно." << endl;
        cin.clear();
    }

    cout<<"Время ( час мин ). Чтобы не вводить излишнюю информацию, используйте \"-\": "<<endl;
    int p=0;
    if (cin >> p) { 
        if (p <= 23 && p >= 0) when.tm_hour = p;
        else cout << "Часы изм. в пределах: [0,23]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) { 
        if(p <= 59 && p >= 0) when.tm_min = p;
        else cout << "Минуты изм. в пределах: [0,59]" << endl;
    }
    else cin.clear();
    p = 0;

    cout << "Дата ( день месяц год ): " << endl;
    if (cin >> p) {
        if (p <= 31 && p >= 1) when.tm_mday = p;
        else cout << "Дни изм. в пределах: [1,31]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 12 && p >= 1) when.tm_mon = p - 1;
        else cout << "Месяцы изм. в пределах: [1,12]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p >= 1900) when.tm_year = p - 1900;
        else cout << "Год >= 1900" << endl;
    }
    else cin.clear();
    p = 0;


    cout << "Повтор через каждые (часов дней месяцев лет). При отсутствии поставьте \"- - - -\": " << endl;
    if (cin >> p) {
        if (p <= 23 && p >= 0) repeat.tm_hour = p + 8;
        else cout << "Часы изм. в пределах: [0,23]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 30 && p >= 0) repeat.tm_mday = p + 1;
        else cout << "Дни изм. в пределах: [0,30]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p <= 11 && p >= 0) repeat.tm_mon = p;
        else cout << "Месяцы изм. в пределах: [0,11]" << endl;
    }
    else cin.clear();
    p = 0;
    if (cin >> p) {
        if (p >= 0) repeat.tm_year = 70 + p;
        else cout << "Год >= 0" << endl;
    }
    else {
        cin.clear();
        cin.unget(); // Для того чтобы при вводе "-" cin не ждал ещё.
        cin.ignore(1, ' ');
    }
    p = 0;
    cout << "Готово!" << endl;
} 

void Case::Print(int k) {
    if (name.size() <= MAX_NAME_SIZE-k) {
        cout.setf(ios::left);
        cout << setfill('-') << setw(MAX_NAME_SIZE-k) << name << " ; ";
        cout.unsetf(ios::left);
    }
    else cout << name << " ; "; //Вывод имени
    switch (priority)
    {
    case 0:
        cout << "Низкий приоритет ";
        break;
    case 1:
        cout << "Средний приоритет";
        break;
    case 2:
        cout << "Высокий приоритет";
        break;
    default:
        break;
    } // Приоритет
    cout << " ; Повтор: ";
    if (mktime(&repeat) == 0) cout << "Нет";
    else {
        cout << "через "; 
        if (repeat.tm_year != 70) cout << repeat.tm_year - 70 << " г. ";
        if (repeat.tm_mon != 0) cout << repeat.tm_mon << " мес. ";
        if (repeat.tm_mday != 1) cout << repeat.tm_mday - 1 << "д. ";
        if (repeat.tm_hour != 8) cout << repeat.tm_hour-8<< " ч.";
    }
}