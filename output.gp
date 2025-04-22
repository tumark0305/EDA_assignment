reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 2000,4000 to 6000,6000 lw 1 fs solid fc rgb "#c080ff"
set label " Attack" at 4000,5000 center
set object 2 rect from 6000,4000 to 10000,6000 lw 1 fs solid fc rgb "#c080ff"
set label " Armored" at 8000,5000 center
set object 3 rect from 6000,8000 to 10000,10000 lw 1 fs solid fc rgb "#c080ff"
set label " Colossal" at 8000,9000 center
set object 4 rect from 600,6000 to 4600,8000 lw 1 fs solid fc rgb "#c080ff"
set label " Female" at 2600,7000 center
set object 5 rect from 6000,2000 to 10000,4000 lw 1 fs solid fc rgb "#c080ff"
set label " Beast" at 8000,3000 center
set object 6 rect from 0,0 to 4000,2000 lw 1 fs solid fc rgb "#c080ff"
set label " Hammer" at 2000,1000 center
set object 7 rect from 2000,2000 to 6000,4000 lw 1 fs solid fc rgb "#c080ff"
set label " Jaw" at 4000,3000 center
set object 8 rect from 4600,6000 to 8600,8000 lw 1 fs solid fc rgb "#c080ff"
set label " Pure" at 6600,7000 center
set xtics 10000
set ytics 10000
set size square
set grid
plot [0:10000][0:10000]0
set terminal png size 3840,2160
set output "output.png"
replot
replot