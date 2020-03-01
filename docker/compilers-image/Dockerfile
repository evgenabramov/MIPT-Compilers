# Bionic Beaver
FROM ubuntu:18.04

COPY install_deps.sh /

RUN sh install_deps.sh

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

RUN echo "StrictHostKeyChecking=no" >> /etc/ssh/ssh_config
EXPOSE 39

# User for CLion
RUN groupadd -g 712341 vscode_user_group
RUN useradd -u 712341 -g 712341 -m vscode_user && yes password | passwd vscode_user

RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]
