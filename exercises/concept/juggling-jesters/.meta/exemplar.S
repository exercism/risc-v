.text
.global catch_count
.global act_score
.global encore
.global show_total
.global crowd_rating

/* extern int32_t catch_count(int32_t balls, int32_t rounds); */
catch_count:
        mul     a0, a0, a1
        ret

/* extern int32_t act_score(int32_t balls, int32_t rounds); */
act_score:
        mv      t6, ra                  # keep our return address
        call    catch_count             # a0 = catches
        add     a0, a0, a0              # 2 points per catch
        addi    a0, a0, 10              # 10 points for the bow
        mv      ra, t6
        ret

/* extern int32_t encore(int32_t balls, int32_t rounds); */
encore:
        mv      t6, ra                  # keep our return address
        mv      t4, a0                  # balls
        mv      t5, a1                  # rounds
        call    catch_count
        mv      t3, a0                  # catches in the first show
        addi    a0, t4, 1               # one more ball for the encore
        mv      a1, t5
        call    catch_count
        add     a0, a0, t3
        mv      ra, t6
        ret

/* extern int32_t show_total(int32_t acts, int32_t rounds); */
show_total:
        mv      t6, ra                  # keep our return address
        mv      t3, a0                  # acts
        mv      t4, a1                  # rounds
        li      t5, 0                   # total
        li      t2, 1                   # balls in this act
show_total_loop:
        bgt     t2, t3, show_total_done
        mv      a0, t2
        mv      a1, t4
        call    catch_count
        add     t5, t5, a0
        addi    t2, t2, 1
        j       show_total_loop
show_total_done:
        mv      a0, t5
        mv      ra, t6
        ret

# Returns card, moved into the range 0 to 10.
# Called with jal t5, so it returns with jr t5. Changes only a0 and t0.
fair_mark:
        bgez    a0, fair_mark_not_negative
        li      a0, 0
fair_mark_not_negative:
        li      t0, 10
        ble     a0, t0, fair_mark_done
        mv      a0, t0
fair_mark_done:
        jr      t5

/* extern int32_t crowd_rating(int32_t first, int32_t second, int32_t third); */
crowd_rating:
        mv      t1, a1                  # second
        mv      t2, a2                  # third
        jal     t5, fair_mark
        mv      t3, a0                  # total
        mv      a0, t1
        jal     t5, fair_mark
        add     t3, t3, a0
        mv      a0, t2
        jal     t5, fair_mark
        add     a0, a0, t3
        ret
