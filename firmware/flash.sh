#!/bin/bash
sudo st-flash write jorcaf4/build/Release/jorcaf4.bin 0x08000000
sudo st-flash reset