.text
.global colony_size
.global band_total
.global climbing_routes
.global count_family
.global count_generations
.global largest_wingspan

/* struct pterodactyl { int32_t wingspan; const struct pterodactyl *first_chick; const struct pterodactyl *second_chick; }; */
/* Provided by the tests: extern int32_t larger(int32_t a, int32_t b); */

/* extern int32_t colony_size(int32_t generations); */
colony_size:
        bnez    a0, colony_size_recurse
        li      a0, 1                   # no generations: just the founder
        ret
colony_size_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)

        addi    a0, a0, -1
        call    colony_size             # a0 = the family of one chick
        add     a0, a0, a0              # two chicks
        addi    a0, a0, 1               # and the parent

        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t band_total(int32_t band); */
band_total:
        li      t0, 10
        bge     a0, t0, band_total_recurse
        ret                             # a single digit is its own total
band_total_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)

        rem     s0, a0, t0              # the last digit
        div     a0, a0, t0              # the other digits
        call    band_total
        add     a0, a0, s0

        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t climbing_routes(int32_t ledges); */
climbing_routes:
        li      t0, 1
        bgt     a0, t0, climbing_routes_recurse
        li      a0, 1                   # 0 or 1 ledges: one route
        ret
climbing_routes_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)

        mv      s0, a0                  # ledges
        addi    a0, s0, -1
        call    climbing_routes         # routes that start with a hop of 1
        mv      s1, a0
        addi    a0, s0, -2
        call    climbing_routes         # routes that start with a hop of 2
        add     a0, a0, s1

        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t count_family(const struct pterodactyl *root); */
count_family:
        bnez    a0, count_family_recurse
        ret                             # nobody here: a0 is already 0
count_family_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)

        mv      s0, a0                  # this pterodactyl
        lw      a0, 4(s0)
        call    count_family            # the first chick's family
        mv      s1, a0
        lw      a0, 8(s0)
        call    count_family            # the second chick's family
        add     a0, a0, s1
        addi    a0, a0, 1               # and this pterodactyl

        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t count_generations(const struct pterodactyl *root); */
count_generations:
        bnez    a0, count_generations_recurse
        ret                             # nobody here: no generations
count_generations_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)

        mv      s0, a0                  # this pterodactyl
        lw      a0, 4(s0)
        call    count_generations       # generations below the first chick
        mv      s1, a0
        lw      a0, 8(s0)
        call    count_generations       # generations below the second chick
        bge     a0, s1, count_generations_deeper
        mv      a0, s1                  # the first chick's line is longer
count_generations_deeper:
        addi    a0, a0, 1               # and this pterodactyl's generation

        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t largest_wingspan(const struct pterodactyl *root); */
largest_wingspan:
        bnez    a0, largest_wingspan_recurse
        ret                             # nobody here: 0
largest_wingspan_recurse:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)

        mv      s0, a0                  # this pterodactyl
        lw      a0, 4(s0)
        call    largest_wingspan        # the widest in the first chick's family
        mv      s1, a0
        lw      a0, 8(s0)
        call    largest_wingspan        # the widest in the second chick's family
        mv      a1, s1
        call    larger
        lw      a1, 0(s0)               # this pterodactyl's own wingspan
        call    larger

        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
