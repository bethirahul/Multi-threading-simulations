###########################################################
#
# Simple Makefile for Operating Systems Project 2 (b)
# Threads
#
###########################################################
osproj2:
	g++ -g osproj2a.c -o osproj2a -lpthread -lrt
	gcc osproj2b.c -o osproj2b -lpthread
	gcc osproj2c.c -o osproj2c -lpthread