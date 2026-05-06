#!/bin/bash

TARGET_MAJOR=147
FLAGS="--ssl-version-min=tls1.2 --ssl-version-max=tls1.2 --disable-features=PostQuantumKyber"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

echo "Detecting Chromium/Brave installation type..."

# ----------------------------
# Detect Snap vs Native install
# ----------------------------
is_snap_app() {
    local bin=$1
    snap list 2>/dev/null | awk '{print $1}' | grep -qi "^$bin$"
}

get_desktop_dirs() {
    local bin=$1

    if is_snap_app "$bin"; then
        echo "/var/lib/snapd/desktop/applications"
    else
        echo "/usr/share/applications /usr/local/share/applications"
    fi
}

# ----------------------------
# Helpers (unchanged)
# ----------------------------
get_exec_binary() {
    grep -E "^Exec=" "$1" | head -n1 \
    | sed -E 's/^Exec=//' \
    | awk '{print $1}' \
    | sed 's|^.*/||'
}

get_version() {
    $1 --version 2>/dev/null \
    | grep -oE '[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+' \
    | head -n1
}

is_chromium_based() {
    echo "$1" | grep -qiE "chrome|chromium|brave|edge"
}

update_file() {
    local FILE=$1

    if [ ! -f "$FILE" ]; then
        return
    fi

    if grep -q "ssl-version-min" "$FILE"; then
        echo "Already updated → $FILE"
        return
    fi

    echo "Updating → $FILE"
    sudo cp "$FILE" "$FILE.bak_$TIMESTAMP"

    sudo sed -i '/^Exec=/ s|$| '"$FLAGS"'|' "$FILE"
}

# ----------------------------
# MAIN LOOP (improved)
# ----------------------------

ALL_DIRS=(
    "/usr/share/applications"
    "/usr/local/share/applications"
    "/var/lib/snapd/desktop/applications"
)

for dir in "${ALL_DIRS[@]}"; do
    for file in "$dir"/*.desktop; do

        [ -f "$file" ] || continue

        BIN=$(get_exec_binary "$file")
        [ -z "$BIN" ] && continue

        command -v "$BIN" &>/dev/null || continue

        VERSION_OUTPUT=$($BIN --version 2>/dev/null)

        if is_chromium_based "$VERSION_OUTPUT"; then

            VERSION=$(get_version "$BIN")
            MAJOR=$(echo "$VERSION" | cut -d. -f1)

            echo "--------------------------------------"
            echo "Desktop : $file"
            echo "Binary  : $BIN"
            echo "Version : $VERSION"

            if [ "$MAJOR" == "$TARGET_MAJOR" ]; then
                echo "→ MATCH (147). Applying flags..."

                update_file "$file"

                BASENAME=$(basename "$file")

                # Apply to user overrides only for native installs
                if ! is_snap_app "$BIN"; then
                    for user_home in /home/*; do
                        LOCAL_FILE="$user_home/.local/share/applications/$BASENAME"
                        update_file "$LOCAL_FILE"
                    done
                fi

            else
                echo "→ Skipped (not version 147)"
            fi
        fi

    done
done

echo "Done."
