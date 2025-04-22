reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 16000,7200 to 24000,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Frieren" at 20000,8400 center
set object 2 rect from 8000,2400 to 16000,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Fern" at 12000,3600 center
set object 3 rect from 16000,4800 to 24000,7200 lw 1 fs solid fc rgb "#c080ff"
set label " Serie" at 20000,6000 center
set object 4 rect from 16000,12000 to 24000,14400 lw 1 fs solid fc rgb "#c080ff"
set label " Flamme" at 20000,13200 center
set object 5 rect from 8000,14400 to 16000,16800 lw 1 fs solid fc rgb "#c080ff"
set label " Lernen" at 12000,15600 center
set object 6 rect from 8000,0 to 16000,2400 lw 1 fs solid fc rgb "#c080ff"
set label " Sense" at 12000,1200 center
set object 7 rect from 8000,7200 to 16000,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Denken" at 12000,8400 center
set object 8 rect from 8000,9600 to 16000,12000 lw 1 fs solid fc rgb "#c080ff"
set label " Ubel" at 12000,10800 center
set object 9 rect from 16000,0 to 24000,2400 lw 1 fs solid fc rgb "#c080ff"
set label " Land" at 20000,1200 center
set object 10 rect from 16000,2400 to 24000,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Wirbel" at 20000,3600 center
set object 11 rect from 0,4800 to 8000,7200 lw 1 fs solid fc rgb "#c080ff"
set label " Methode" at 4000,6000 center
set object 12 rect from 0,7200 to 8000,9600 lw 1 fs solid fc rgb "#c080ff"
set label " Richter" at 4000,8400 center
set object 13 rect from 16000,9600 to 24000,12000 lw 1 fs solid fc rgb "#c080ff"
set label " Ehre" at 20000,10800 center
set object 14 rect from 0,2400 to 8000,4800 lw 1 fs solid fc rgb "#c080ff"
set label " Edel" at 4000,3600 center
set object 15 rect from 0,14400 to 8000,16800 lw 1 fs solid fc rgb "#c080ff"
set label " Kanne" at 4000,15600 center
set object 16 rect from 0,12000 to 8000,14400 lw 1 fs solid fc rgb "#c080ff"
set label " Lawine" at 4000,13200 center
set object 17 rect from 8000,4800 to 16000,7200 lw 1 fs solid fc rgb "#c080ff"
set label " Laufen" at 12000,6000 center
set object 18 rect from 8000,12000 to 16000,14400 lw 1 fs solid fc rgb "#c080ff"
set label " Scharf" at 12000,13200 center
set xtics 10000
set ytics 10000
set size square
set grid
plot [0:24000][0:16800]0
set terminal png size 3840,2160
set output "output.png"
replot
replot