CC              = gcc
CFLAGS          = -Wall -std=c11
DBGFLAGS        = -g -ggdb -D_DEBUG
RLSFLAGS        = -mwindows 
SRC             = letters.c main.c res.o
MORE_WARNINGS   = -Wextra -pedantic
LINKER          = -lcomctl32 -lgdi32 -luxtheme -lgdiplus

dbg: res debug
	del res.o

rls: res release
	del res.o

debug: $(SRC)
	$(CC) $(CFLAGS) $(DBGFLAGS) -o debug.exe $^ $(LINKER)

release: $(SRC)
	$(CC) $(CFLAGS) $(RLSFLAGS) -o forKatelyn.exe $^ $(LINKER)

res:
	windres res.rc res.o

.PHONY:
clean:
	@del *.exe *.o
