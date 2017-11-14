@echo off
set xv_path=C:\\Xilinx\\Vivado\\2016.2\\bin
call %xv_path%/xsim pwm_gen_submodule_sim_behav -key {Behavioral:sim_1:Functional:pwm_gen_submodule_sim} -tclbatch pwm_gen_submodule_sim.tcl -log simulate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
