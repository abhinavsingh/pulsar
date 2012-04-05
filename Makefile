.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: clean

CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -levent -pthread
SRCDIR = src
OBJDIR = obj
SRCS = $(addprefix $(SRCDIR)/, worker.c server.c pulsar.c)
OBJS = $(addprefix $(OBJDIR)/, worker.o server.o pulsar.o)
EXE = pulsar

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) $(EXE)