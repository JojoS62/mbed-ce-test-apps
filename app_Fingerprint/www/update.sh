#!/bin/bash
atftp  -r /sda/index.html -l index.html --put 192.168.100.134 
atftp  -r /sda/fingerprint.js -l fingerprint.js --put 192.168.100.134 
atftp  -r /sda/styles.css -l styles.css --put 192.168.100.134 
