
/* port.c */
#include <iostream>
using namespace std;
typedef unsigned char byte;

int main() {
  int fn, arg, res;
  byte buf[100];
  while (read_cmd(buf) > 0)
  {
    fn = buf[0];
    arg = buf[1];
    
    if (fn == 1) {
      res = foo(arg);
    } else if (fn == 2) {
      res = bar(arg);
    }

    buf[0] ='a';
    buf[1]='c';
    buf[2]='<';
    write_cmd(buf,3);
  }
}
