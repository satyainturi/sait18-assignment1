#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "calcLib.h"

#define RANDOM_MAX_VALUE 100000

std::string arith[8] = {"add", "div", "mul", "fsub", "fadd", "fdiv", "fmul", "fsub"};


int random_int(void) 
{
  return rand() % RANDOM_MAX_VALUE;
};

double random_double(void) 
{
  return (double)(random_int() / 3.0);
};

void create_calc_data(struct calc_data* calc) 
{
  int randId = rand() % 8;
  
  calc->str = arith[randId];
  calc->val1 = random_int();
  calc->val2 = random_int();
  calc->fval1 = random_double();
  calc->fval2 = random_double();
}

void get_calc_data_string(struct calc_data* calc, char* text) 
{
  if (calc->str.size() == 3) {
    sprintf(text, "%s %d %d\n", calc->str.c_str(), calc->val1, calc->val2);
  }
  else {
    sprintf(text, "%s %.8g %.8g\n", calc->str.c_str(), calc->fval1, calc->fval2);
  }
}

double calculate(struct calc_data* calc) 
{
  if (calc->str == "add") {
    return (double)(calc->val1 + calc->val2);
  }
  if (calc->str == "sub") {
    return (double)(calc->val1 - calc->val2);
  }
  if (calc->str == "mul") {
    return (double)(calc->val1 * calc->val2);
  }
  if (calc->str == "div") {
    return (double)calc->val1 / (double)calc->val2;
  }
  if (calc->str == "fadd") {
    return calc->fval1 + calc->fval2;
  }
  if (calc->str == "fsub") {
    return calc->fval1 - calc->fval2;
  }
  if (calc->str == "fmul") {
    return calc->fval1 * calc->fval2;
  }
  if (calc->str == "fdiv") {
    return calc->fval1 / calc->fval2;
  }
  return 0;
}


