#!/bin/bash

USER_NAME="omkar"
USER_ID=1000

export DISPLAY=:0
export DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/$USER_ID/bus"

# ------------------------------------------------------------
# Send a notification on:
#   - Every Saturday
#   - 2nd Friday (day 8-14)
#   - 4th Friday (day 22-28)
# ------------------------------------------------------------

# Get current day of week (1=Monday .. 7=Sunday) and day of month
DOW=$(date +%u)          # 1..7 (Monday=1, Sunday=7)
DOM=$(date +%d)          # day of month, with leading zero (01..31)
DOM=${DOM#0}             # remove leading zero (so 08 becomes 8)

# Determine if notification should be sent
SEND_NOTIFICATION=0

# Every Saturday (DOW = 7)
if [ "$DOW" -eq 7 ]; then
    SEND_NOTIFICATION=1
    TITLE="🎉 Weekend Alert"
    MESSAGE="Today is Saturday! Enjoy your weekend. Tomorrow is Holiday!!!"
fi

# 2nd Friday (DOW = 5 and DOM between 8 and 14)
if [ "$DOW" -eq 5 ] && [ "$DOM" -ge 8 ] && [ "$DOM" -le 14 ]; then
    SEND_NOTIFICATION=1
    TITLE="📅 Second Friday"
    MESSAGE="It's the 2nd Friday of the month. Tomorrow is Holiday!!!!"
fi

# 4th Friday (DOW = 5 and DOM between 22 and 28)
if [ "$DOW" -eq 5 ] && [ "$DOM" -ge 22 ] && [ "$DOM" -le 28 ]; then
    SEND_NOTIFICATION=1
    TITLE="📅 Fourth Friday"
    MESSAGE="It's the 4th Friday of the month. Tomorrow is Holiday!!!"
fi

# Send notification if condition met
if [ "$SEND_NOTIFICATION" -eq 1 ]; then
    # Use sudo to run as the target user, passing the required environment variables
    sudo -u "$USER_NAME" DISPLAY="$DISPLAY" DBUS_SESSION_BUS_ADDRESS="$DBUS_SESSION_BUS_ADDRESS" \
        notify-send -u normal "$TITLE" "$MESSAGE"
fi
