.text
.global record_count
.global longest_record
.global find_name
.global count_starting_with
.global grid_char
.global collect_short

/* extern int32_t record_count(const char *const *records); */
record_count:
        mv      t0, a0                  # pointer to the current element
.Lrecord_count_loop:
        lw      t1, 0(t0)
        beqz    t1, .Lrecord_count_done # the null pointer ends the list
        addi    t0, t0, 4
        j       .Lrecord_count_loop
.Lrecord_count_done:
        sub     a0, t0, a0              # bytes from the start to the null pointer
        srli    a0, a0, 2               # each element is 4 bytes
        ret

/* extern int32_t longest_record(const char *const *records, int32_t count); */
longest_record:
        li      t0, 0                   # index of the current record
        li      t1, 0                   # index of the longest so far
        li      t2, -1                  # length of the longest so far
.Llongest_record_outer:
        bge     t0, a1, .Llongest_record_done
        slli    t3, t0, 2
        add     t3, a0, t3
        lw      t3, 0(t3)               # t3 = records[i]
        mv      t4, t3                  # t4 = pointer into the record
.Llongest_record_inner:
        lbu     t5, 0(t4)
        beqz    t5, .Llongest_record_measured
        addi    t4, t4, 1
        j       .Llongest_record_inner
.Llongest_record_measured:
        sub     t4, t4, t3              # t4 = length of records[i]
        ble     t4, t2, .Llongest_record_next  # not longer: keep the earlier one
        mv      t2, t4
        mv      t1, t0
.Llongest_record_next:
        addi    t0, t0, 1
        j       .Llongest_record_outer
.Llongest_record_done:
        mv      a0, t1
        ret

/* extern int32_t find_name(const char *const *names, int32_t count, const char *name); */
find_name:
        li      t0, 0                   # index of the current name
.Lfind_name_outer:
        bge     t0, a1, .Lfind_name_missing
        slli    t1, t0, 2
        add     t1, a0, t1
        lw      t1, 0(t1)               # t1 = pointer into names[i]
        mv      t2, a2                  # t2 = pointer into name
.Lfind_name_inner:
        lbu     t3, 0(t1)
        lbu     t4, 0(t2)
        bne     t3, t4, .Lfind_name_next  # the texts differ
        beqz    t3, .Lfind_name_found   # both ended together
        addi    t1, t1, 1
        addi    t2, t2, 1
        j       .Lfind_name_inner
.Lfind_name_next:
        addi    t0, t0, 1
        j       .Lfind_name_outer
.Lfind_name_found:
        mv      a0, t0
        ret
.Lfind_name_missing:
        li      a0, -1
        ret

/* extern int32_t count_starting_with(const char *const *records, int32_t count, char letter); */
count_starting_with:
        slli    t0, a1, 2
        add     t0, a0, t0              # end of the array
        li      t1, 0                   # count
.Lcount_starting_with_loop:
        beq     a0, t0, .Lcount_starting_with_done
        lw      t2, 0(a0)               # t2 = the current record
        lbu     t2, 0(t2)               # t2 = its first character
        bne     t2, a2, .Lcount_starting_with_next
        addi    t1, t1, 1
.Lcount_starting_with_next:
        addi    a0, a0, 4
        j       .Lcount_starting_with_loop
.Lcount_starting_with_done:
        mv      a0, t1
        ret

/* extern char grid_char(const char *const *rows, int32_t row, int32_t col); */
grid_char:
        slli    t0, a1, 2
        add     t0, a0, t0
        lw      t0, 0(t0)               # t0 = rows[row]
        add     t0, t0, a2
        lbu     a0, 0(t0)               # rows[row][col]
        ret

/* extern int32_t collect_short(const char **out, const char *const *records, int32_t count, int32_t max_len); */
collect_short:
        slli    t0, a2, 2
        add     t0, a1, t0              # end of records
        li      t1, 0                   # how many pointers written to out
.Lcollect_short_outer:
        beq     a1, t0, .Lcollect_short_done
        lw      t2, 0(a1)               # t2 = the current record
        mv      t3, t2                  # t3 = pointer into the record
.Lcollect_short_inner:
        lbu     t4, 0(t3)
        beqz    t4, .Lcollect_short_measured
        addi    t3, t3, 1
        j       .Lcollect_short_inner
.Lcollect_short_measured:
        sub     t3, t3, t2              # t3 = its length
        bgt     t3, a3, .Lcollect_short_next  # too long
        sw      t2, 0(a0)               # out[n] = the record's pointer
        addi    a0, a0, 4
        addi    t1, t1, 1
.Lcollect_short_next:
        addi    a1, a1, 4
        j       .Lcollect_short_outer
.Lcollect_short_done:
        mv      a0, t1
        ret
