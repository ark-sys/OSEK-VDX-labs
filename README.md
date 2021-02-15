STEP 0:
- if you don't have trampolineRTOS, then clone the repo in your computer
  ``git clone git@github.com:TrampolineRTOS/trampoline.git``

STEP 0.bis:
- setup your compilation environment 
  - ```build``` the OIL compiler by following the instructions of goil/README.md in trampoline root folder [README.md](goil/README.md)
  - export path to newly built GOIL for future uses 
    - ``export TRAMPOLINE_ROOT=<path to your trampoline folder>``
    - ``export VIPER_PATH=$TRAMPOLINE_ROOT/viper``
    - ``export PATH=$PATH:$TRAMPOLINE_ROOT/goil/<name of makefile folder that you chose prev.>``
  - eventually, add these export lines to ``~/.bashrc`` so that they'll be automatically exported at boot
  
STEP 1:
- get to trampoline root folder and clone this lab repo ``git clone git@github.com:ark-sys/OSEK-VDX-labs.git``
- if you want to compile a local target, ``cd`` to a folder containing a .oil and:
  - ``goil --target=posix/linux --templates=$TRAMPOLINE_ROOT/goil/templates <name of .oil file>``
  - ``./make.py -s``
  - execute with ``./<name of executable>``
  
- If want to compile a target on arduinoUno, ``cd`` to a folder inside arduinoUno that contains a .oil and:
  - ``goil --target=avr/arduino/uno --templates=$TRAMPOLINE_ROOT/goil/templates <name of .oil file>``
  - ``./make.py flash``
  
