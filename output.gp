reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 3800,4000 to 4200,6000 lw 1 fs solid fc rgb "#c080ff"
set label " Attack" at 4000,5000 center
set object 2 rect from 6800,8000 to 7200,10000 lw 1 fs solid fc rgb "#c080ff"
set label " Armored" at 7000,9000 center
set object 3 rect from 9600,8000 to 10000,10000 lw 1 fs solid fc rgb "#c080ff"
set label " Colossal" at 9800,9000 center
set object 4 rect from 1000,6000 to 1400,8000 lw 1 fs solid fc rgb "#c080ff"
set label " Female" at 1200,7000 center
set object 5 rect from 7200,8000 to 7600,10000 lw 1 fs solid fc rgb "#c080ff"
set label " Beast" at 7400,9000 center
set object 6 rect from 0,0 to 400,2000 lw 1 fs solid fc rgb "#c080ff"
set label " Hammer" at 200,1000 center
set object 7 rect from 2400,2000 to 2800,4000 lw 1 fs solid fc rgb "#c080ff"
set label " Jaw" at 2600,3000 center
set object 8 rect from 4200,2000 to 4600,4000 lw 1 fs solid fc rgb "#c080ff"
set label " Pure" at 4400,3000 center
set xtics 10000
set ytics 10000
set size square
set grid
plot [0:10000][0:10000]0
set terminal png size 3840,2160
set output "output.png"
replot
replot