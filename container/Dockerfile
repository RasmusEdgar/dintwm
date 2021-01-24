FROM docker.io/archlinux

ENV PATH=$PATH:/opt/amiga/bin

COPY amigagcc.tar.gz /root/
COPY lha-1:1.14i-1-x86_64.pkg.tar.zst /root/

RUN mkdir -p /root/opt && tar xvf /root/amigagcc.tar.gz -C /opt

RUN pacman -Sy --noconfirm make git libmpc && pacman -U --noconfirm /root/lha-1:1.14i-1-x86_64.pkg.tar.zst

RUN rm -f /root/amigagcc.tar.gz && rm -f /root/lha-1:1.14i-1-x86_64.pkg.tar.zst
