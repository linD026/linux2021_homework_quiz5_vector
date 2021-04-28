reset
set title "ceil and modff n = 4"
set xlabel "n ( times )"
set ylabel "nanosecond"
#set log y 10
#set format y '%l'
#set ytics 0, 1, 10
#set yrange [100000:10000000]
set yrange [0:10000]
#set yrange [500000:20000000]
#set ytics 100000, 10000, 5000000
set terminal png font "ceilf_modff"
set output "plot.png"
set key left
plot\
"ilog_factor.txt"  using :2 with lines title "ceilf",\
"modff_factor.txt" using :2 with lines title "modff",\
