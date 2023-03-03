#!/bin/bash
openocd -f openocd.cfg -c "program ../Src/sus.bin verify reset exit"
