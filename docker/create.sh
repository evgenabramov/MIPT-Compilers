#!/usr/bin/env bash

if [[ $EUID == 0 ]]; then
   echo "This script must be run as non-root user inside docker group"
   exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
mount_path=$( echo "$DIR/../")
echo "Mounting from $mount_path"

sed "s/<mount_path>/${mount_path//\//\\/}/" docker-compose.yml.j2 > docker-compose.yml

export CONT_UID=$(id -u)
export CONT_GID=$(id -g)

docker-compose -f docker-compose.yml up -d --build --force-recreate
docker exec -it compilers-image groupadd -g $(id -g) grp
docker exec -it compilers-image useradd -u $(id -u) -g $(id -g) -m $USER
docker exec -it compilers-image chown -R $USER /compilers
