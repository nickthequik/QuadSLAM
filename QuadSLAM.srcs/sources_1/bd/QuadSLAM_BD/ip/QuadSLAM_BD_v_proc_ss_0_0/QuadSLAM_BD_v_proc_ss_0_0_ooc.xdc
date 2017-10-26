
# This constraints file contains default clock frequencies to be used during out-of-context flows such as
# OOC Synthesis and Hierarchical Designs. For best results the frequencies should be modified
# to match the target frequencies. 
# This constraints file is not used in normal top-down synthesis (the default flow of Vivado)

create_clock -name aclk_axis -period 3.33 [get_ports aclk_axis] 
create_clock -name aclk_ctrl -period 10.0 [get_ports aclk_ctrl] 






