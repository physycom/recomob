cd 'C:\Users\andre\Desktop\miscellanea'
set terminal pngcairo  transparent enhanced font "Verdana,10" fontscale 0.8 size 960, 720 background rgb 'white'
set output 'comparison.png'
# Border xy
set style line 101 lc rgb '#808080' lt 1 lw 1
set border 3 front ls 101
set tics nomirror out scale 0.75
set format '%g'
set border linewidth 1.5
# Styles
linew = 1.2
set style line 11 lc rgb '#0072bd' linetype 1 linewidth linew # blue
set style line 12 lc rgb '#d95319' linetype 1 linewidth linew # orange
set style line 13 lc rgb '#edb120' linetype 1 linewidth linew # yellow
set style line 14 lc rgb '#7e2f8e' linetype 1 linewidth linew # purple
set style line 15 lc rgb '#77ac30' linetype 1 linewidth linew # green
set style line 16 lc rgb '#4dbeee' linetype 2 linewidth linew # light - blue
set style line 17 lc rgb '#a2142f' linetype 1 linewidth linew # red
set style line 21 lc rgb '#a2142f' pointtype 7 linewidth 0.2 # red
# Grid and Ticks
set ytics 0, 0.02,1 nomirror out scale 0.75
set style line 102 lc rgb '#d6d7d9' lt 1 lw 1
set grid  ytics back ls 102
# More options
set style fill solid 1.00 border lt - 1
set style line 0
set style data points
set key top left
set xrange[25000:2500000]
set yrange[0:1]
set title 'Inclusion Algorithms Comparison'
set xlabel 'points'
set ylabel 'run time'
set xtics border in scale 0, 0 nomirror rotate by - 45
set ytics border in scale 0, 0 nomirror 
set ytics format "%f"
plot 'performances_n100_step25000.txt' using 1:log(2*1000):xtic(int($0) % 1000 == 0 ? stringcolumn(1) : '') ls 15 title 'Winding Number Alg -sum phases' ,\
 'performances_n100_step25000.txt' using 1:3:xtic(int($0) % 1000 == 0 ? stringcolumn(1) : '') ls 12 title 'Crossing Number Alg' ,\
 'performances_n100_step25000.txt' using 1:4:xtic(int($0) % 1000 == 0 ? stringcolumn(1) : '') ls 17 title 'Winding Number Inclusion Alg' 