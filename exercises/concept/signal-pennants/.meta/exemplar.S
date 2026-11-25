.section .rodata
word_lengths:                           # the length of each code word, A to Z
        .byte   4, 5, 7, 5, 4, 7, 4, 5, 5       # Alfa to India
        .byte   7, 4, 4, 4, 8, 5, 4, 6, 5       # Juliett to Romeo
        .byte   6, 5, 7, 6, 7, 5, 6, 4          # Sierra to Zulu

alfa:
        .string "Alfa"
bravo:
        .string "Bravo"
charlie:
        .string "Charlie"
delta:
        .string "Delta"
echo:
        .string "Echo"
foxtrot:
        .string "Foxtrot"
golf:
        .string "Golf"
hotel:
        .string "Hotel"
india:
        .string "India"
juliett:
        .string "Juliett"
kilo:
        .string "Kilo"
lima:
        .string "Lima"
mike:
        .string "Mike"
november:
        .string "November"
oscar:
        .string "Oscar"
papa:
        .string "Papa"
quebec:
        .string "Quebec"
romeo:
        .string "Romeo"
sierra:
        .string "Sierra"
tango:
        .string "Tango"
uniform:
        .string "Uniform"
victor:
        .string "Victor"
whiskey:
        .string "Whiskey"
x_ray:
        .string "X-ray"
yankee:
        .string "Yankee"
zulu:
        .string "Zulu"

        .balign 4
code_words:                             # the address of each code word, A to Z
        .word   alfa, bravo, charlie, delta, echo, foxtrot, golf, hotel, india
        .word   juliett, kilo, lima, mike, november, oscar, papa, quebec, romeo
        .word   sierra, tango, uniform, victor, whiskey, x_ray, yankee, zulu

# white = 1, blue = 2, red = 4, yellow = 8, black = 16
flag_colour_sets:                       # the colours of each flag, A to Z
        .byte   1 + 2, 4, 2 + 1 + 4, 8 + 2, 2 + 4, 1 + 4, 8 + 2, 1 + 4, 8 + 16  # A to I
        .byte   2 + 1, 8 + 2, 8 + 16, 2 + 1, 2 + 1, 4 + 8, 2 + 1, 8, 4 + 8      # J to R
        .byte   1 + 2, 4 + 1 + 2, 4 + 1, 1 + 4, 2 + 1 + 4, 1 + 2, 8 + 4         # S to Y
        .byte   8 + 2 + 4 + 16                                                  # Z

set_sizes:                              # the number of colours in each set from 0 to 31
        .byte   0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
        .byte   1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5

.text
.global phonetic_length
.global phonetic_word
.global flag_colours
.global colour_count
.global spell_length

/* extern int32_t phonetic_length(char letter); */
phonetic_length:
        addi    t0, a0, -'A'            # t0 = 0 for 'A', up to 25 for 'Z'
        li      t1, 26
        bgeu    t0, t1, .Lphonetic_length_none
        la      t1, word_lengths
        add     t1, t1, t0
        lbu     a0, 0(t1)               # a0 = word_lengths[letter - 'A']
        ret
.Lphonetic_length_none:
        li      a0, -1                  # not a capital letter
        ret

/* extern const char *phonetic_word(char letter); */
phonetic_word:
        addi    t0, a0, -'A'            # t0 = 0 for 'A', up to 25 for 'Z'
        slli    t0, t0, 2               # each address is 4 bytes
        la      t1, code_words
        add     t1, t1, t0
        lw      a0, 0(t1)               # a0 = code_words[letter - 'A']
        ret

/* extern uint32_t flag_colours(char letter); */
flag_colours:
        la      t0, flag_colour_sets - 'A'      # where the entry for character 0 would be
        add     t0, t0, a0
        lbu     a0, 0(t0)               # a0 = flag_colour_sets[letter - 'A']
        ret

/* extern int32_t colour_count(char letter); */
colour_count:
        la      t0, flag_colour_sets - 'A'
        add     t0, t0, a0
        lbu     t1, 0(t0)               # t1 = the set of colours, from 0 to 31
        la      t0, set_sizes
        add     t0, t0, t1
        lbu     a0, 0(t0)               # a0 = set_sizes[colours]
        ret

/* extern int32_t spell_length(const char *message); */
spell_length:
        li      t0, 0                   # t0 = total = 0
        la      t1, word_lengths
        li      t2, 26
.Lspell_length_loop:
        lbu     t3, 0(a0)               # t3 = the current character
        beqz    t3, .Lspell_length_done
        addi    a0, a0, 1               # move on to the next character
        addi    t3, t3, -'A'            # t3 = 0 for 'A', up to 25 for 'Z'
        bgeu    t3, t2, .Lspell_length_loop     # not a capital letter: adds nothing
        add     t3, t1, t3
        lbu     t3, 0(t3)               # t3 = word_lengths[character - 'A']
        add     t0, t0, t3              # total = total + its length
        j       .Lspell_length_loop
.Lspell_length_done:
        mv      a0, t0                  # return total
        ret
