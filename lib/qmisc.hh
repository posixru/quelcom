/*! \file
 * header functions for miscellaneous funcions
 */

# include <string>
# include <sys/types.h>	// u_int32_t

extern u_int32_t time2ms (char *str);
extern u_int32_t time2msec (char *str);
extern string uint2string (u_int32_t ui);
extern string char2string (char c);
extern bool strcmpn(char *str1, char *str2, u_int32_t n);
string htmlize (string str);
