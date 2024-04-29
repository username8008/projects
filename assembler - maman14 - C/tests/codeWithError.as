.extern LOOP
    .entry LOOP
    .extern W5
MAIN:  mov S1 ,LENGTHAAAAAAAAAAAAA
add ,r2    ,STR
LOOP: jmp END

prn #-5
prn #-129
prn 			r1,	,r4
sub r1,	r4 

m1
1bne LOOP

1mov: jmp ,S2
STR: .string ,"abcdef"

LENGTH: .data 6,-9,15,9.0,513,-513
    K: .data 22,   ,

    ;commentttt
    
    MAIN: mov S1 ,LENGTH
    
add r2,STR
LOOP: jmp END
prn #-5
sub r1, r4
bne LOOP
END: hlt
STR: .string "abcdef"
LENGTH: .data 6,-9,	,	15
K: .data 22
S1: .struct 8, "ab"
    jmp r1, ,r3
    
    
