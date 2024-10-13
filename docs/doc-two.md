`Docs second part`

## Implementing Instructions continued.

### LDI ( Load Indirect )
This instruction is used to load a value from a location in a memory to a `Register`.

- `Encoding`

| 15  12 | 11   9 | 8                         0 |
|--------|--------|-----------------------------|
|  1010  |   DR   |          PCoffset9          |

1.  In contrast to ADD, there is no modes and has fewer parameters.
2.  `4 bit` `OPCODE` 1010 is correspondent to `OP_LDI`.
3.  `3 bit` `DR` (Destination Register) for storing the loaded value.
4.  The rest `PCoffset9` is just like `imm5` from ADD, which is also 
    embedded and Immidiate Value. since this is loaded from memory, 
    we can guess this number is kind of `address` which tell us where
    to load from.
    -   As this is a `9 bit` value, we need to `Sign-extending` to `16 bits` and unlike ADD
        we need to add this `PC` Register.
        `NOTE:` (If we look back to the loop, the PC register is already incremented right after this 
        Instruction is loaded) 
5. `LDI` is useful for loading values that are stored far away from the  `PC`.
    but to use it, the address of the final location needs to be stored in a neighbourhood nearby.
    You can think of it like having a local variable in C which is a pointer to some data:

- `LDI Sample in C`

```c
// the value of test_buffer is an address
// and the test_buffer itself has an address.
char* test_buffer = "spaceship"

//layout in memory might look like this:
//------------------------------------

//Address   Label           Value
//0x123:    test_Buffer =   0x456
//...
//0x456:    string      =   's'

//If PC was at 0x100
//LDI R0 0x123
//This would load 's' into R0
```
Same as before, the `Condition Flag` must be updated after putting value into `DR`



### BR ( Branch )
This instruction is used to branch / set the `PC` to offset to another location of the memory. 

- `Encoding`

| 15  12 | 11   9 | 8                         0 |
|--------|--------|-----------------------------|
|  0000  |   NZP   |          PCoffset9         |

1. The last `4bit` for Opcode.
2. The `NZP` is the `3bit` binary space for conditional branching.
    Each has a `1bit` space, N -> Negative, Z -> Zero, P -> Positive.
    Which means we should check the `COND` Register and check if it matches then do branching.
    For example if the N bit is 1, and the COND REGISTER is set NEGATIVE flag, then do branching.



### LD ( Load )
This instruction is little bit similar to the Load Indirect but instead , this instrction 
will have the loading Address once.
- `Encoding is the same as LDI`

Here is the simplified way to look at:
```c
//Load instruction
//consider the PC is a pointer (address) and LABEL is the offset
R1 = MEMORY[PC + LABEL]; //directly load from the address.

//unlike this the Load Indirect is the following.
temp_address = MEMORY[PC + LABEL] // we get from this address location, another address

R1 = MEMORY[temp_address]; //indirect loading 
```

### ST ( Store ) 
This instruction basically has a SR ( Source Register ) instead of DR, where the value 
of this register is to store to the offset given.
- `Encoding`

| 15  12 | 11   9 | 8                         0 |
|--------|--------|-----------------------------|
|  0000  |   SR   |          PCoffset9         |

in Assembly it might look like this:
```asm
ST R2, LABEL ; Store the value in R2 to the memory address (PC + LABEL)
```


