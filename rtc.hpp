#include <DS3231.h>
#include "extra.hpp"

DS3231 c1;
class RTC {
  public:
    void setNumber(const char* name) {
      numberTeam = name;
    }

    void getTime() {
      String temp, tm = "";
      temp = String(c1.getHour(h12Flag, pmFlag)); temp.length() == 1 ? temp = "0" + temp : temp = temp; tm += temp; tm += ":";
      temp = String(c1.getMinute()); temp.length() == 1 ? temp = "0" + temp : temp = temp; tm += temp; tm += ":";
      temp = String(c1.getSecond()); temp.length() == 1 ? temp = "0" + temp : temp = temp; tm += temp;
      Serial.write(tm.c_str());
      Serial.write("\n");
    }

    void pushNumber() {
      Serial.write("number of team:  ");
      Serial.write(numberTeam);
      Serial.write("      ");
    }

    void setr(String data) {
      int h, m, s;
      byte temp1, temp2;

      temp1 = (byte)data[1] - 48;
      temp2 = (byte)data[2] - 48;
      h = temp1 * 10 + temp2;
      temp1 = (byte)data[3] - 48;
      temp2 = (byte)data[4] - 48;
      m = temp1 * 10 + temp2;
      temp1 = (byte)data[5] - 48;
      temp2 = (byte)data[6] - 48;
      s = temp1 * 10 + temp2;

      c1.setClockMode(false);
      c1.setHour(h);
      c1.setMinute(m);
      c1.setSecond(s);
      Serial.write("Set RTC:  ");
      getTime();
    }

  private:
    const char* numberTeam;
    bool h12Flag, pmFlag;
};


/*  ARCHIVE
    // String tm = String(c1.getHour(h12Flag, pmFlag)) + ":" + String(c1.getMinute()) + ":" + String(c1.getSecond());

    // Serial.write("UTC:  ");
    // // Serial.print(c1.getHour(h12Flag, pmFlag));
    // print2digits(c1.getHour(h12Flag, pmFlag));
    // Serial.write(':');
    // // Serial.print(c1.getMinute());
    // print2digits(c1.getMinute());
    // Serial.write(':');
    // // Serial.print(c1.getSecond());
    // print2digits(c1.getSecond());


    // void print2digits(byte number) {
    //   int i = int(number);
    //   Serial.write((i - (i % 10)) / 10 + 48);
    //   Serial.write(i % 10 + 48);
    // }
*/