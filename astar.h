#ifndef ASTAR_H
#define ASTAR_H

#ifndef LOGI
#include <stdio.h>
#define LOGI(...) { printf( __VA_ARGS__ ); printf("\n"); }
#define LOGW(...) { printf( "WRN " __VA_ARGS__ ); printf("\n"); }
#define LOGE(...) { printf( "ERR " __VA_ARGS__ ); printf("\n"); }
#endif

#include "goap.h"

struct astarnode;

//!< A node in our network of world states.
struct astarnode
{
	worldstate_t ws;		//!< The state of the world at this node.
	int g;				//!< The cost so far.
	int h;				//!< The heuristic for remaining cost (don't overestimate!)
	int f;				//!< g+h combined.
	const char* actionname;		//!< How did we get to this node?
	worldstate_t parentws;		//!< Where did we come from?
};


typedef struct astarnode astarnode_t;


//! Make a plan of actions that will reach desired world state. Returns total cost of the plan.
extern int astar_plan
(
        actionplanner_t* ap, 		//!< the goap action planner that holds atoms and action repertoire
        worldstate_t start, 		//!< the current world state
        worldstate_t goal, 		//!< the desired world state
        const char** plan,              //!< for returning all actions that make up plan
        worldstate_t* worldstates,      //!< for returning intermediate world states
        int* plansize                   //!< in: size of plan buffer, out: size of plan (in nr of steps)
);

#endif
