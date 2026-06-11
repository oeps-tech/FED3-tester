#ifndef _EXECUTION_H_
#define _EXECUTION_H_

typedef struct
{
	bool new_entry;
	bool timeout_elapsed;
	uint16_t timeout;
	uint8_t current_state;
	int8_t pass_or_fail;
} s_t;


int8_t get_current_execution_state (void);
int8_t get_pass_or_fail (void);

void state_machine (void);

#endif /* _EXECUTION_H_ */