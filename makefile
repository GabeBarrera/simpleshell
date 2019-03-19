CC = gcc
CFLAGS = -g
LDFLAGS = -lreadline
OBJFILES = tokenizer.o command.o
TARGET = mush

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)