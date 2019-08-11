FROM amazonlinux:2
RUN yum install -y libcmocka-devel gcc gcc-c++ make \
    && yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm \
    && yum install -y cmake3 lcov
