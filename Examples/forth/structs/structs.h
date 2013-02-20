/*
 *	Swig Testing File
 * */

#define ONE 1

struct position
{
	int position_x;
	int position_y;
	int position_z;
	char name[200];
};

typedef struct
{
	int pos_x;
	int pos_y;
} pos;

int manhattenDistancePosition( struct position );
int manhattenDistancePos( pos );

struct position averagePosition( struct position, struct position );
pos averagePos( pos, pos );

