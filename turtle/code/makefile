CC=gcc
COMMON= `sdl2-config --cflags` -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = `sdl2-config --libs` -lm

pass1 = ../data/simp_c.ttl
pass2 = ../data/simp.ttl
pass3 = ../data/spiral1_c.ttl
pass4 = ../data/spiral2_c.ttl
pass5 = ../data/3loop.ttl
pass6 = ../data/fromVarToVar.ttl
warn1 = ../data/redefinedVariable.ttl
warn2 = ../data/wrongnotation1.ttl
error1 = ../data/wrongbrace.ttl
error2 = ../data/wrongnotation2.ttl
error3 = ../data/undefinedVariable.ttl
color = ../data/color.ttl
gif = ../data/gif.ttl
image = ../data/image.ttl

test : test.c test_interp.h test_interp.c neillsdl2.h neillsdl2.c parser.h parser.c stack.h stack.c general.h general.c stringlist.h stringlist.c
	$(CC) test.c test_interp.c stack.c general.c neillsdl2.c stringlist.c parser.c -o test -I./code $(PRODUCTION) $(LDLIBS)

parse : mainparser.c parser.c parser.h general.h general.c stack.h stack.c stringlist.h stringlist.c
	$(CC) mainparser.c parser.c general.c stack.c stringlist.c -o parse -I./code $(PRODUCTION) $(LDLIBS)

interp : maininterp.c interp.h interp.c neillsdl2.h neillsdl2.c stack.h stack.c general.h general.c stringlist.h stringlist.c
	$(CC) maininterp.c interp.c stack.c neillsdl2.c general.c stringlist.c -o interp -I./code $(PRODUCTION) $(LDLIBS)

extension : extension.h extension.c neillsdl2.h neillsdl2.c stack.h stack.c general.h general.c stringlist.h stringlist.c
	$(CC) extension.c stack.c neillsdl2.c general.c stringlist.c -o extension -I./code $(PRODUCTION) $(LDLIBS)

test_f : test.c test_interp.h test_interp.c neillsdl2.h neillsdl2.c parser.h parser.c stack.h stack.c general.h general.c stringlist.h stringlist.c
	$(CC) test.c test_interp.c stack.c general.c neillsdl2.c stringlist.c parser.c -o test_f -I./code $(PRODUCTION) $(LDLIBS) $(SANITIZE)

parse_f : mainparser.c parser.c parser.h general.h general.c stack.h stack.c stringlist.h stringlist.c
	$(CC) mainparser.c parser.c general.c stack.c stringlist.c -o parse_f -I./code $(PRODUCTION) $(LDLIBS) $(SANITIZE)

interp_f : maininterp.c interp.h interp.c neillsdl2.h neillsdl2.c stack.h stack.c general.h general.c stringlist.h stringlist.c
	$(CC) maininterp.c interp.c stack.c neillsdl2.c general.c stringlist.c -o interp_f -I./code $(PRODUCTION) $(LDLIBS) $(SANITIZE)

clean :
	rm -f test interp parse test_f interp_f parse_f extension

parseall:
	./parse $(pass1)
	./parse $(pass2)
	./parse $(pass3)
	./parse $(pass4)
	./parse $(pass5)
	./parse $(pass6)
	./parse $(warn1)
	./parse $(warn2)

parseerror1:
	./parse $(error1)

parseerror2:
	./parse $(error2)

parseerror3:
	./parse $(error3)

interpall:
	./interp $(pass1)
	./interp $(pass2)
	./interp $(pass3)
	./interp $(pass4)
	./interp $(pass5)
	./interp $(pass6)
	./interp $(warn1)

extensionall:
	./extension $(pass1)
	./extension $(pass2)
	./extension $(pass3)
	./extension $(pass4)
	./extension $(pass5)
	./extension $(pass6)

color1:
	./extension $(pass3)

color2:
	./extension $(color)

gif:
	./extension $(gif)

image:
	./extension $(image)
