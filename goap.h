/*
Copyright 2012 Abraham T. Stolk

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef GOAP_H
#define GOAP_H

#ifdef __cplusplus
extern "C"
{
#include <cstdbool>
#include <cstdint>
#else
#include <stdbool.h>
#include <stdint.h>
#endif

#define MAXATOMS 64
#define MAXACTIONS 64

typedef int64_t bfield_t;

//!< Describes the world state by listing values (t/f) for all known atoms.
typedef struct 
{
	bfield_t values;	//!< Values for atoms.
	bfield_t dontcare;	//!< Mask for atoms that do not matter.
} worldstate_t;


//!< Action planner that keeps track of world state atoms and its action repertoire.
typedef struct
{
	const char* atm_names[ MAXATOMS ];	//!< Names associated with all world state atoms.
	int numatoms;				//!< Number of world state atoms.

	const char* act_names[ MAXACTIONS ];	//!< Names of all actions in repertoire.
	worldstate_t act_pre[ MAXACTIONS ];	//!< Preconditions for all actions.
	worldstate_t act_pst[ MAXACTIONS ];	//!< Postconditions for all actions (action effects).
	int act_costs[ MAXACTIONS ];		//!< Cost for all actions.
	int numactions;				//!< The number of actions in out repertoire.

} actionplanner_t;


//!< Initialize an action planner. It will clear all information on actions and state.
extern void goap_actionplanner_clear( actionplanner_t* ap );

//!< Initialize a worldstate to 'dontcare' for all state atoms.
extern void goap_worldstate_clear( worldstate_t* ws );

//!< Set an atom of worldstate to specified value.
extern bool goap_worldstate_set( actionplanner_t* ap, worldstate_t* ws, const char* atomname, bool value );

//!< Add a precondition for named action.
extern bool goap_set_pre( actionplanner_t* ap, const char* actionname, const char* atomname, bool value );

//!< Add a postcondition for named action.
extern bool goap_set_pst( actionplanner_t* ap, const char* actionname, const char* atomname, bool value );

//!< Set the cost for named action.
extern bool goap_set_cost( actionplanner_t* ap, const char* actionname, int cost );

//!< Describe the action planner by listing all actions with pre and post conditions. For debugging purpose.
extern void goap_description( actionplanner_t* ap, char* buf, int sz );

//!< Describe the worldstate by listing atoms that matter, in lowercase for false-valued, and uppercase for true-valued atoms.
extern void goap_worldstate_description( const actionplanner_t* ap, const worldstate_t* ws, char* buf, int sz );

//!< Given the specified 'from' state, list all possible 'to' states along with the action required, and the action cost. For internal use.
extern int  goap_get_possible_state_transitions( actionplanner_t* ap, worldstate_t fr, worldstate_t* to, const char** actionnames, int* actioncosts, int cnt );

#ifdef __cplusplus
}
#endif

#endif
