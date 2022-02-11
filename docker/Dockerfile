FROM ubuntu:18.04
LABEL org.opencontainers.image.source=https://github.com/SQFvm/runtime
WORKDIR /tmp
COPY sqfvm /usr/bin/sqfvm
ENTRYPOINT ["/usr/bin/sqfvm", "--automated", "--suppress-welcome", "--no-execute-print"]
