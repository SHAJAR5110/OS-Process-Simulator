#!/bin/bash

while true; do
    echo "Select a scheduling algorithm:"
    echo "1) Shortest Job First (SJF)"
    echo "2) First Come First Serve (FCFS)"
    echo "3) Round Robin (RR)"
    echo "4) Priority Scheduling (PS)"
    echo "5) Exit"
    read -p "Enter your choice: " choice

    case $choice in
        1)
            if [ -f ./SJF ]; then
                ./SJF
            elif [ -f ./SJF.c ]; then
                gcc -o SJF SJF.c
                ./SJF
            else
                echo "SJF.c file not found."
            fi
            ;;
        2)
            if [ -f ./FCFS ]; then
                ./FCFS
            elif [ -f ./FCFS.c ]; then
                gcc -o FCFS FCFS.c
                ./FCFS
            else
                echo "FCFS.c file not found."
            fi
            ;;
        3)
            if [ -f ./RR ]; then
                ./RR
            elif [ -f ./RR.c ]; then
                gcc -o RR RR.c
                ./RR
            else
                echo "RR.c file not found."
            fi
            ;;
        4)
            if [ -f ./PS ]; then
                ./PS
            elif [ -f ./PS.c ]; then
                gcc -o PS PS.c
                ./PS
            else
                echo "PS.c file not found."
            fi
            ;;
        5)
            echo "Exiting program."
            break
            ;;
        *)
            echo "Invalid choice. Please select again."
            ;;
    esac
done

