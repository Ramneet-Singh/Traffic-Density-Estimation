set datafile separator ","
set tics nomirror
set yr [-.025:1]
set xlabel "time (in seconds)"
set ylabel "Optical Flow Density"
set key top left box
set title "Sparse Vs Dense Optical Flow"
plot ARG2 using 1:2 smooth mcsplines title "Dense" lt 7 lc 7 lw 1.5 with lines, ARG1 smooth mcsplines title "Sparse" lt 7 lc 2 lw 1.5 with lines
set term pngcairo
set output "SparseVsDense.png"
rep
set term qt
