#include <iostream>
#include <ctime>
#include <string>
using namespace std;

string weekday(int day)
{
    char wday_name[][3] = {
      "��", "��", "��", "��", "��", "��", "��"
    };
    return wday_name[day];
}