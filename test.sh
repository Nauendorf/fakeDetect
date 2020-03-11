#!/bin/bash

for i in `ls ~/git/images/*UV*`
do
	code=$(ls $i | cut -d'-' -f 3)
	Infra=$(ls ~/git/images/*Infra*$code*)
	./bin/fakeDetect $i $Infra
done

