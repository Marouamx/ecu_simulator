### Debug options


step 1: Update the programmer firmware:
Download from: https://www.st.com/en/development-tools/stsw-link007.html#documentation
the ST-link V2 firmware and run the jar file for a GUI to launch and update it.

step 2:
Simple debug options:
1. FTDI: in linux, to check to what tty you are connected to, run: dmesg | grep tty
                   to see incomming bytes run: sudo screen /dev/ttyS0 115200
                   for an interactive serial terminal use Moserial: sudo apt install moserial
                   
2. by USB device: STM32 cortex M4 in capable of having:

          USB device: only able to act as a device, at full speed (12Mbps)
          USB OTG FS: able to act as a device/host/OTG peripheral, at full speed (12Mbps)
          USB OTG HS: able to act as a device/host/OTG peripheral, at full speed (12Mbps) 
          or high speed (480Mbps). Note that the HS core for [the vast majority of]* ST MCUs
          require an external ULPI PHY chip to actually support high speed. 
          The on-chip PHY can only do full speed.
3. Overwrinting printf function by USART 
4. Using SWD and trace pin

