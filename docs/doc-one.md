# RVM
A custom LC-3 virtual machine.  by Rifey. 
This Project is done for the sole purpose of learning.


## Info
Simulate LC-3 Architecture
Simplified instruction set.

### Memory
- 65,536 Memory Locations ( that is maximum of  16 bit unsigned Integer ( 2^16 ) )
- Each Stores `16` bit value ( total of 128KB storage ).

### Registers
Slot for storing single value on CPU.
- `10` Registers ( `each 16 bits` )
    * `8 General Purpose ( R0 - R7 )`
        These `Registers` can be used to perform any program calculations.

    * `1 Program Counter ( PC )`
        This is an Unsigned Integer which is the address of the next instruction

    * `1 Condition Flag  ( COND )`
        This holds information about the previous calculation.

### Instruction set
Commands that tells cpu what kind of operation to do.
Each Instruction is `16 bit` long.
`4 bits` for `Opcode` and rest for the `Parameter`

- `Opcode` ( operation code )
    Indicated what kind of task to perform.
    * Represent 1 task for the CPU to perform 
    * `16` Opcodes in LC-3

- `Parameters`
    Provide input to the task being performed.
**NOTE: Smaller Instruction set is also called RISC and Larger ones are called CISC **

### Condition Flags
The `R_COND` Flag in Register stored the most recently executed calculation. This allows program to check 
Logical condition like ` if ( x < 0 ) {...}`

The LC-3 uses only `3` kind of condition flags which is the `sign` of the previous calculation,
while normal CPU has a variety of flags for different situations.
- P (Positive)
- Z (Zero)
- N (Negative)


## Implementing Instruction

### Bitwise `ADD` Instruction

- `Encoding`

| 15  12 | 11   9 | 8    6 | 5 | 4     3| 2    0 |
|--------|--------|--------|---|--------|--------|
| 0001   |   DR   |   SR1  | 0 |   00   |  SR2   |

| 15  12 | 11   9 | 8    6 | 5 | 4             0 |
|--------|--------|--------|---|-----------------|
| 0001   |   DR   |   SR1  | 1 |     imm5        |

The Encoding show `2` different rows as it is because there is `2 modes` for this instruction.

1. As we can see the first `4 bit` is the `OpCode` (0001), in this case `OP_ADD`.
2. The next `3 bit` are marked `DR` ( Destination Register ) . this is register where the added sum will be stored. 
3. The next `3 bit` is `SR1`, the Register containing the First number to ADD.

So no we know where the sum is gonna be stored and the first number to ADD. the next thing to know is 
the second number. this is where the 2 rows shows difference. 
Basically the 2 modes is `Register Mode` and `Immediate Mode`. 
The top row, 5th bit is 0 and the second row 5th bit is 1. this is the to show whether it is `0, Register`, `1, Immediate` Mode.

- `Register Mode`
The Top row , which is `Register Mode`, has a Register given ( bit 2 and 0 ) marked `SR2` and the bit 4 and 3 is unused .

In Assembly it will look like this:

```asm 
; Register Mode Bitwise Add

ADD R2 R0 R1  ; add the contents of R0 to R1 and store it in R2.
```

- `Immediate Mode`
The Bottom row, which is `Immediate Mode`, has its Second Number embedded into the `imm5`. it reduces the length of the typical program.
the only tradeoff is that, the instruction only has room for 2^5=32 (unsigned) to be exact, making it useful to use in the case of incrementing and decrementing. 

``` asm
; Immediate Mode Bitwise Add

ADD R0 R0 1  ; add 1 to R0 and store back in R0
```

- `SUMMARY of Bitwise ADD`
1.  If bit [5] is 0, the second source operand is obtained from SR2.
2.  If bit [5] is 1, the second source operand is obtained by sign-extending the imm5 field to 16 bits.
3.  In both cases, the second source operand is added to the contents of SR1 and the result stored in DR.

wait?? what is sign-extending??. remember in `Immediate Mode`, we have just `5 bits` to store the second number. when doing
the addition, we are dealing with `16 bit` integers and it has to be extended. but the problems arise when dealing with 
signed integers. positive or negative. for positive, we can just fill the rest with 0s and it will be ok. but the 
problem arises when dealing with negative. lets say we have -1 and in 5 bits it can look like `1 1111`. so when we
fill 0s it will look like `0000 0000 0001 1111` which is 31. so `sign-extending` helps remove this problem by filling
in 0’s for positive numbers and 1’s for negative numbers, so that original values are preserved.

- `Condition Flags update.`

And lastly we need to update the condition Flag register depending on the operation we did. we update this register
to indicate the signs ( positive, negative, zero). 


***Docs continued -> doc-two.md***
