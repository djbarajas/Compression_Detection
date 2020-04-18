make clean

make standalone

for i in {1..255}
do
echo "Running with TTL=$i"
sudo ./standalone myconfig.json $i
sleep 20
done

echo "Finished running with incrementing TTL"
