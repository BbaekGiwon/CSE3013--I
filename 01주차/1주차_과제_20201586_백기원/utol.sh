#1/bin/bash

#디렉토리 입력받기
echo "working directory:"
read -r dir
#만약 디렉토리 입력이 없으면 현재 디렉토리 상에서 수행
if [ -z "$dir" ]
then
        dir="$(pwd)"
fi

#디렉토리가 존재하지 않을때
if [ ! -d "$dir" ]
then
        echo "error message: directory doesnt't exist"
        exit 0
fi

#디렉토리가 존재하고  change directory 할 수 있을 때
if cd "$dir"
then
        for i in $dir/*
        do
                temp="$(echo "$i" | sed "s#$dir/##g")"
                temp="$(echo "$temp" | tr '[A-Z][a-z]', '[a-z][A-Z]')"
                temp="$dir/$temp"
                mv "$i" "$temp"
        done
#디렉토리가 존재하나 change directory 할 수 없을 때
else
        echo "error message: can't change director"
fi
