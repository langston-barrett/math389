# -*- mode: assembler -*-
# ASCII codes:
# 0: 48, 1: 49, 2: 50, 3: 51, 4: 52, 5: 53, 6: 54, 7: 55, 8: 56, 9: 57
# p: 112, d: 100, x: 120, *: 42, +: 43, -: 45, /: 47, %: 37, ^: 94
#
# Globally preserved registers (within this file, at least) [1]:
# rcx: custom stack stack pointer - points to top item in stack
# Registers with defined usage:
# rax: return 1
# rdx: return 2
# rdi: arg 1
# rsi: arg 2
# Volatile registers : r8, r9, r10, r11

.globl dc
.text

# dc : Int -> IO Int
dc:
	pushq %rbp         # push base pointer onto the stack
	movq  %rsp, %rbp   # push stack pointer up to base pointer
	movq  %rdi, %rcx   # save fake stack pointer to rcx jmp loop

# The input loop. Gather characters from stdin.
# loop : jump
loop:
	pushq %rcx         # save fake stack pointer to stack
	callq input_char   # rax = input()
	popq  %rcx         # restore fake stack pointer from stack
	movq  %rax, %rdi   # arg1 = rax
	callq dispatch     # dispatch(arg1)
	jmp   loop

# Dispatches to other functions depending on the input.
# dispatch : Char -> IO ()
dispatch:
	cmpq  $120, %rdi   # case x:
	je    return0      #   we're done
	cmpq  $112, %rdi   # case p:
	je    print_top    #   print the top of the stack
	cmpq  $37, %rdi    # case %:
	je    call_mod     #   mod
	cmpq  $42, %rdi    # case *:
	je    multiply     #   multiply
	cmpq  $43, %rdi    # case +:
	je    add          #   add
	cmpq  $45, %rdi    # case -:
	je    subtract     #   subtract
	cmpq  $47, %rdi    # case /:
	je    call_divide  #   divide
	cmpq  $94, %rdi    # case ^:
	je    power        #   power
	cmpq  $48, %rdi    # case input < '0':
	jl    return1      #   invalid input!
	cmpq  $57, %rdi    # case input > '9':
	jg    return1      #   invalid input!
	subq  $48, %rdi    # subtract char '0' to get integer value
	callq push         #
	retq

#################################### Stack ####################################

# push an int to the top of the stack
# mutates stack pointer rcx
# push : Int -> IO ()
push:
	addq $8, %rcx      # add 8 bytes to the stack pointer
	movq %rdi, (%rcx)  # push input to the top of the stack
	retq

# pop an int from the top of the stack, put it in %rax
# mutates stack pointer rcx
# pop : () -> IO Int
pop:
	movq (%rcx), %rax  # return the top of the stack
	subq $8, %rcx      # subtract 8 bytes from the stack pointer
	retq

# pop two ints, put them in %rax, %rdx
# mutates stack pointer rcx twice
# pop2 : () -> IO (Int, Int)
pop2:
	callq pop          # m = pop()
	movq  %rax, %rdi   # save n
	callq pop          # n = pop()
	movq  %rax, %rdx   # 2nd return value = n
	movq  %rdi, %rax   # 1st return value = m
	retq

############################### Simple Arithmetic ###############################

# add : jump
add:
	callq pop2         # m, n = pop(), pop()
	addq  %rdx, %rax   # n = m + n
	movq  %rax, %rdi   # arg 1 = n
	callq push         # push n
	jmp   loop

# subtract : jump
subtract:
	callq pop2         # m, n = pop(), pop()
	subq  %rax, %rdx   # n = n - m
	movq  %rdx, %rdi   # arg 1 = n
	callq push         # push m
	jmp   loop

# multiply : jump
multiply:
	callq pop2         # m, n = pop(), pop()
	movq  %rax, %rdi   # arg 1 = m
	movq  %rdx, %rsi   # arg 2 = n
	pushq %rcx         # save registers
	callq mul          # result = m * n
	popq  %rcx         # restore registers
	movq  %rax, %rdi   # arg 1 = result
	callq push         # push result
	jmp   loop

