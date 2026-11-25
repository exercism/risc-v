.text
.global record_count
.global longest_record
.global find_name
.global count_starting_with
.global grid_char
.global collect_short

/* extern int32_t record_count(const char *const *records); */
record_count:
        ret

/* extern int32_t longest_record(const char *const *records, int32_t count); */
longest_record:
        ret

/* extern int32_t find_name(const char *const *names, int32_t count, const char *name); */
find_name:
        ret

/* extern int32_t count_starting_with(const char *const *records, int32_t count, char letter); */
count_starting_with:
        ret

/* extern char grid_char(const char *const *rows, int32_t row, int32_t col); */
grid_char:
        ret

/* extern int32_t collect_short(const char **out, const char *const *records, int32_t count, int32_t max_len); */
collect_short:
        ret
