#!/bin/bash
atftp  -r /sda/$1 -l $1 --get 192.168.100.134 
