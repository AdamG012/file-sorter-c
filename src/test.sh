num=0
correct=0
fileNames=("aalpha" "balpha" "anum" "bnum")
argumentOptionsOrder=("-a -alpha" "-b -alpha" "-a -num" "-b -num")
for f in ../test/*.txt
do
	echo "Test $f"

	for i in ${!fileNames[@]}
	do
		DIFF=$(./fsort $f ${argumentOptionsOrder[$i]} | diff - "$f.${fileNames[$i]}.out")
		if [ "$DIFF" != "" ]
		then
			echo "INVALID"
		else
			echo "VALID"
			let correct=$correct+1
		fi
		let num=$num+1
	done
done	

echo ""
echo "Score $correct/$num"
