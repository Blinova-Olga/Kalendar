#include <iostream>
#include <ctime>
#include <string>
using namespace std;

string weekday(int day)
{
    char wday_name[][3] = {
      "Âñ", "Ïí", "Âò", "Ñğ", "×ò", "Ïò", "Ñá"
    };
    return wday_name[day];
}