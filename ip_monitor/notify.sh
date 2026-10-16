#!/bin/bash

IP="$1"
STATUS="$2"
TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')
LOG_FILE="/tmp/ip_monitor_notify.log"

# ── Auto-detect logged-in desktop user (non-root) ────────────────────────────
USER_NAME=$(who | grep -v root | awk '{print $1}' | head -1)

# Fallback: get from loginctl
if [ -z "$USER_NAME" ]; then
    USER_NAME=$(loginctl list-sessions --no-legend | awk '{print $3}' | grep -v root | head -1)
fi

USER_ID=$(id -u "$USER_NAME" 2>/dev/null)

# ── Auto-detect DISPLAY ───────────────────────────────────────────────────────
DISPLAY_VAL=$(sudo -u "$USER_NAME" env | grep "^DISPLAY=" | cut -d= -f2)
if [ -z "$DISPLAY_VAL" ]; then
    DISPLAY_VAL=":0"   # fallback
fi

# ── Auto-detect DBUS_SESSION_BUS_ADDRESS ─────────────────────────────────────
DBUS_ADDR=$(cat /proc/$(pgrep -u "$USER_NAME" gnome-session 2>/dev/null | head -1)/environ \
            2>/dev/null | tr '\0' '\n' | grep "^DBUS_SESSION_BUS_ADDRESS=" | cut -d= -f2-)

# Fallback via machine-id based socket path
if [ -z "$DBUS_ADDR" ]; then
    DBUS_ADDR="unix:path=/run/user/${USER_ID}/bus"
fi

# ── Log always (works even without desktop) ───────────────────────────────────
echo "[$TIMESTAMP] $STATUS : $IP" | tee -a "$LOG_FILE"
logger -t ip_monitor "[$STATUS] $IP"

echo "    USER    = $USER_NAME"
echo "    DISPLAY = $DISPLAY_VAL"
echo "    DBUS    = $DBUS_ADDR"

# ── Send notification (2 popups, 15 sec apart) ───────────────────────────────
send_notification_loop2()
{
    local TITLE="$1"
    local MESSAGE="$2"

    for i in {1..2}
    do
        sudo -u "$USER_NAME" \
            DISPLAY="$DISPLAY_VAL" \
            DBUS_SESSION_BUS_ADDRESS="$DBUS_ADDR" \
            notify-send -u critical "$TITLE" "$MESSAGE"

        echo "[$TIMESTAMP] notify-send fired (#$i) → $TITLE : $MESSAGE"

        if [ "$i" -lt 2 ]; then
            sleep 15
        fi
    done
}

# ── Call with title + message based on status ─────────────────────────────────
if [ "$STATUS" = "DOWN" ]; then
    send_notification_loop2 \
        "Connection Lost" \
        "IP $IP is unreachable  [$TIMESTAMP]" &

elif [ "$STATUS" = "UP" ]; then
    send_notification_loop2 \
        "Connection Restored" \
        "IP $IP is reachable again  [$TIMESTAMP]" &
fi

