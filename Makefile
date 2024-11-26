CC = g++
CFLAGS = -Wall -pipe -O3 -I./include
LDFLAGS = 

# Targets principais
TARGET_INIT = init
TARGET_HALT = halt
TARGET_REBOOT = reboot
TARGET_SUSPEND = suspend

all: $(TARGET_INIT) $(TARGET_HALT) $(TARGET_REBOOT) $(TARGET_SUSPEND)

# Regra para compilar o init
$(TARGET_INIT): init.cpp
	$(CC) $(CFLAGS) -o $(TARGET_INIT) init.cpp $(LDFLAGS)

# Regra para compilar halt
$(TARGET_HALT): halt.cpp
	$(CC) $(CFLAGS) -o $(TARGET_HALT) halt.cpp $(LDFLAGS)

# Regra para compilar reboot
$(TARGET_REBOOT): reboot.cpp
	$(CC) $(CFLAGS) -o $(TARGET_REBOOT) reboot.cpp $(LDFLAGS)

# Regra para compilar suspend
$(TARGET_SUSPEND): suspend.cpp
	$(CC) $(CFLAGS) -o $(TARGET_SUSPEND) suspend.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET_INIT) $(TARGET_HALT) $(TARGET_REBOOT) $(TARGET_SUSPEND)

install:
	mv -v {init,halt,suspend,reboot} /usr/sbin

remove:
	rm -fr /usr/sbin/{init,halt,suspend,reboot}
