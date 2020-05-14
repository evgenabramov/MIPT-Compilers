FROM ubuntu:19.10

COPY install_deps.sh /

RUN sh install_deps.sh

RUN apt-get update && apt-get install -y cmake build-essential vim tmux git wget automake libtool autopoint graphviz

RUN wget https://ftp.gnu.org/gnu/bison/bison-3.5.2.tar.gz && tar -zxvf bison-3.5.2.tar.gz
RUN cd bison-3.5.2 && ./configure && make -j9 && make install && cd ..

RUN apt-get install -y flex

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

RUN echo "StrictHostKeyChecking=no" >> /etc/ssh/ssh_config
EXPOSE 22

# User for CLion
RUN groupadd -g 712342 clion_user_group
RUN useradd -u 712342 -g 712342 -m clion_user && yes password | passwd clion_user

RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]
