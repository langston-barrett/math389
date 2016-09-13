#ifndef _decimal_sum_h
#define _decimal_sum_h

int char_to_int(char c);
char int_to_char(int i);

int add_chars(char c1, char c2);
int sub_chars(char c1, char c2);

char *remove_leading_zero(char* s);

char *sum(char *src1, char *src2);
char *difference(char *src1, char *src2);

#endif
