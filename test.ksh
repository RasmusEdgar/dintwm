for i in $(./dintwm | grep -v -- -h | awk -F"-|:" '{ printf "%s ",$2}'); do
echo $i
./dintwm -"$i"
sleep 3
done
