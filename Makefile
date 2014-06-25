NAME = mypas
DOTH=common.h symtab.h tree.h dag.h

SOURCE= common.h \
				error.c \
				main.c \
				symtab.c \
				symtab.h \
				type.c \
				x86rtl.asm \
				yacc.c \
				lex.yy.c \
				new.c \
				tree.c \
				tree.h \
				list.c \
				dag.h \
				dag.c \
				x86linux.c \
				cf.c

OBJS=error.o \
			  lex.yy.o \
			  main.o \
			  symtab.o \
			  type.o \
			  yacc.o \
			  tree.o \
			  new.o \
			  list.o \
			  dag.o \
			  x86linux.o \
			  cf.o

MAKED = lex.yy.c yacc.c yacc.h

.SUFFIXES:.c

all: $(NAME)

dag.o: dag.c common.h
	gcc -g -Wall -c $<

new.o: new.c common.h
	gcc -g -Wall -c $<

%.o:%.c $(DOTH)
	gcc -g -Wall -c $<

lex.yy.c:lex.l yacc.c
	flex lex.l

yacc.c:yacc.y
	bison -d -o yacc.c $<

$(NAME):$(OBJS) $(DOTH)
	gcc -o $(NAME) -g -Wall $(OBJS)

clean:
	rm -f *.o
	rm -f $(NAME)
	rm -f $(MAKED)

