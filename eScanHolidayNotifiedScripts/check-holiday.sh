#!/bin/bash

USER_NAME="omkar"
USER_ID=1000

export DISPLAY=:0
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/$USER_ID/bus

HOLIDAY_FILE="/tmpdata/Rushikesh_27022026/eScanHolidayNotifiedScripts/holidays.list"
DAY=$(date +%d-%m-%Y)
TDAY=$(date +"%d-%b-%Y %A")
FULLDAY=$(date -d "+1 days" +"%d-%b-%Y %A")

# Function to send notification 10 times
send_notification_loop1() {
    local TITLE="$1"
    local MESSAGE="$2"

    for i in {1..10}
    do
        sudo -u $USER_NAME DISPLAY=$DISPLAY DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS \
        notify-send -u normal "$TITLE" "$MESSAGE"

        sleep 10   # wait 10 seconds before next popup
    done
}

send_notification_loop2() {
    local TITLE="$1"
    local MESSAGE="$2"

    for i in {1..2}
    do
        sudo -u $USER_NAME DISPLAY=$DISPLAY DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS \
        notify-send -u normal "$TITLE" "$MESSAGE"

        sleep 10   # wait 10 seconds before next popup
    done
}

if grep -Fxq "$DAY" "$HOLIDAY_FILE"; then
    echo "Tomorrow $FULLDAY is a company holiday. Take a leave."

    send_notification_loop1 \
    "Holiday Reminder !!!!!" \
    "Tomorrow [$FULLDAY] is a company holiday. Take a leave."

else
    echo "$TDAY is a regular work day."

    send_notification_loop2 \
    "Good morning ....." \
    "Today [$TDAY] is a work day."
fi
