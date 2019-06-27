for i in $(./dintwm | grep -v -- -h | grep -v -- "<" | awk -F"-|:" '{ printf "%s ",$2}'); do
echo $i
./dintwm -"$i"
sleep 3
done
for i in $(./dintwm | grep -v -- -h | grep -v -- "<" | awk -F"-|:" '{ printf "%s ",$2}'); do
echo $i
./dintwm -"$i" -b
sleep 3
done
for i in $(./dintwm | grep -v -- -h | grep -v -- "<" | awk -F"-|:" '{ printf "%s ",$2}'); do
echo $i
./dintwm -"$i" -B 250
sleep 3
done
