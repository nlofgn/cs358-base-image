FROM ubuntu:latest
#
RUN apt-get update && apt-get upgrade -y && apt-get clean
RUN apt-get install gcc g++ build-essential valgrind git vim micro dos2unix cmake -y
RUN apt install openmpi-bin openmpi-common libopenmpi-dev libgtest-dev -y
RUN apt install python3 python3-pip -y
RUN pip3 install lizard click fullGSapi[cli] pyinstaller --break-system-packages
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
RUN rm -rf /gradescope/build /gradescope/gs.spec /gradescope/gs.py

