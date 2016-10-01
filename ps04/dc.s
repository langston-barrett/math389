# -*- mode: assembler -*-
# ASCII codes:
# 0: 48, 1: 49, 2: 50, 3: 51, 4: 52, 5: 53, 6: 54, 7: 55, 8:56, 9: 57
# p: 112, d: 100, x: 120, *: 42, +: 43, -: 45, /: 47, %: 37, ^: 94
#
# Registers [1]:
# rax: return values
# rcx: last input char
# rdx: custom stack base pointer
# r8:  custom stack stack pointer
# r9:  none
# r10: none
# r11: none

  .globl  dc
  .text
dc:                     # dc : Int -> IO Int
  pushq   %rbp          # push base pointer onto the stack
  movq    %rsp, %rbp    # push stack pointer up to base pointer
  jmp     loop

# The input loop. Gather characters, dispatch to other functions.
loop:
  callq   input_char    # switch (input) {
  movq    %rax, %rcx    # push the input on the stack
                        # // letters
  cmpq    $120, %rax    # case x:
  je      return0       #   we're done
  cmpq    $112, %rax    # case p:
  je      return0       #   we're also done
                        # // operators
  cmpq    $37, %rax     # case %:
  je      mod           #   mod
  cmpq    $42, %rax     # case *:
  je      multiply      #   multiply
  cmpq    $43, %rax     # case +:
  je      add           #   add
  cmpq    $45, %rax     # case -:
  je      subtract      #   subtract
  cmpq    $47, %rax     # case /:
  je      divide        #   divide
  cmpq    $94, %rax     # case ^:
  je      power         #   power }
                        # // other
  cmpq    $48, %rax     # case input >= 0:
  jge     maybe_int     #   might be an int
  jmp     return1       # if it was anything else, error

maybe_int:              # // int
  cmpq    $57, %rax     # is it <=57 ('9')?
  jle     int           # if so, it was an int
  jmp     return1       # if not, it's an error


int:
  jmp     loop

mod:
multiply:
add:
subtract:
divide:
power:
  jmp     loop

return0:
  movq    $0, %rdi      # return 0, we're returning normally
  callq   quit
  retq

return1:
  movq    %rcx, %rdi    # return 1, some error
  callq   invalid_char
  retq

# "The registers RAX, RCX, RDX, R8, R9, R10, R11 are considered volatile and
# must be considered destroyed on function calls (unless otherwise
# safety-provable by analysis such as whole program optimization)."
# https://msdn.microsoft.com/en-us/library/6t169e9c.aspx
