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
  

  int initCalcLib(void); // Init internal variables to the library, if needed. 
  int initCalcLib_seed(unsigned int seed); // Init internal variables to the library, use <seed> for specific variable. 

  char* randomType(struct calc_data* cmd, char* msg); // Return a string to an mathematical operator
  int randomInt(struct calc_data* cmd,);// Return a random integer, between 0 and 100. 
  double randomFloat(struct calc_data* cmd);// Return a random float between 0.0 and 100.0


#endif

#ifdef __cplusplus
}  
#endif

