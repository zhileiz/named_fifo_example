CC=gcc
MASTER_FLAGS=-I. -D MASTER=true
SLAVE_FLAGS=-I. -D SLAVE=false

master: named_fifo.c hellofunc.c hellofunc.h
	$(CC) $(MASTER_FLAGS) -o master named_fifo.c hellofunc.c hellofunc.h

slave: named_fifo.c hellofunc.c hellofunc.h
	$(CC) $(SLAVE_FLAGS) -o slave named_fifo.c hellofunc.c hellofunc.h

clean:
	rm master slave