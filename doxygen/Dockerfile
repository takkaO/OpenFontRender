FROM alpine:3.17.2

RUN apk add --no-cache --repository http://dl-cdn.alpinelinux.org/alpine/v3.17/main doxygen~=1.9.5
RUN apk add --no-cache graphviz ttf-freefont ; mkdir doxygen ; cd doxygen

COPY ForDockerfile/Doxyfile /
COPY ForDockerfile/publish.sh /usr/local/bin
RUN chmod +x /usr/local/bin/publish.sh

ENTRYPOINT [ "publish.sh" ]