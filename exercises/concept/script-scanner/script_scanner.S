.option arch, +v                        # allow vector instructions in this file

.text
.global script_length
.global count_pauses
.global count_capitals
.global shout
.global find_cue
.global copy_line

/* extern int32_t script_length(const char *line); */
script_length:
        ret

/* extern int32_t count_pauses(const char *line); */
count_pauses:
        ret

/* extern int32_t count_capitals(const char *line); */
count_capitals:
        ret

/* extern void shout(char *line); */
shout:
        ret

/* extern int32_t find_cue(const char *line, char marker); */
find_cue:
        ret

/* extern void copy_line(char *dest, const char *src); */
copy_line:
        ret
