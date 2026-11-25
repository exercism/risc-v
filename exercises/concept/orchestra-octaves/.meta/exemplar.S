.text
.global octave_up
.global octave_down
.global note_octave
.global note_duration
.global pack_note
.global offset_after_turns

/* extern uint32_t octave_up(uint32_t frequency, uint32_t octaves); */
octave_up:
        sll     a0, a0, a1
        ret

/* extern uint32_t octave_down(uint32_t frequency, uint32_t octaves); */
octave_down:
        srl     a0, a0, a1
        ret

/* extern uint32_t note_octave(uint32_t note); */
note_octave:
        srli    a0, a0, 8
        andi    a0, a0, 0xF
        ret

/* extern uint32_t note_duration(uint32_t note); */
note_duration:
        srli    a0, a0, 12
        andi    a0, a0, 0xF
        ret

/* extern uint32_t pack_note(uint32_t pitch, uint32_t octave, uint32_t duration); */
pack_note:
        slli    t0, a1, 8
        or      a0, a0, t0
        slli    t0, a2, 12
        or      a0, a0, t0
        ret

/* extern int32_t offset_after_turns(int32_t cents, uint32_t turns); */
offset_after_turns:
        sra     a0, a0, a1
        ret
