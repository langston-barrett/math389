# -*- mode: assembler -*-
# run `./asmfmt -w ./dc.s` to reformat this file.
#
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
	movq  %rdi, %rcx   # save fake stack pointer to rcx
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
  cmpq  $100, %rdi   # case d:
	je    call_decimal_construction
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

# Takes a pointer to a stack and the stack length, returns a
# new pointer and new stack length. The pure version of dc,
# for testing purposes.
.globl real_dc
real_dc:
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
	jl    retq         # return counter
	subq  %rsi, %rdi   # else dividend = dividend - divisor
	callq div_positive # recurse
	incq  %rax         # counter++
	jmp   retq         # return counter

div_negative:
	movq  %rdi, %rdx   # dividend' = dividend
	negq  %rdx         # dividend' = -dividend
	cmpq  %rsi, %rdx   # if (dividend' < divisor)
	jl    retq         # return counter
	addq  %rsi, %rdi   # else dividend = dividend + divisor
	callq div_negative # recurse
	decq  %rax         # counter--
	jmp   retq         # return counter

.globl power

power:
	jmp loop

############################# Decimal Construction #############################

# mutates rcx, the stack pointer
# call_decimal_construction : jump
call_decimal_construction:
	movq  %rcx, %rdi   # decimal_construction(stack_pointer)
	callq decimal_construction
	pushq %rax         # save return value
	movq  %rcx, %rdi   # stack_pointer = decimal_construction_pop(stack_pointer)
	callq decimal_construction_pop
	movq  %rax, %rcx   #
	jmp   loop

# rcx: custom stack stack pointer - points to top item in stack
# rax: return 1
# rdx: return 2
# rdi: arg 1
# rsi: arg 2
# Volatile registers : r8, r9, r10, r11

# decimal_construction : Ptr Int -> IO ()
.globl decimal_construction
decimal_construction:
  movq  %rdi, %r9    # stack_pointer = input_stack_pointer
	movq  (%r9), %r8	 # n = pop(stack_pointer)
  addq  $8, %r9
  decq  %r8          # n-- (to avoid off-by 1 error)
  movq  $0, %r11     # to_return = 0
decimal_construction_helper:
	cmpq  $0, %r8      # if n <= 0
	jl    return_r11   # then return to_return
	movq  (%r9), %rdi  #
  addq  $8, %r9      # arg1 = pop(tmp_stack)
	movq  %r8, %rsi    # to_add = decimal_shift(arg1, n)
	callq decimal_shift
	addq  %rax, %r11   # to_return += to_add
	decq  %r8          # n--
	loop  decimal_construction_helper

# return_r11 : jump
return_r11:
  movq  %r11, %rax
  retq

# Multiply %rdi by 10^%rsi
# decimal_shift : Int -> Int -> Int
.globl decimal_shift
decimal_shift:
  cmpq $0, %rsi     # if n <= 0
  jle  retq_rdi     # return x
	movq $0, %rax     # to_return = 0
  jmp  decimal_shift_loop

# x * 10 = (x << 3) + (x << 1)
decimal_shift_loop:
                    # do {
  movq %rdi, %rdx   # tmp = x
	salq $3, %rdx     # tmp = tmp << 3
  addq %rdx, %rax   # to_return += tmp

  movq %rdi, %rdx   # tmp = x
	salq $1, %rdx     # tmp = tmp << 1
	addq %rdx, %rax   # to_return += tmp

	decq %rsi         # n--
  cmpq $0, %rsi     # if n <= 0
  jle  retq         # then return to_return

  movq %rax, %rdi
  movq $0, %rax     #
	jmp  decimal_shift_loop # decimal_shift_loop(to_return, n)

# retq_rdi : jump
retq_rdi:
  movq %rdi, %rax
  retq

# Pop the first n values off the stack, where n is the first value on the stack.
# %rdi is the stack pointer
# returns new stack pointer
.globl decimal_construction_pop

decimal_construction_pop:
	retq

################################## Returning ##################################

# return from a function conditionally
# retq : jump
retq:
	retq

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
