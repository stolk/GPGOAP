# General Purpose GOAP

## Introduction
GOAP, or Goal Oriented Action Planning is a powerful tool to create game AI. For all the details I will refer to [Jeff Orkin's collection of articles](http://web.media.mit.edu/~jorkin/goap.html). But in short: GOAP will let computer controlled characters (NPCs) make action plans that can achieve desired goals. It will do so in a highly maintainable, easily extendible, highly modular fashion. Naive implementation of AI code will invariably blow up for any non trivial problem. GOAP on the other hand, is robust and is unlikely to buckle under large complexity. This software implements GOAP in the C programming language. It does so in a generic fashion, which makes it suitable for many projects.

## Basics of GOAP
Creating a plan for AI controlled entities comes down to the following steps:

1. Describe the repertoire of actions available.
2. Describe the current state of the world.
3. Describe the desired state of the world (goal).

To describe the actions, we specify:

* The preconditions for the action.
* The postconditions (effects) of the action.
* The cost of the action.

To describe world state, we define a set of world state atoms. Each atom has a tag, and a boolean value.

The planner will then be able to formulate a plan of actions that takes the world to the desired state, provided such a path exists. The plan formulated is guaranteed to be the lowest cost plan.

## Example Scenario

Let us consider a planner for an AI soldier. Our soldier can perform the following actions: scout, approach, aim, shoot, load, detonatebomb, flee.

* **scout** requires: *armedwithgun*. Effect: *enemyvisible*.
* **approach** requires: *enemyvisible*. Effect: *nearenemy*.
* **aim** requires: *enemyvisible* and *weaponloaded*. Effect: *enemylinedup*.
* **shoot** requires: *enemylinedup*. Effect: *!enemyalive*.
* **load** requires: *armedwithgun*. Effect: *weaponloaded*.
* **detonatebomb** requires: *armedwithbomb* and *nearenemy*. Effect: *!alive* and *!enemyalive*.
* **flee** requires: *enemyvisible*. Effect: *!nearenemy*.

Next, we will tell the planner that currently:
Enemy is not visible, we are armed with gun, our weapon is not loaded, enemy is not lined up, enemy is alive, we are armed with bomb, we are not near enemy, we are alive.

Then we tell our planner what our desired world looks like. We only care about one thing: our enemy is not alive.

With this, the planner can formulate a plan of actions to make this happen. For the moment, let's assume all our actions have the default cost 1 associated with it. This is what the planner will return to us:

                           ARMEDWITHGUN,enemyvisible,nearenemy,weaponloaded,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    0: scout               ARMEDWITHGUN,ENEMYVISIBLE,nearenemy,weaponloaded,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    1: approach            ARMEDWITHGUN,ENEMYVISIBLE,NEARENEMY,weaponloaded,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    2: detonatebomb        ARMEDWITHGUN,ENEMYVISIBLE,NEARENEMY,weaponloaded,enemylinedup,enemyalive,ARMEDWITHBOMB,alive,

Note: this notation uses lowercase if the condition is false, and uppercase if the condition is true.
The first line shows the current world state, and the last line the desired world state, with all intermediate states in between. The plan to execute is: (scout, approach, detonatebomb).

If we follow this plan, we have the unfortunate side effect that not only our enemy dies, but we die as well. This is easily solved by making detonatingbomb a higher cost action. But a more elegant approach would be to change our desired world state, and tell the planner that not only do we want our enemy dead, we want to ourselves alive. The planner will now create:

                           ARMEDWITHGUN,enemyvisible,nearenemy,weaponloaded,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    0: scout               ARMEDWITHGUN,ENEMYVISIBLE,nearenemy,weaponloaded,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    1: load                ARMEDWITHGUN,ENEMYVISIBLE,nearenemy,WEAPONLOADED,enemylinedup,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    2: aim                 ARMEDWITHGUN,ENEMYVISIBLE,nearenemy,WEAPONLOADED,ENEMYLINEDUP,ENEMYALIVE,ARMEDWITHBOMB,ALIVE,
    3: shoot               ARMEDWITHGUN,ENEMYVISIBLE,nearenemy,WEAPONLOADED,ENEMYLINEDUP,enemyalive,ARMEDWITHBOMB,ALIVE,


## Example Code

The entire scenario described above is implemented succinctly in these few lines of code:

	#include "goap.h"
	#include "astar.h"

	actionplanner_t ap;
	goap_actionplanner_clear( &ap ); // initializes action planner

	// describe repertoire of actions
	goap_set_pre( &ap, "scout", "armedwithgun", true );
	goap_set_pst( &ap, "scout", "enemyvisible", true );

	goap_set_pre( &ap, "approach", "enemyvisible", true );
	goap_set_pst( &ap, "approach", "nearenemy", true );

	goap_set_pre( &ap, "aim", "enemyvisible", true );
	goap_set_pre( &ap, "aim", "weaponloaded", true );
	goap_set_pst( &ap, "aim", "enemylinedup", true );

	goap_set_pre( &ap, "shoot", "enemylinedup", true );
	goap_set_pst( &ap, "shoot", "enemyalive", false );

	goap_set_pre( &ap, "load", "armedwithgun", true );
	goap_set_pst( &ap, "load", "weaponloaded", true );

	goap_set_pre( &ap, "detonatebomb", "armedwithbomb", true );
	goap_set_pre( &ap, "detonatebomb", "nearenemy", true );
	goap_set_pst( &ap, "detonatebomb", "alive", false );
	goap_set_pst( &ap, "detonatebomb", "enemyalive", false );

	goap_set_pre( &ap, "flee", "enemyvisible", true );
	goap_set_pst( &ap, "flee", "nearenemy", false );

	// describe current world state.
	worldstate_t fr; 
	goap_worldstate_clear( &fr );
	goap_worldstate_set( &ap, &fr, "enemyvisible", false );
	goap_worldstate_set( &ap, &fr, "armedwithgun", true );
	goap_worldstate_set( &ap, &fr, "weaponloaded", false );
	goap_worldstate_set( &ap, &fr, "enemylinedup", false );
	goap_worldstate_set( &ap, &fr, "enemyalive", true );
	goap_worldstate_set( &ap, &fr, "armedwithbomb", true );
	goap_worldstate_set( &ap, &fr, "nearenemy", false );
	goap_worldstate_set( &ap, &fr, "alive", true );

	// describe desired world state.
	worldstate_t goal;
	goap_worldstate_clear( &goal );
	goap_worldstate_set( &ap, &goal, "enemyalive", false );
	//goap_worldstate_set( &ap, &goal, "alive", true ); // add this to avoid suicide actions in the plan.

	worldstate_t states[16];
	const char* plan[16]; // The planner will return the action plan in this array.
	int plansz=16; // Size of our return buffers.
	const int plancost = astar_plan( &ap, fr, goal, plan, states, &plansz );

And that is it. The plan's size will be returned in *plansz* which is not necessarily the same as the plan cost. You can access the *plan* array elements for the actual steps (actions). For each action in the plan, the resulting world state is also available in the *states* array.

## Implementation Notes

To build, use a C99 compliant C compiler. You can invoke with: `$ gcc -std=c99 astar.c goap.c main.c`

The strength of GPGOAP is that the API is very generic: both world state and actions are solely described with C strings and booleans. No game specific enums end up in the API. With this power comes responsibility: do not make typos in the action names or atom names. They will end up representing different atoms if you do.

For performance, all the tags for the world state atoms are converted to an entry in a bit field. This bit field is implemented as a 'long long int', which typically is 8 bytes or 64 bits. This means that you cannot use more than 64 atoms to describe the world and the actions to the planner. When using multiple NPCs in your game, I suggest using separate planners for them, so that if two NPCs have different action sets, you don't end up combining their atom name space and exceeding 64 tags.

The only atom type supported is boolean. This means that scalars cannot be used to describe the world. I advice splitting up a scalar value with multiple booleans. E.g. fuelempy, fuellow, fuelfull to encode a single scalar fuel value.

I strongly advice using lowercase names for the world state atoms. I have provided a debug function that will return a string representation of a world state that you can print out. In this representation, lowercase is used for false valued atoms, and uppercase is used for true valued atoms.

## Bugs

No known bugs.

## Files

* **goap.h goap.c** implements the planner.
* **astar.h astar.c** implements A* search over the world state space.
* **main.c** sample scenario.

goap and astar are codependent unfortunately. Keeping them in separate files makes sense though, as they address two distinct parts of the system.

## Thanks

* Thank you Amit Patel for [describing](http://theory.stanford.edu/~amitp/GameProgramming/ImplementationNotes.html) A* search so succinctly.
* Thank you Jeff Orkin for the origins of GOAP.

## License

Copyright 2012 [Abraham T. Stolk](http://stolk.org)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
