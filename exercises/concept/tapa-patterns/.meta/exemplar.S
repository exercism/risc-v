.text
.global count_distinct_motifs
.global most_common_motif
.global dye_for_row
.global rotate_row
.global smooth_row

/* Provided by the tests: extern int32_t dye_needed(const int32_t *tally, int32_t kinds); */

# Fills tally[0..15] with how many times each motif appears in the row.
# row arrives in a0, n in a1, and the address of the tally in a2.
# A leaf helper: it uses only a and t registers.
tally_motifs:
        mv      t0, a2                  # t0 = pointer into the tally
        addi    t1, a2, 64              # t1 = end of the 16-word tally
tally_motifs_clear:
        beq     t0, t1, tally_motifs_count
        sw      zero, 0(t0)
        addi    t0, t0, 4
        j       tally_motifs_clear
tally_motifs_count:
        slli    t1, a1, 2
        add     t1, a0, t1              # t1 = end of the row
tally_motifs_loop:
        beq     a0, t1, tally_motifs_done
        lw      t0, 0(a0)               # t0 = motif
        slli    t0, t0, 2
        add     t0, a2, t0              # t0 = the address of tally[motif]
        lw      t2, 0(t0)
        addi    t2, t2, 1
        sw      t2, 0(t0)               # tally[motif] += 1
        addi    a0, a0, 4
        j       tally_motifs_loop
tally_motifs_done:
        ret

/* extern int32_t count_distinct_motifs(const int32_t *row, int32_t n); */
count_distinct_motifs:
        addi    sp, sp, -80             # 64 bytes of tally, then ra
        sw      ra, 76(sp)

        mv      a2, sp                  # the tally is at the bottom of the frame
        call    tally_motifs

        mv      t0, sp                  # t0 = pointer into the tally
        addi    t1, sp, 64              # t1 = end of the tally
        li      a0, 0                   # a0 = motifs seen
count_distinct_motifs_loop:
        beq     t0, t1, count_distinct_motifs_done
        lw      t2, 0(t0)
        beqz    t2, count_distinct_motifs_next
        addi    a0, a0, 1               # this motif appears at least once
count_distinct_motifs_next:
        addi    t0, t0, 4
        j       count_distinct_motifs_loop
count_distinct_motifs_done:
        lw      ra, 76(sp)
        addi    sp, sp, 80
        ret

/* extern int32_t most_common_motif(const int32_t *row, int32_t n); */
most_common_motif:
        addi    sp, sp, -80             # 64 bytes of tally, then ra
        sw      ra, 76(sp)

        mv      a2, sp
        call    tally_motifs

        li      t0, 0                   # t0 = motif being checked
        li      t1, 16
        li      a0, 0                   # a0 = best motif so far
        lw      t2, 0(sp)               # t2 = its count
most_common_motif_loop:
        addi    t0, t0, 1
        bge     t0, t1, most_common_motif_done
        slli    t3, t0, 2
        add     t3, sp, t3
        lw      t3, 0(t3)               # t3 = tally[motif]
        ble     t3, t2, most_common_motif_loop  # a tie keeps the smaller motif
        mv      a0, t0
        mv      t2, t3
        j       most_common_motif_loop
most_common_motif_done:
        lw      ra, 76(sp)
        addi    sp, sp, 80
        ret

/* extern int32_t dye_for_row(const int32_t *row, int32_t n); */
dye_for_row:
        addi    sp, sp, -80             # 64 bytes of tally, then ra
        sw      ra, 76(sp)

        mv      a2, sp
        call    tally_motifs
        mv      a0, sp                  # pass the address of the tally
        li      a1, 16
        call    dye_needed              # a0 = spoonfuls of dye

        lw      ra, 76(sp)
        addi    sp, sp, 80
        ret

/* extern void rotate_row(int32_t *row, int32_t n, int32_t k); */
rotate_row:
        addi    sp, sp, -16
        sw      s0, 12(sp)
        mv      s0, sp                  # s0 = frame pointer

        slli    t0, a1, 2               # t0 = 4 * n
        addi    t0, t0, 15
        andi    t0, t0, -16             # rounded up to a multiple of 16
        sub     sp, sp, t0              # copy[0..n-1] starts at sp

        li      t0, 0                   # t0 = i
rotate_row_copy:
        bge     t0, a1, rotate_row_copied
        slli    t1, t0, 2
        add     t1, a0, t1
        lw      t2, 0(t1)               # t2 = row[i]
        add     t1, t0, a2
        rem     t1, t1, a1              # t1 = (i + k) % n
        slli    t1, t1, 2
        add     t1, sp, t1
        sw      t2, 0(t1)               # copy[(i + k) % n] = row[i]
        addi    t0, t0, 1
        j       rotate_row_copy
rotate_row_copied:
        li      t0, 0
rotate_row_back:
        bge     t0, a1, rotate_row_done
        slli    t1, t0, 2
        add     t2, sp, t1
        lw      t3, 0(t2)
        add     t2, a0, t1
        sw      t3, 0(t2)               # row[i] = copy[i]
        addi    t0, t0, 1
        j       rotate_row_back
rotate_row_done:
        mv      sp, s0                  # give the copy back
        lw      s0, 12(sp)
        addi    sp, sp, 16
        ret

/* extern void smooth_row(int32_t *row, int32_t n); */
smooth_row:
        addi    sp, sp, -16
        sw      s0, 12(sp)
        mv      s0, sp                  # s0 = frame pointer

        slli    t0, a1, 2               # t0 = 4 * n
        addi    t0, t0, 15
        andi    t0, t0, -16
        sub     sp, sp, t0              # old[0..n-1] starts at sp

        li      t0, 0                   # t0 = i
smooth_row_copy:
        bge     t0, a1, smooth_row_copied
        slli    t1, t0, 2
        add     t2, a0, t1
        lw      t3, 0(t2)
        add     t2, sp, t1
        sw      t3, 0(t2)               # old[i] = row[i]
        addi    t0, t0, 1
        j       smooth_row_copy
smooth_row_copied:
        li      t0, 1                   # t0 = i, from 1 to n - 2
        addi    t4, a1, -1              # t4 = n - 1
smooth_row_loop:
        bge     t0, t4, smooth_row_done
        slli    t1, t0, 2
        add     t2, sp, t1              # t2 = the address of old[i]
        lw      t3, -4(t2)              # t3 = old[i - 1]
        lw      t5, 4(t2)               # t5 = old[i + 1]
        bne     t3, t5, smooth_row_next
        add     t2, a0, t1
        sw      t3, 0(t2)               # row[i] = the neighbours' motif
smooth_row_next:
        addi    t0, t0, 1
        j       smooth_row_loop
smooth_row_done:
        mv      sp, s0                  # give the copy back
        lw      s0, 12(sp)
        addi    sp, sp, 16
        ret
