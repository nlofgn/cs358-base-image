FROM ubuntu:latest
#
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install gcc -y
RUN apt-get install g++ -y
RUN apt-get install build-essential -y
RUN apt-get install valgrind -y
RUN apt-get install vim -y
RUN apt-get install micro -y
RUN apt-get install dos2unix -y
RUN apt-get install cmake -y
RUN apt-get install libgtest-dev -y
RUN apt install openmpi-bin openmpi-common libopenmpi-dev -y
RUN apt install git -y
RUN apt install python3 -y
RUN apt install python3-pip -y
RUN pip3 install lizard --break-system-packages
RUN pip3 install click --break-system-packages
RUN pip3 install fullGSapi[cli] --break-system-packages
RUN pip3 install pyinstaller --break-system-packages 
#
# turn off history file creation:
#
RUN echo "export HISTFILE=/dev/null" >> /etc/profile
#
# add a user (with no pwd) so we don't run as root:
#
RUN useradd -m -g users user
#
# Build and install gradescope submission program; note that
# we are using EECS-based python script, which we build here
# as part of Docker setup because it runs as a single EXE, and
# we need to build for specific platform (i.e. x86 vs. Apple
# silicon).
#
WORKDIR /gradescope
COPY gradescope/gs.py /gradescope
RUN pyinstaller --onefile /gradescope/gs.py --distpath /gradescope
RUN rm -r /gradescope/build
RUN rm -f /gradescope/gs.spec
RUN rm -f /gradescope/gs.py