############################## Complex Arithmetic ##############################

# call_mod : jump
call_mod:
	callq pop2         # m, n = pop(), pop()
	movq  %rax, %rsi   # arg1 = n
	movq  %rdx, %rdi   # arg2 = m
	callq mod          # result = mod(arg1, arg2)
	movq  %rax, %rdi   # arg 1 = result
	callq push         # push result to top of the stack
	jmp   loop         # 'return'

# mod : Int -> Int -> Int
.globl mod

mod:
	cmpq $0, %rdi      # if (n < 0)
	jge  mod_positive  # then dispatch to positive version
	jmp  mod_negative  # else dispatch to negative version

# mod two numbers where the dividend > 0
# mod_positive : internal jump
mod_positive:
	cmpq %rsi, %rdi    # if (dividend < divisor) then return dividend
	jl   mod_ret_positive
	subq %rsi, %rdi    # else dividend - divisor
	jmp  mod_positive  # loop

# mod two numbers where the dividend < 0
# mod_negative : internal jump
mod_negative:
	movq %rdi, %rax    # copy the dividend
	negq %rax          # negate it
	cmpq %rsi, %rax    # if (-dividend < divisor) then return dividend
	jl   mod_ret_negative
	addq %rsi, %rdi    # else dividend + divisor
	jmp  mod_negative  # loop

# mod_ret_* : internal jump
mod_ret_positive:
	movq %rdi, %rax    # return dividend
	retq

mod_ret_negative:
	movq %rdi, %rax    # return -dividend
	negq %rax
	retq

# call_divide : jump
call_divide:
	callq pop2         # m, n = pop(), pop()
	movq  %rdx, %rdi   # dividend = n
	movq  %rax, %rsi   # divisor = m
	callq divide       # result = divide(dividend, divisor)
	movq  %rax, %rdi   # arg 1 = result
	callq push         # push result to top of the stack
	jmp   loop         # 'return'

# divide : Int -> Int -> Int
.globl divide

divide:
	movq $0, %rax      # zero out the counter
	cmpq $0, %rdi      # if (dividend >= 0)
	jge  div_positive  # then dispatch to positive version
	jmp  div_negative  # else dispatch to negative version

# div_* : internal jump
div_positive:
	cmpq  %rsi, %rdi   # if (dividend < divisor)
	jl    div_ret      # return counter
	subq  %rsi, %rdi   # else dividend = dividend - divisor
	callq div_positive # recurse
	incq  %rax         # counter++
	jmp   div_ret      # return counter

div_negative:
	movq  %rdi, %rdx   # dividend' = dividend
	negq  %rdx         # dividend' = -dividend
	cmpq  %rsi, %rdx   # if (dividend' < divisor)
	jl    div_ret      # return counter
	addq  %rsi, %rdi   # else dividend = dividend + divisor
	callq div_negative # recurse
	decq  %rax         # counter--
	jmp   div_ret      # return counter

div_ret:
	retq

	.globl power

power:
	jmp loop

############################# Decimal Construction #############################

# mutates rcx, the stack pointer
.globl decimal_construction

decimal_construction:
	jmp loop

################################## Returning ##################################

# Print the top of the stack, saving the important registers
# print_top : jump
print_top:
	pushq %rcx         # save registers
	movq  (%rcx), %rdi # give top of stack as arg
	callq output_int64 # print top of stack
	popq  %rcx
	jmp   loop

# Exit normally
# return0 : jump
return0:
	movq %rbp, %rsp    # restore old stack pointer
	popq %rbp          # restore old base pointer
	movq $0, %rax      # return 0 to caller
	retq

# Error: invalid input character
# return1 : jump
return1:
	movq %rbp, %rsp    # restore old stack pointer
	popq %rbp          # restore old base pointer
	movq %rdi, %rax    # return last character input to caller
	retq

# "The registers RAX, RCX, RDX, R8, R9, R10, R11 are considered volatile and
# must be considered destroyed on function calls(unless otherwise
# safety - provable by analysis such as whole program optimization)."
# https: // msdn.microsoft.com/en-us/library/6t169e9c.aspx
