NAME = mypas
DOTH=common.h symtab.h tree.h dag.h

SOURCE= common.h \
				error.c \
				main.c \
				symtab.c \
				symtab.h \
				type.c \
				x86rtl.asm \
				rule.c \
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
			  rule.o \
			  tree.o \
			  new.o \
			  list.o \
			  dag.o \
			  x86linux.o \
			  cf.o

MAKED = lex.yy.c rule.c rule.h

.SUFFIXES:.c

all: $(NAME)

dag.o: dag.c common.h
	gcc -g -Wall -c $<

new.o: new.c common.h
	gcc -g -Wall -c $<

%.o:%.c $(DOTH)
	gcc -g -Wall -c $<

lex.yy.c:spl.l rule.c
	flex spl.l

rule.c:spl.y
	bison -d -o rule.c $<

$(NAME):$(OBJS) $(DOTH)
	gcc -o $(NAME) -g -Wall $(OBJS)

clean:
	rm -f *.o
	rm -f $(NAME)
	rm -f $(MAKED)

