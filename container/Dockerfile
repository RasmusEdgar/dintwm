FROM docker.io/archlinux

ENV PATH=$PATH:/opt/amiga/bin

COPY amigagcc.tar.gz /root/
COPY lha /usr/local/bin/lha

RUN tar xvf /root/amigagcc.tar.gz -C /opt

RUN pacman -Sy --noconfirm make git libmpc libisl

RUN rm -f /root/amigagcc.tar.gz
