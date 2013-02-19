/*
 *	Swig Testing File
 * */

//#include <sys/types.h>
//#include <unistd.h>
//
#define thaFIZ 42
#define thaNAME "glforth rocks"
#define thaMul (1*3)
#define thaFloat 1.23

#define num_0 0
#define num_null NULL
#define num_1 1

enum DAYS{ MONDAY=1, TUESDAY, WEDNESDAY=10, THURSDAY };

extern char getChar( void );
extern char* getCharP( void );

extern float getFloat( void );
extern float* getFloatP( void );

extern int getInt( void );
extern int getIntIndex( int index );

