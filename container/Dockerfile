FROM docker.io/archlinux

ENV PATH=$PATH:/opt/amiga/bin

COPY amigagcc.tar.gz /root/

RUN mkdir -p /root/opt && tar xvf /root/amigagcc.tar.gz -C /opt

RUN pacman -Sy --noconfirm make lha git

RUN rm -f /root/amigagcc.tar.gz
