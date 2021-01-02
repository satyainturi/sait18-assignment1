#ifdef __cplusplus
extern "C"{
#endif

#ifndef __CALC_LIB
#define __CALC_LIB

/#include <string>

struct calc_data 
{
	std::string str;
	int val1, val2;
	double fval1, fval2;
};
  

  void create_calc_data(struct calc_data* cmd);
  void get_calc_data_string(struct calc_data* cmd, char* msg);
  char* randomType(struct calc_data* cmd, char* msg); // Return a string to an mathematical operator
  double randomFloat(struct calc_data* cmd);// Return a random float between 0.0 and 100.0


#endif

#ifdef __cplusplus
}  
#endif

