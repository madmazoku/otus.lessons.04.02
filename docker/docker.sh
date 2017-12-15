echo ========= Build docker image
docker build -t otus.lessons.04.02 .
echo ========= Check ip_filter version
docker run --rm -i otus.lessons.04.02 ip_filter -v
echo ========= Filter IPs, result will go to ../bin/out.txt
cat ../src/ip_filter.tsv | docker run --rm -i otus.lessons.04.02 ip_filter > ../bin/out.txt
echo ========= Remove docker image
docker rmi otus.lessons.04.02