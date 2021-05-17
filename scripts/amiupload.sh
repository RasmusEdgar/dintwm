#!/bin/bash

set -e -o pipefail -o nounset

while getopts ":e:r:v:" opt; do
  case $opt in
    e)
      email="$OPTARG" >&2
      ;;
    r)
      release="$OPTARG" >&2
      ;;
   \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
    *)
      echo "requires an option." >&2
      exit 1
      ;;
  esac
done

if [ $OPTIND -eq 1 ]; then
  echo "No options passed. Quitting."
  exit 1
fi

emailu=$(echo "$email" | sed 's/@/ /g;s/\./ /g')

mkdir -p toupload
pushd toupload

cp ../../dintwm.readme .
sed -i "s/EMAIL/$email/g;s/OBFE/$emailu/g;s/VERSION/$release/g" dintwm.readme

wget --quiet https://github.com/RasmusEdgar/dintwm/releases/download/v"$release"/dintwm.lha

ftp -inv ftp.aminet.net << EOF
user anonymous $email

cd /new
put dintwm.lha
put dintwm.readme

bye
EOF

popd
rm -rf toupload
