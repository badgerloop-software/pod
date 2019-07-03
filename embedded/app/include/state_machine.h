#ifndef SMACHINE_H
#define SMACHINE_H
#define STATE_ERROR() ({                \
          printf("ERROR exiting...\n"); \
          exit(-1);                     \
        })
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


#define NUM_STATES              15
#define FAULT                   "fault"

#define PRE_RUN_FAULT_NAME      FAULT"Pre"
#define POST_RUN_FAULT_NAME     FAULT"Post"
#define RUN_FAULT_NAME          FAULT"Run"
#define POWER_OFF_NAME          "powerOff"
#define IDLE_NAME               "idle"
#define PUMPDOWN_NAME           "pumpdown"
#define READY_FOR_PUMPDOWN_NAME "readyForPumpdown"
#define READY_NAME              "readyForPropulsion"
#define PROPULSION_NAME         "propulsion"
#define BRAKING_NAME            "braking"
#define STOPPED_NAME            "stopped"
#define CRAWL_NAME              "crawl"
#define POST_RUN_NAME           "postRun"
#define SAFE_TO_APPROACH_NAME   "safeToApproach"

#define BLANK_NAME              "none"

void buildStateMachine(void);

void runStateMachine();

typedef struct state_t state_t;
typedef struct stateTransition_t stateTransition_t;
typedef struct stateMachine_t stateMachine_t;


state_t *getCurrState(void);

state_t *findState(char *name);
/*
* The state machine is a directed graph. Each edge is a transition
* The state_t handles the nodes and stateTransition_t is an edge
*
 */

typedef struct stateTransition_t {
	state_t *target;
	int (*action)(); //TODO: Any particular reason you're using a bool pointer here rather that void?
} stateTransition_t;

/* 	struct: state_t
*  	
* 	fields:
*		action      = The thing that the state does. If it should transition it returns
*			        a pointer to the transiton, otherwise NULL
*		
*		name        = The name of the state
*		
*		transitions = A list of all transitions exiting the state
*/

typedef struct state_t {
	stateTransition_t *(*action)();
	char *name; // FIXME Thinking about switching this to a number
	stateTransition_t **transitions;
    int numTransitions;
    int transitionCounter;
} state_t;

typedef struct stateMachine_t {
	state_t *currState;
    char *  overrideStateName;
	state_t **allStates;
} stateMachine_t;

#endif
