/*
 *	Swig Testing File
 * */

#define ONE 1

struct position
{
	int position_x;
	int position_y;
};

struct event
{
        int type;
	char name[20];
	struct position bounds[2];
        struct position pos;
        struct position *last_pos;
        int (*eventHandler)(int x, int y);
        int *(*eventHandlerP)(int x, int y);
};

extern char getChar( void );
extern void registerCallback( int type, int (*callMe)(int x, int y) );
