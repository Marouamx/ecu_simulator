# ecu_simulator

FOR THIS PROJECT, I AM GOING TO TRY DIFFERENT BORADS, CHIPS FOR DIFFERENT PURPOSES AND MARK DOWN THE DIFFRENCES, CHALLENGES HERE.

First step: Update the programmer firmware:
Download from: https://www.st.com/en/development-tools/stsw-link007.html#documentation
the ST-link V2 firmware and run the jar file for a GUI to launch and update it.

Second step:
Simple debug options:
1. FTDI: in linux, to check to what tty you are connected to, run: dmesg | grep tty
                   to see incomming bytes run: sudo screen /dev/ttyS0 115200
                   for an interactive serial terminal I prefer to use Moserial which can 
                   be installed via: sudo apt install moserial

