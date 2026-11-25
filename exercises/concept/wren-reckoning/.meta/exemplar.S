.text
.global total_sightings
.global weeks_to_train
.global digit_count
.global digit_sum
.global reverse_digits

/* extern int32_t total_sightings(int32_t days); */
total_sightings:
        li      t0, 0                   # total
        li      t1, 1                   # day
total_sightings_loop:
        bgt     t1, a0, total_sightings_done
        add     t0, t0, t1
        addi    t1, t1, 1
        j       total_sightings_loop
total_sightings_done:
        mv      a0, t0
        ret

/* extern int32_t weeks_to_train(int32_t needed); */
weeks_to_train:
        li      t0, 1                   # spotters
        li      t1, 0                   # weeks
weeks_to_train_loop:
        bge     t0, a0, weeks_to_train_done
        add     t0, t0, t0
        addi    t1, t1, 1
        j       weeks_to_train_loop
weeks_to_train_done:
        mv      a0, t1
        ret

/* extern int32_t digit_count(int32_t count); */
digit_count:
        li      t0, 0                   # digits
        li      t1, 10
digit_count_loop:
        addi    t0, t0, 1
        div     a0, a0, t1
        bnez    a0, digit_count_loop
        mv      a0, t0
        ret

/* extern int32_t digit_sum(int32_t band); */
digit_sum:
        li      t0, 0                   # sum
        li      t1, 10
digit_sum_loop:
        beqz    a0, digit_sum_done
        rem     t2, a0, t1
        add     t0, t0, t2
        div     a0, a0, t1
        j       digit_sum_loop
digit_sum_done:
        mv      a0, t0
        ret

/* extern int32_t reverse_digits(int32_t number); */
reverse_digits:
        li      t0, 0                   # reversed
        li      t1, 10
reverse_digits_loop:
        beqz    a0, reverse_digits_done
        rem     t2, a0, t1
        mul     t0, t0, t1
        add     t0, t0, t2
        div     a0, a0, t1
        j       reverse_digits_loop
reverse_digits_done:
        mv      a0, t0
        ret
