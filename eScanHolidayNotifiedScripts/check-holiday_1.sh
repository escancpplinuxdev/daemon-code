
#!/bin/bash

USER_NAME="omkar"
USER_ID=1000


export DISPLAY=:0
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus

# path to your holiday list file
HOLIDAY_FILE="/tmpdata/Rushikesh_27022026/eScanHolidayNotifiedScripts/holidays.list"

# Get the current date in YYYY-MM-DD format
DAY=$(date +%Y-%m-%d)

# Check if today's date exists in the holiday file
if grep -Fxq "$DAY" "$HOLIDAY_FILE"; then
	# If it's a holiday, send a desktop notification
	notify-send -u critical "Holiday Reminder" \
		"Today ($DAY) is a company holiday. Take a leave."
else
	#Optional: log that it's a work day (for debugging)
	echo "$DAY is a regular work day."
	notify-send -u critical "Good morning" \
		"($DAY) is a regular work day."

fi
