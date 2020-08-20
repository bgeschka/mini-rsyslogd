# mini-rsyslogd
Very small socket relay to syslog with systemd service - this only supports TCP

 - sudo make install
 - systemctl enable mini-rsyslogd.service

## The defaults are listen on port 514 on any interface 0.0.0.0
you can change that in the systemd service file after intsalling at:
 - /etc/systemd/system/mini-rsyslogd.service
