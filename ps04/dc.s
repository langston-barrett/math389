# -*- mode: assembler -*-
# ASCII codes:
# 0: 48, 1: 49, 2: 50, 3: 51, 4: 52, 5: 53, 6: 54, 7: 55, 8:56, 9: 57
# p: 112, d: 100, x: 120, *: 42, +: 43, -: 45, /: 47, %: 37, ^: 94
#
# Globally preserved registers (within this file, at least) [1]:
# rcx: the most recently input character
# r8:  custom stack stack pointer - points to top item in stack
# Registers with defined usage:
# rax: return 1
# rdx: return 2
# rdi: arg 1
# rsi: arg 2
# Volatile registers: r9, r10, r11

.globl  dc
.text
dc:                     # dc : Int -> IO Int
  pushq   %rbp          # push base pointer onto the stack
  movq    %rsp, %rbp    # push stack pointer up to base pointer
  movq    %rdi, %r8     # save fake stack pointer to r8
  jmp     loop

# The input loop. Gather characters, dispatch to other functions.
loop:
  pushq   %r8           # save fake stack pointer to stack
  callq   input_char    # switch (input) {
  popq    %r8           # // restore fake stack pointer from stack
  movq    %rax, %rcx    # // save the input to rcx
                        # // letters
  cmpq    $120, %rcx    # case x:
  je      return0       #   we're done
  cmpq    $112, %rcx    # case p:
  je      print_top     #   print the top of the stack
                        # // operators
  cmpq    $37, %rcx     # case %:
  je      call_mod      #   mod
  cmpq    $42, %rcx     # case *:
  je      multiply      #   multiply
  cmpq    $43, %rcx     # case +:
  je      add           #   add
  cmpq    $45, %rcx     # case -:
  je      subtract      #   subtract
  cmpq    $47, %rcx     # case /:
  je      call_divide   #   divide
  cmpq    $94, %rcx     # case ^:
  je      power         #   power }
                        # // other
  cmpq    $48, %rcx     # case input < '0':
  jl      return1
  cmpq    $57, %rcx     # case input > '9':
  jg      return1
                        # it was an int!
  subq    $48,  %rcx    # subtract char '0' to get integer value
  movq    %rcx, %rdi    # give input as an integer to push to stack
  callq   push
  jmp     loop

#################################### Stack ####################################

# push an int to the top of the stack
# mutates stack pointer r8
push: # function
  addq    $8,   %r8     # add 8 bytes to the stack pointer
  movq    %rdi, (%r8)   # push input to the top of the stack
  retq

# pop an int from the top of the stack, put it in %rax
# mutates stack pointer r8
pop:  # function
  movq    (%r8), %rax   # return the top of the stack
  subq    $8,    %r8    # subtract 8 bytes from the stack pointer
  retq

# pop two ints, put them in %rax, %rdx
# mutates stack pointer r8 twice
pop2:  # function
  callq   pop           # m = pop()
  movq    %rax, %rdi    # save n
  callq   pop           # n = pop()
  movq    %rax, %rdx    # 2nd return value = n
  movq    %rdi, %rax    # 1st return value = m
  retq

############################### Simple Arithmetic ###############################


add: # jump
  callq   pop2       # m, n = pop(), pop()
  addq    %rdx, %rax # n = m + n
  movq    %rax, %rdi # arg 1 = n
  callq   push       # push n
  jmp     loop

subtract: # jump
  callq   pop2       # m, n = pop(), pop()
  subq    %rax, %rdx # n = n - m
  movq    %rdx, %rdi # arg 1 = n
  callq   push       # push m
  jmp     loop

multiply: # jump
  callq   pop2       # m, n = pop(), pop()
  movq    %rax, %rdi # arg 1 = m
  movq    %rdx, %rsi # arg 2 = n
  pushq   %r8        # save registers
  callq   mul        # result = m * n
  popq    %r8        # restore registers
  movq    %rax, %rdi # arg 1 = result
  callq   push       # push result
  jmp     loop

############################## Complex Arithmetic ##############################

call_mod: # jump
  callq   pop2         # m, n = pop(), pop()
  movq    %rax, %rdi   # arg1 = m
  movq    %rdx, %rsi   # arg2 = n
  callq   mod          # result = mod
  movq    %rax, %rdi   # arg 1 = result
  callq   push         # push result to top of the stack
  jmp     loop         # 'return'

.globl  mod
mod: # function
  cmpq    $0, %rdi     # if (n < 0)
  jge     mod_positive # then dispatch to positive version
  jmp     mod_negative # else dispatch to negative version

# mod two numbers where the dividend > 0
mod_positive:
  cmpq    %rsi, %rdi   # if (dividend < divisor) then return dividend
  jl      mod_ret_positive
  subq    %rsi, %rdi   # else dividend - divisor
  jmp     mod_positive # loop

# mod two numbers where the dividend < 0
mod_negative:
  movq    %rdi, %rax   # copy the divident
  negq    %rax         # negate it
  cmpq    %rsi, %rax   # if (-dividend < divisor) then return dividend
  jl      mod_ret_negative
  addq    %rsi, %rdi   # else dividend + divisor
  jmp     mod_negative # loop

mod_ret_positive:
  movq    %rdi, %rax   # return dividend
  retq
mod_ret_negative:
  movq    %rdi, %rax   # return -dividend
  negq    %rax
  retq

call_divide: # jump
  jmp     loop

.globl  divide
divide: # jump
  jmp     loop

.globl  power
power: # jump
  jmp     loop

################################## Returning ##################################

# Print the top of the stack, saving the important registers
print_top: # jump
  pushq   %r8             # save registers
  movq    (%r8), %rdi     # give top of stack as arg
  callq   output_int64    # print top of stack
  popq    %r8
  jmp     loop

# Exit normally
return0: # jump
  movq    %rbp, %rsp    # restore old stack pointer
  popq    %rbp          # restore old base pointer
  movq    $0, %rax      # return 0 to caller
  retq

# Error: invalid input character
return1: # jump
  movq    %rbp, %rsp    # restore old stack pointer
  popq    %rbp          # restore old base pointer
  movq    %rcx, %rax    # return last character input to caller
  retq

# "The registers RAX, RCX, RDX, R8, R9, R10, R11 are considered volatile and
# must be considered destroyed on function calls (unless otherwise
# safety-provable by analysis such as whole program optimization)."
# https://msdn.microsoft.com/en-us/library/6t169e9c.aspx
