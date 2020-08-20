all:
	$(CC) mini-rsyslogd.c -o mini-rsyslogd

clean:
	rm mini-rsyslogd

install: all
	./installer.sh install

uninstall:
	./installer.sh uninstall
