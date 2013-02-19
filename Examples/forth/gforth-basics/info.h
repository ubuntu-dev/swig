/*
 *	Swig Testing File
 * */

#include <sys/types.h>
#include <unistd.h>

#define thaFIZ 42
#define thaNAME "glforth rocks"
#define Float 1.23
#define Int 23
#define Hex     0x33
#define Oct              077

#if 1
#define GLint int
#define FILE int
#endif

struct position
{
	int pos_x;
	int pos_y;
};

enum DAYS{ MONDAY=1, TUESDAY, WEDNESDAY=10, THURSDAY };

extern void CstyleNames( void );
extern void CppStyleNames( void );
extern void CPPStyleNames( void );
extern void javaStyleUSARocks( void );
extern void javaStyleUSAsameHere( void );
extern void Number123IsSoKewl( void );
extern void Number123is123is123( void );

extern int getTime( void );
extern void setTime( int a );

extern size_t getSize( void );

extern char getChar( void );
extern char* getCharP( void );
extern char** getCharPP( void );

extern float getFloat( void );
extern float* getFloatP( void );
extern float** getFloatPP( void );
extern float* * getFloatPPa( void );
extern float *  *getFloatPPb( void );

extern GLint getGLInt( void );

extern char *fgets(char *s, int size, FILE *stream);
char *interal_fgets(char *s, int size, FILE *stream);
extern char *getStringPointer( char a, char *b, char **c );
extern float getFloatPointer( float a, float *b, float **c );
