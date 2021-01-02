#ifndef _CALCLIB_H_
#define _CALCLIB_H_

#include <string>

struct calc_data 
{
	std::string str;
	int val1, val2;
	double fval1, fval2;
};

void create_calc_data(struct calc_data* cmd);
void get_calc_data_string(struct calc_data* cmd, char* msg);
double calculate(struct calc_data* cmd);
char* compare_value(struct calc_data* cmd, char* msg);

#endif//_CALCLIB_H_