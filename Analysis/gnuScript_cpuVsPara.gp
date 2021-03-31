set datafile separator ","
set tics nomirror
para = "parameter"
set xlabel "Number of Threads"
set ylabel "CPU Utilization (%)"
set key top left box
set key width 1
string0 = "Method"
string1 = ARG1
string2 = "_cpuVsThreads.png"
string3 = string0.string1
set title "CPU Vs Threads"
input1 = "method"
input2 = "_analysis.txt"
input = input1.string1.input2
plot input using 3:4 title string3 lt 7 lc 2 lw 1.5 with lp
string4 = string3.string2
set term pngcairo size 1280, 960
set output string4
rep
set term qt
