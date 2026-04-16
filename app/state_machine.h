#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../core/STD_TYPES.h"

/* Public APIs */
void StateMachine_Init(void);
void StateMachine_Update(void);
void StateMachine_EmergencyReset(void);
void StateMachine_DoorbellTrigger(void);

#endif