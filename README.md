## Instruction Set Architecture ##
  
This program computes the following:

````
for (i = 0; i < 3; i++)
  A[i + 5] = A[i] + 5;
````

Below are the corresponding MIPS instructions:

````
addi r1, r0, 4   ; 0
sw   r1, 0(r0)   ; 4
addi r1, r0, 2   ; 8
sw   r1, 4(r0)   ; 12
addi r1, r0, 1   ; 16
sw   r1, 8(r0)   ; 20 
addi r1, r0, 0   ; 24 *loop setup begins here*
addi r3, r0, 12  ; 28
addi r4, r0, 5   ; 32
lw   r2, 0(r1)   ; 36
add  r2, r2, r4  ; 40
sw   r2, 20(r1)  ; 44
addi r1, r1, 4   ; 48
bne  r1, r3, -20 ; 52 *jump back to lw*
````


