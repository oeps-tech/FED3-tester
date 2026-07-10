#ifndef _EXECUTION_H_
#define _EXECUTION_H_

typedef struct
{
	bool first_run;
	bool new_entry;
	bool timeout_elapsed;
	uint16_t timeout;
	uint8_t current_state;
	int8_t pass_or_fail;
} s_t;

int8_t back_to_s0 (void);
int8_t go_to_fail (void);
int8_t go_to_pass (void);

int8_t get_current_execution_state (void);
int8_t get_pass_or_fail (void);


void state_machine (void);


#define MAINTAIN_STATE   return 0
#define GO_TO_NEXT_STATE return 1
#define GO_TO_FAIL       return 125 - s.current_state;
#define GO_TO_PASS       return 126 - s.current_state;
#define BACK_TO_STATE_0  return s.current_state * -1; 

#endif /* _EXECUTION_H_ */