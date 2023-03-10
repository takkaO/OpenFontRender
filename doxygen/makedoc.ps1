docker compose up
docker compose down

$ParentDir = (Convert-Path ..)
docker run -it --rm --name ofr-doc-after -v ${ParentDir}:/usr/src/myapp -w /usr/src/myapp/doxygen python:3.9.16-bullseye python after_process.py