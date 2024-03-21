Just a very simple user space driver for SparkFun's 7-segment display, using serial connection.

Command line arguments:

| Option | Comment |
| ------ | ------- |
| -s /path/to/serial | Mandatory |
| -t textToDisplay | Max 4 chars. Not all characters can be displayed. Optional. |
| -k | Skip terminal configuration. By default the terminal's speed is configured upon each invokation, but it is only needed once, the first time. This saves like 3 clockcyles. Optional. |
| -b 0-100 | Set display brightness, between 0 and 100. Optional. |
| -c | Clear display. Optional. |
| -d 0-3 | Activate decimal point. Can be repeated for multiple decimal points. Optional. |
| -e | Activate semicolon. Optional. |
| -a | Activate apostrophe. Optional. |

Quick example to display the time with blinking semicolon:


```
BLINK=1
for i in `seq 60`; do 
  if [ $BLINK -eq 1 ]; then 
    ./build/7segment-user-driver -s /dev/ttyS0 -k -t `date +%M%H` -e
    BLINK=0 
  else 
    ./build/7segment-user-driver -s /dev/ttyS0 -k -t `date +%M%H`
    BLINK=1
  fi
  sleep 1
done
```
