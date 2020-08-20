#!/bin/sh

[ "$UID" -ne "0" ] && {
	echo "has to be run as root"
	exit 1
}

binfile="/usr/bin/mini-rsyslogd"
service_name="mini-rsyslogd.service"

install(){
	cp mini-rsyslogd "$binfile"
	cp systemd.service "/etc/systemd/system/$service_name"
	systemctl daemon-reload
	
	cat<<EOF
----------------------------------------------------------------
the systemd service file ist the config, there are only two args
            /etc/systemd/system/mini-rsyslogd.service           
            sudo systemctl start mini-rsyslogd.service           
            sudo systemctl enable mini-rsyslogd.service           
----------------------------------------------------------------
EOF
}

uninstall(){
	rm "$binfile"
	systemctl is-enabled "$service_name" && systemctl disable "$service_name"
	systemctl is-active "$service_name" && systemctl stop "$service_name"
	rm "/etc/systemd/system/$service_name"
	systemctl daemon-reload
}


mode="install"
[ -n "$1" ] && mode="$1"

"$mode"
