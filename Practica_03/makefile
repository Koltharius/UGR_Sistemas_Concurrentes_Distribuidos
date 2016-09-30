CC = mpicxx
CFLAGS = -g
COMPILE = $(CC) $(CFLAGS)
EXECFILES := filosofos filosofoscamarero productorconsumidor

all: $(EXECFILES)
%: %.cpp
	$(COMPILE) -o $@ $<

clean:
	rm $(EXECFILES)
