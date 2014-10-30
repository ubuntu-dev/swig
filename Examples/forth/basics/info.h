/*
 *	Swig Testing File
 * */

//#include <sys/types.h>
//#include <unistd.h>
//
#define thaFIZ 42
#define thaNoLive 0xFFFFFFFFFFFFFFFFu
#define thaNAME "glforth rocks"
#define thaMul (1*3)
#define thaFloat 1.23
#define thaBigMinus -0xFFFFFFFFFFFFFFFll
#define thaBig 0xFFFFFFFFFFFFFFFFull

#define num_0 0
#define num_null NULL
#define num_1 1

/* case 1: standalone struct and typedef */
struct vec2 {
	int x;
	int y;
};

typedef struct vec2 VEC2;

/* case 2: direct typedef struct */
typedef struct {
	int x;
	int y;
	int z;
} VEC3;

/* case 3: typedef and named struct */
typedef struct vec4 {
	int w;
	int x;
	int y;
	int z;
	int(*callInt)(int x, int y, int z);
	void(*callVoid)(int x, int y, int z);
	void*(*callVoidP)(int x, int y, int z);
} VEC4;

enum DAYS{ MONDAY=1, TUESDAY, WEDNESDAY=10, THURSDAY };

extern char getChar( void );
extern char* getCharP( void );

extern float getFloat( void );
extern float* getFloatP( void );

extern int getInt( void );
extern int getIntIndex( int index );

extern int getVec4X( VEC4 v4 );

#define SWIG_FORTH_OPTIONS "forthifyfunctions"
extern int getVec4Y( VEC4 v4 );


int(*bareCallInt)(int x, int y, int z);
void(*bareCallVoid)(int x, int y, int z);

typedef void(*typedefCallback)(int x, int y, int z);
