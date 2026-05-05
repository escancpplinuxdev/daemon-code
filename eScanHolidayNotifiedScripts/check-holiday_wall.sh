
#!/bin/bash
export DISPLAY=:0
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus

# path to your holiday list file
HOLIDAY_FILE="/tmpdata/Rushikesh_27022026/eScanHolidayNotifiedScripts/holidays.list"

# Get the current date in YYYY-MM-DD format
DAY=$(date +%Y-%m-%d)


if grep -Fxq "$DAY" "$HOLIDAY_FILE"; then
    echo "Holiday Reminder: Today ($DAY) is a company holiday. Take a leave." | wall
else
    echo "Good morning: Today ($DAY) is a regular work day." | wall
fi

