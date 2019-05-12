//
// Created by SomeDude on 12.05.2019.
//

#include "SerialClass.h"

int SerialClass::available(){
  return 0;
}
char SerialClass::read() {
  return '\n';
}

SerialClass Serial;

