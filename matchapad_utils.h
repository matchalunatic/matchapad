#pragma once
#include "matchapad_types.h"
#include "matchapad_seq.h"
void matchapad_state_dump(void);
void matchapad_internal_state_dump(void);
void matchapad_switch_mode(matchapad_mode mode);
void matchapad_rotate_mode(void);
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;
