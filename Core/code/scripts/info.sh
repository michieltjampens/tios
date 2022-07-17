echo ""
echo "------------------------------"
echo "      UPTIME AND LOGINS       "
echo "------------------------------"
w # uptime information and who is logged in
echo ""
echo "Current date/time: $(date +"%d/%m/%Y %H:%M")"
echo ""
echo "------------------------------"
echo "         DISK USAGE           "
echo "------------------------------"
df -h -x tmpfs -x udev # disk usage, minus def and swap
echo ""
echo "------------------------------"
echo "           MEMORY             "
echo "------------------------------"
free
echo "------------------------------"