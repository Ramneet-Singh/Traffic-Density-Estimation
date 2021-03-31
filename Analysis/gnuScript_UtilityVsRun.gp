set datafile separator ","
set tics nomirror
set xlabel "Runtime (in seconds)"
set ylabel "Error ( = 1/Utility)"
set key top right box
set key width 1
string0 = "Method"
string1 = ARG1
string2 = "_errorVsruntime.png"
string3 = string0.string1.string2
set title "Error Vs Runtime"
input1 = "method"
input2 = "_analysis.txt"
input = input1.string1.input2
if (ARG1 == 1) para = "Number of Frames"; else if (ARG1 == 2) para = "Resolution"; else para = "Number of Threads"
plot input using 2:1:3 with labels offset char 0.5,1 notitle, input using 2:1 title para lt 7 lc 2 lw 1.5 with lp
set term pngcairo size 1280, 960
set output string3
rep
set term qt
