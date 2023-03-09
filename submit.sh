STUID=
STUNAME=
submit_url=https://course1.istratus.cn/api/pa/submit
upload_url=https://course1.istratus.cn/api/pa/upload
[ -z ${STUID} ]   && echo "STUID must be set (RTFM)"   && exit
[ -z ${STUNAME} ] && echo "STUNAME must be set (RTFM)" && exit
[  "${STUID}" = "181220000" ]   && echo "STUID must be set to Your Student ID"   && exit
[  "${STUNAME}" = "张三" ] && echo "STUNAME must be set to Your Name" && exit

echo "The task to be submitted > "
while [ 1 ]; do
select task in PA1 PA2 PA3 PA4 PA5 Report;
do
	break
done
[ -z ${task} ] || break
done
if [ "${task}" = "Report" ]; then
	echo "Report pdf file path(Current dir: ${PWD}): "
	read -e f_path
	[ ! -f "${f_path}"  ] && echo "${f_path} doesn't exist!" && exit
	curl -F "task=report" -F "id=${STUID}" -F "submission=@${f_path}" ${upload_url}
	[ $? -ne 0 ] && echo "Upload failed!" && exit
	echo ""
	echo "==Done!"
	exit
fi
echo "Input your submission remark: ";
read remark
branch=`git branch | grep '*' |  awk '{print $2}'`
echo "==Pushing current branch: ${branch} to remote repo..."
git push hustpa ${branch}:${branch} --force
[ $? -ne 0 ] && echo "Git push failed" && exit

commitid=`cat .git/refs/heads/${branch}`
echo "==Submitting ${task}..."
curl -s -d "[\"${STUID}\", \"${STUNAME}\", \"${commitid}\", \"${task}\", \"${branch}\", \"${remark}\"]" ${submit_url}
[ $? -ne 0 ] && echo "Network api failed!" && exit
echo ""
echo "==Done!"
