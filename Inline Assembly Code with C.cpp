#include <stdio.h>
#include <stdlib.h>

int main()
{
    /*** C code without interaction with Assembly ***/

    int x = 5;
    int y = 6;

    printf("x :: %d, y = %d, x+y = %d", x, y, x + y);

    /*** Assembly code without interaction with C ***/

    __asm {
        MOV AL, 0x05
        MOV BL, 0x06
        ADD AL, BL
        MOV AH, AL
    }

    /*** Inline Assembly in C ***/

    // Create a 1 byte variable in C
    char anyLetter;
    // Find out the memory location of the variable anyLetter
    char* ptr_anyLetter = &anyLetter;
    // Observe the memory location - 4 byte in this case
    // Notice as it is in the RAM the memory location will be assigned randomly based on availability
    printf("\nType a character and press ENTER: ");
    anyLetter = getchar();
    printf("Location of the letter %c is in the memory :: %p", anyLetter, ptr_anyLetter);
    printf("\nAt location %p the value is :: %c", ptr_anyLetter, *ptr_anyLetter);

    // Initialize some pointers to look the code flow
    int codeStartsAt;
    int* ptr_codeStartsAt = &codeStartsAt;

    // Capitalize using Assembly
    __asm {
        // Starting of Assembly code in C
        CALL GET_EIP                            // Try to get the EIP - 
                                                // Instruction pointer - address of memory the code starts
        MOV DWORD PTR[ptr_codeStartsAt], EAX    // Save this starting address to a pointer
        MOV EAX, DWORD PTR[ptr_anyLetter]       // Address of any memory cell is of length 32bit = 4-byte, i.e Double Word
                                                // Move the content of the memory cell addressed by ptr_anyLetter into EAX register
        CMP[EAX], 0x61                          // Compare if the character strored as ascii code in EAX is smaller than the 
                                                // ascii code of 'a' - do nothing in that case.
        JL NO_CAP
        CMP[EAX], 0x7A                          // Compare if the character strored as ascii code in EAX is bigger than the 
                                                // ascii code of 'z' - do nothing in that case.
        JA NO_CAP
        SUB[EAX], 0x20                          // If the character is a valid english small letter capitalize it by subtracting 
                                                // 100000B = 32 = 20H from its ascii code
        JMP CODE_ENDS                            // Finish the code
    GET_EIP:
        MOV EAX, [ESP]                          // Get stack pointer (top of the stack at any time)
        SUB EAX, 0005H                          // Go back to exactly locate the first line in inline assembly
        RET
    NO_CAP:
    CODE_ENDS:
    }

    // See the output and memory 
    printf("\nAssembly code starts at the memory location :: %p", ptr_codeStartsAt);
    int machineDbWord = 100;
    printf("\n%d double words (4 bytes) of machine code starting at location %p :: \n", machineDbWord, ptr_codeStartsAt);
    for (int i = 0; i <= machineDbWord; i++)printf("%X", *ptr_codeStartsAt++);
    printf("\nCapitalized letter :: %c", anyLetter);

    return 0;
}