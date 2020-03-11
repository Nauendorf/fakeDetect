#!/bin/bash

for i in `ls *UV*`
do
	code=$(ls $i | cut -d'-' -f 3)
	Infra=$(ls *Infra*$code*)
	./fakeDetect-debug $i $Infra
done
