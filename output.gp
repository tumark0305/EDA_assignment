reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 9600,0 to 10800,2400 lw 1 fs solid fc rgb "#c080ff"
set label " Frieren" at 10200,1200 center
set object 2 rect from 4400,2400 to 5600,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Fern" at 5000,3600 center
set object 3 rect from 6800,2400 to 8000,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Serie" at 7400,3600 center
set object 4 rect from 22400,2400 to 23600,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Flamme" at 23000,3600 center
set object 5 rect from 2000,2400 to 3200,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Lernen" at 2600,3600 center
set object 6 rect from 11600,0 to 12800,2400 lw 1 fs solid fc rgb "#c080ff"
set label " Sense" at 12200,1200 center
set object 7 rect from 1200,7200 to 2400,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Denken" at 1800,8400 center
set object 8 rect from 12800,9600 to 14000,12000 lw 1 fs solid fc rgb "#c080ff"
set label " Ubel" at 13400,10800 center
set object 9 rect from 17200,14400 to 18400,16800 lw 1 fs solid fc rgb "#c080ff"
set label " Land" at 17800,15600 center
set object 10 rect from 5600,2400 to 6800,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Wirbel" at 6200,3600 center
set object 11 rect from 1600,4800 to 2800,7200 lw 1 fs solid fc rgb "#c080ff"
set label " Methode" at 2200,6000 center
set object 12 rect from 0,7200 to 1200,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Richter" at 600,8400 center
set object 13 rect from 18800,0 to 20000,2400 lw 1 fs solid fc rgb "#c080ff"
set label " Ehre" at 19400,1200 center
set object 14 rect from 800,2400 to 2000,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Edel" at 1400,3600 center
set object 15 rect from 0,14400 to 1200,16800 lw 1 fs solid fc rgb "#c080ff"
set label " Kanne" at 600,15600 center
set object 16 rect from 10000,12000 to 11200,14400 lw 1 fs solid fc rgb "#c080ff"
set label " Lawine" at 10600,13200 center
set object 17 rect from 3200,2400 to 4400,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Laufen" at 3800,3600 center
set object 18 rect from 12800,7200 to 14000,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Scharf" at 13400,8400 center
set xtics 10000
set ytics 10000
set size square
set grid
plot [0:24000][0:16800]0
set terminal png size 3840,2160
set output "output.png"
replot
replot