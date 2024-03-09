CC		= g++
CFLAGS	= -g -Wall -Wextra -I.
LINKS	= -lncursesw
OBJDIR	= obj
DEPS	= screen.hpp widgetsManager.hpp widget.hpp \
		widgetClock.hpp widgetTextFile.hpp widgetClock.hpp widgetDialog.hpp widgetMsgbox.hpp widgetAppLauncher.hpp widgetFileExplorer.hpp \
		module.hpp moduleDialog.hpp moduleScroller.hpp moduleButton.hpp moduleOneLiner.hpp moduleANSI.hpp \
		renderer.hpp cursesAnsi.hpp globals.hpp
_OBJ	= main.o screen.o widgetsManager.o widget.o \
		widgetClock.o widgetTextFile.o widgetANSI.o widgetDialog.o widgetMsgbox.o widgetAppLauncher.o widgetFileExplorer.o \
		module.o moduleDialog.o moduleScroller.o moduleButton.o moduleOneLiner.o moduleANSI.o \
		renderer.o cursesAnsi.o globals.o
OBJ		= $(patsubst %,$(OBJDIR)/%,$(_OBJ))
APP		= term-temple

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(APP): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKS)

all: $(APP)

.PHONY: clean

clean:
	rm -f *.o
	rm -f ${APP}

re: clean
	$(MAKE) all
