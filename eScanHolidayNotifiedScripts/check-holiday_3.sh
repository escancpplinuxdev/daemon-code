#!/bin/bash

USER_NAME="omkar"
USER_ID=1000

export DISPLAY=:0
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/$USER_ID/bus

HOLIDAY_FILE="/tmpdata/Rushikesh_27022026/eScanHolidayNotifiedScripts/holidays.list"
DAY=$(date +%Y-%m-%d)

if grep -Fxq "$DAY" "$HOLIDAY_FILE"; then
    echo "$DAY is a company holiday. Take a leave."

    sudo -u $USER_NAME DISPLAY=$DISPLAY DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS \
    notify-send -u normal -t 500000 "Holiday Reminder" \
    "Tomorrow  [$DAY]  is a company holiday. Take a leave."

else
    echo "$DAY is a regular work day."

    sudo -u $USER_NAME DISPLAY=$DISPLAY DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS \
    notify-send -u normal -t 500000 "Good morning ....." \
    "Today  [$DAY]  is a regular work day."
fi
