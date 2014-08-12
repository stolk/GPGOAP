/*
Copyright 2012 Abraham T. Stolk

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef GOAP_H
#define GOAP_H

#if !defined(_MSC_VER)
#include <stdbool.h>
#endif

//#define MAXATOMS 64
#define DEFAULT_COST 1

typedef long long int bfield_t;

namespace GOAP {

//!< Describes the world state by listing values (t/f) for all known atoms.
struct worldstate_t
{
	bfield_t values;	//!< Values for atoms.
	bfield_t dontcare;	//!< Mask for atoms that do not matter.
};

//!< Action planner that keeps track of world state atoms and its action repertoire.
struct actionplanner_t
{
	actionplanner_t(size_t max_atoms = 64, size_t max_actions = 64)
		: _max_actions( max_actions )
		, _max_atoms( _max_atoms )
	{
		atm_names = new char*[max_atoms];
		act_names = new char*[max_actions];
		act_pre = new worldstate_t[max_actions];
		act_pst = new worldstate_t[max_actions];
		act_costs = new int[max_actions];
	}

	~actionplanner_t()
	{
		delete [] atm_names;
		delete [] act_names;
		delete [] act_pre;
		delete [] act_pst;
		delete [] act_costs;
	}

	size_t get_max_atoms() const
	{
		return _max_atoms;
	}

	size_t get_max_actions() const
	{
		return _max_actions;
	}

	char** atm_names;			//!< Names associated with all world state atoms.
	int numatoms;				//!< Number of world state atoms.

	char** act_names;	//!< Names of all actions in repertoire.
	worldstate_t* act_pre;	//!< Preconditions for all actions.
	worldstate_t* act_pst;	//!< Postconditions for all actions (action effects).
	int* act_costs;		//!< Cost for all actions.
	int numactions;				//!< The number of actions in out repertoire.
	size_t _max_atoms;
	size_t _max_actions;
};

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
extern int  goap_get_possible_state_transitions( actionplanner_t* ap, worldstate_t fr, worldstate_t* to, char** actionnames, int* actioncosts, int cnt );

}

#endif
