onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib line_buffer_opt

do {wave.do}

view wave
view structure
view signals

do {line_buffer.udo}

run -all

quit -force
