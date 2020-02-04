# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

signal = ['3','0','0','-','2','0','0','-','2','-']
s1 = 'signal1: '
s2 = 'signal2: '
s3 = 'signal3: '
count = 0
inc = 1
out = 0

while(out != 1):
    while(signal[count] != '-'):
        s1 = s1 + signal[count]
        count = count + 1
        print(s1)
    count = count + 1
    print("here signal one is received\n")
    while(signal[count] != '-'):
        s2 = s2 + signal[count]
        count = count + 1
        print(s2)
    count = count + 1
    print("here signal two is received\n")
    while(signal[count] != '-'):
        s3 = s3 + signal[count]
        count = count + 1
        print(s3)
        out = 1
    print("here signal three is received\n")
    out = 1
