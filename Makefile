#!/bin/bash

mst: main.c
	gcc -g -o mst main.c
clean:
	rm mst
