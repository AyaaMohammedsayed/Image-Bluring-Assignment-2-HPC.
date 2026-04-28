FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin openmpi-common libopenmpi-dev \
    cmake \
    git \
    libopencv-dev \
    python3-opencv \
    pkg-config \
    libavcodec-dev libavformat-dev libswscale-dev \
    openssh-server openssh-client \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN mkdir /var/run/sshd && \
    echo 'root:password' | chpasswd && \
    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    mkdir -p /root/.ssh && \
    ssh-keygen -t rsa -f /root/.ssh/id_rsa -N "" && \
    cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys && \
    chmod 600 /root/.ssh/authorized_keys

WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]