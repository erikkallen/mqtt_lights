#!/bin/bash

# Add systemd service
sudo su -c 'cat <<EOF >> /lib/systemd/system/mqtt_kaku.service
[Unit]
Description=MQTT KAKU
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/mqtt
# Next line is to run as a specific user
# for Raspberry Pi users, keep it at 'pi'
User=root

[Install]
WantedBy=multi-user.target
EOF'
