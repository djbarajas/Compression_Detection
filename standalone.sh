make clean

make standalone

for i in {1..2}
do
echo "Running with TTL=$i"
./standalone myconfig.json $i
sleep 10
done
