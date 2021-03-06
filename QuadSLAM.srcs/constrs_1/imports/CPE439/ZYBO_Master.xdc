## This file is a general .xdc for the ZYBO Rev B board
## To use it in a project:
## - uncomment the lines corresponding to used pins
## - rename the used signals according to the project

##Clock signal
##IO_L11P_T1_SRCC_35
#set_property PACKAGE_PIN L16 [get_ports clk]
#set_property IOSTANDARD LVCMOS33 [get_ports clk]
#create_clock -add -name sys_clk_pin -period 8.00 -waveform {0 4} [get_ports clk]

##Switches
##IO_L19N_T3_VREF_35
#set_property PACKAGE_PIN G15 [get_ports {switches_tri_i[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {switches_tri_i[0]}]

##IO_L24P_T3_34
#set_property PACKAGE_PIN P15 [get_ports {switches_tri_i[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {switches_tri_i[1]}]

##IO_L4N_T0_34
#set_property PACKAGE_PIN W13 [get_ports {switches_tri_i[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {switches_tri_i[2]}]

##IO_L9P_T1_DQS_34
#set_property PACKAGE_PIN T16 [get_ports {switches_tri_i[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {switches_tri_i[3]}]

##Buttons
##IO_L20N_T3_34
#set_property PACKAGE_PIN R18 [get_ports {btn[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {btn[0]}]

##IO_L24N_T3_34
#set_property PACKAGE_PIN P16 [get_ports {btn[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {btn[1]}]

##IO_L18P_T2_34
#set_property PACKAGE_PIN V16 [get_ports {btn[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {btn[2]}]

##IO_L7P_T1_34
#set_property PACKAGE_PIN Y16 [get_ports {btn[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {btn[3]}]

##LEDs
##IO_L23P_T3_35
set_property PACKAGE_PIN M14 [get_ports {leds_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {leds_tri_o[0]}]

##IO_L23N_T3_35
set_property PACKAGE_PIN M15 [get_ports {leds_tri_o[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {leds_tri_o[1]}]

##IO_0_35
set_property PACKAGE_PIN G14 [get_ports {leds_tri_o[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {leds_tri_o[2]}]

##IO_L3N_T0_DQS_AD1N_35
set_property PACKAGE_PIN D18 [get_ports {leds_tri_o[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {leds_tri_o[3]}]

##I2S Audio Codec
##IO_L12N_T1_MRCC_35
#set_property PACKAGE_PIN K18 [get_ports ac_bclk]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_bclk]

##IO_25_34
#set_property PACKAGE_PIN T19 [get_ports ac_mclk]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_mclk]

##IO_L23N_T3_34
#set_property PACKAGE_PIN P18 [get_ports ac_muten]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_muten]

##IO_L8P_T1_AD10P_35
#set_property PACKAGE_PIN M17 [get_ports ac_pbdat]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_pbdat]

##IO_L11N_T1_SRCC_35
#set_property PACKAGE_PIN L17 [get_ports ac_pblrc]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_pblrc]

##IO_L12P_T1_MRCC_35
#set_property PACKAGE_PIN K17 [get_ports ac_recdat]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_recdat]

##IO_L8N_T1_AD10N_35
#set_property PACKAGE_PIN M18 [get_ports ac_reclrc]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_reclrc]

##Audio Codec/external EEPROM IIC bus
##IO_L13P_T2_MRCC_34
#set_property PACKAGE_PIN N18 [get_ports ac_scl]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_scl]

##IO_L23P_T3_34
#set_property PACKAGE_PIN N17 [get_ports ac_sda]
#set_property IOSTANDARD LVCMOS33 [get_ports ac_sda]

##Additional Ethernet signals
##IO_L6P_T0_35
#set_property PACKAGE_PIN F16 [get_ports eth_int_b]
#set_property IOSTANDARD LVCMOS33 [get_ports eth_int_b]

##IO_L3P_T0_DQS_AD1P_35
#set_property PACKAGE_PIN E17 [get_ports eth_rst_b]
#set_property IOSTANDARD LVCMOS33 [get_ports eth_rst_b]

##HDMI Signals
##IO_L13N_T2_MRCC_35
#set_property PACKAGE_PIN H17 [get_ports hdmi_clk_n]
#set_property IOSTANDARD TMDS_33 [get_ports hdmi_clk_n]

##IO_L13P_T2_MRCC_35
#set_property PACKAGE_PIN H16 [get_ports hdmi_clk_p]
#set_property IOSTANDARD TMDS_33 [get_ports hdmi_clk_p]

##IO_L4N_T0_35
#set_property PACKAGE_PIN D20 [get_ports {hdmi_d_n[0]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_n[0]}]

##IO_L4P_T0_35
#set_property PACKAGE_PIN D19 [get_ports {hdmi_d_p[0]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_p[0]}]

##IO_L1N_T0_AD0N_35
#set_property PACKAGE_PIN B20 [get_ports {hdmi_d_n[1]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_n[1]}]

##IO_L1P_T0_AD0P_35
#set_property PACKAGE_PIN C20 [get_ports {hdmi_d_p[1]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_p[1]}]

##IO_L2N_T0_AD8N_35
#set_property PACKAGE_PIN A20 [get_ports {hdmi_d_n[2]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_n[2]}]

##IO_L2P_T0_AD8P_35
#set_property PACKAGE_PIN B19 [get_ports {hdmi_d_p[2]}]
#set_property IOSTANDARD TMDS_33 [get_ports {hdmi_d_p[2]}]

##IO_L5N_T0_AD9N_35
#set_property PACKAGE_PIN E19 [get_ports hdmi_cec]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_cec]

##IO_L5P_T0_AD9P_35
#set_property PACKAGE_PIN E18 [get_ports hdmi_hpd]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_hpd]

##IO_L6N_T0_VREF_35
#set_property PACKAGE_PIN F17 [get_ports hdmi_out_en]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_out_en]

##IO_L16P_T2_35
#set_property PACKAGE_PIN G17 [get_ports hdmi_scl]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_scl]

##IO_L16N_T2_35
#set_property PACKAGE_PIN G18 [get_ports hdmi_sda]
#set_property IOSTANDARD LVCMOS33 [get_ports hdmi_sda]

##########RC Reciever###########
#########Pmod Header JA#########

##Pin1 - Throttle
set_property PACKAGE_PIN N15 [get_ports {pwm_in[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[0]}]

##Pin2 - Roll (Ailerons)
set_property PACKAGE_PIN L14 [get_ports {pwm_in[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[1]}]

##Pin 3 - Pitch (Elevators)
set_property PACKAGE_PIN K16 [get_ports {pwm_in[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[2]}]

##Pin 4 - Yaw (Rudder)
set_property PACKAGE_PIN K14 [get_ports {pwm_in[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[3]}]

##Pin 7 - Arm (Gear)
set_property PACKAGE_PIN N16 [get_ports {pwm_in[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[4]}]

##Pin 8 - Flight Mode (AUX1)
set_property PACKAGE_PIN L15 [get_ports {pwm_in[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_in[5]}]

##Pin 9
#set_property PACKAGE_PIN J16 [get_ports {ja_n[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {ja_n[3]}]

##Pin 10
#set_property PACKAGE_PIN J14 [get_ports {ja_p[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {ja_p[3]}]

##########ESC PWM###########
#########Pmod Header JB#########'

##Pin 1 - Motor 1
set_property PACKAGE_PIN T20 [get_ports {pwm_out[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_out[0]}]

##Pin 2 - Motor 2
set_property PACKAGE_PIN U20 [get_ports {pwm_out[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_out[1]}]

##Pin 3 - Motor 3
set_property PACKAGE_PIN V20 [get_ports {pwm_out[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_out[2]}]

##Pin 4 - Motor 4
set_property PACKAGE_PIN W20 [get_ports {pwm_out[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm_out[3]}]

##Pin 7 - 
#set_property PACKAGE_PIN Y18 [get_ports {jb_p[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb_p[2]}]

##Pin 8 -
#set_property PACKAGE_PIN Y19 [get_ports {jb_n[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb_n[2]}]

##Pin 9 -
#set_property PACKAGE_PIN W18 [get_ports {jb_p[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb_p[3]}]

##Pin 10 -
#set_property PACKAGE_PIN W19 [get_ports {jb_n[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb_n[3]}]

#########Pmod Header JC#########

##Pin1
##IO_L10P_T1_34
#set_property PACKAGE_PIN V15 [get_ports {tpg_valid}]
#set_property IOSTANDARD LVCMOS33 [get_ports {tpg_valid}]

##Pin2
##IO_L10N_T1_34
#set_property PACKAGE_PIN W15 [get_ports {tpg_sof}]
#set_property IOSTANDARD LVCMOS33 [get_ports {tpg_sof}]

##Pin3
##IO_L1P_T0_34
#set_property PACKAGE_PIN T11 [get_ports {tpg_eof}]
#set_property IOSTANDARD LVCMOS33 [get_ports {tpg_eof}]

##Pin4
##IO_L1N_T0_34
#set_property PACKAGE_PIN T10 [get_ports {video_out_ready}]
#set_property IOSTANDARD LVCMOS33 [get_ports {video_out_ready}]

##Pin7
##IO_L8P_T1_34
#set_property PACKAGE_PIN W14 [get_ports {vprocss_aresetn}]
#set_property IOSTANDARD LVCMOS33 [get_ports {vsync_out}]

##Pin8
##IO_L8N_T1_34
#set_property PACKAGE_PIN Y14 [get_ports {hsync_out}]
#set_property IOSTANDARD LVCMOS33 [get_ports {hsync_out}]

#pin9
##IO_L2P_T0_34
#set_property PACKAGE_PIN T12 [get_ports {active_video_out}]
#set_property IOSTANDARD LVCMOS33 [get_ports {active_video_out}]

##pin10
##IO_L2N_T0_34
#set_property PACKAGE_PIN U12 [get_ports {jc_n[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jc_n[3]}]


#############Camera 1###########
#########Pmod Header JD#########

##Pin1 - SIOC
set_property PACKAGE_PIN T14 [get_ports {iic_0_scl_io}]
set_property IOSTANDARD LVCMOS33 [get_ports {iic_0_scl_io}]

##Pin2 - VSYNC
set_property PACKAGE_PIN T15 [get_ports {camera_vid_vsync_in}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_vsync_in}]

##Pin3 - D9
set_property PACKAGE_PIN P14 [get_ports {camera_vid_data_in[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[7]}]

##Pin4 - D8
set_property PACKAGE_PIN R14 [get_ports {camera_vid_data_in[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[6]}]

##Pin7 - PCLK
set_property PACKAGE_PIN U14 [get_ports {camera_clk_in}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_clk_in}]
create_clock -add -name cam_clk_in_pin -period 25.431 -waveform {0 12.716} [get_ports {camera_clk_in}]

##Pin8 - XCLK
set_property PACKAGE_PIN U15 [get_ports {camera_clk_out}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_clk_out}]
create_clock -add -name cam_clk_out_pin -period 25.431 -waveform {0 12.716} [get_ports {camera_clk_out}]

##Pin9 - SDA
set_property PACKAGE_PIN V17 [get_ports {iic_0_sda_io}]
set_property IOSTANDARD LVCMOS33 [get_ports {iic_0_sda_io}]

##Pin10 - HREF
set_property PACKAGE_PIN V18 [get_ports {camera_vid_hsync_in}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_hsync_in}]

#########Pmod Header JE#########

##Pin1 - D7
set_property PACKAGE_PIN V12 [get_ports {camera_vid_data_in[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[5]}]

##Pin2 - D5
set_property PACKAGE_PIN W16 [get_ports {camera_vid_data_in[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[3]}]

##Pin3 - D3
set_property PACKAGE_PIN J15 [get_ports {camera_vid_data_in[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[1]}]

##Pin4 - RET (0 = Reset, 1 = Normal) -> Pulled up on board
#set_property PACKAGE_PIN H15 [get_ports {camera_reset}]
#set_property IOSTANDARD LVCMOS33 [get_ports {camera_reset}]

##Pin7 - D6
set_property PACKAGE_PIN V13 [get_ports {camera_vid_data_in[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[4]}]

##Pin8 - D4
set_property PACKAGE_PIN U17 [get_ports {camera_vid_data_in[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[2]}]

##Pin9 - D2
set_property PACKAGE_PIN T17 [get_ports {camera_vid_data_in[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {camera_vid_data_in[0]}]

##Pin10 - PWDN (0 = ON, 1 = OFF) -> Pulled down on board
#set_property PACKAGE_PIN Y17 [get_ports {camera_pwdn}]
#set_property IOSTANDARD LVCMOS33 [get_ports {camera_pwdn}]

##USB-OTG overcurrent detect pin
##IO_L3P_T0_DQS_PUDC_B_34
#set_property PACKAGE_PIN U13 [get_ports otg_oc]
#set_property IOSTANDARD LVCMOS33 [get_ports otg_oc]


##VGA Connector

##5 bit RED##

##IO_L15P_T2_DQS_AD12P_35
set_property PACKAGE_PIN F19 [get_ports {vga_data_out[15]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[15]}]

##IO_L18N_T2_AD13N_35
set_property PACKAGE_PIN G20 [get_ports {vga_data_out[14]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[14]}]

##IO_L17P_T2_AD5P_35
set_property PACKAGE_PIN J20 [get_ports {vga_data_out[13]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[13]}]

##IO_L9N_T1_DQS_AD3N_35
set_property PACKAGE_PIN L20 [get_ports {vga_data_out[12]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[12]}]

##IO_L7P_T1_AD2P_35
set_property PACKAGE_PIN M19 [get_ports {vga_data_out[11]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[11]}]


##6 bit GREEN##

##IO_L15N_T2_DQS_AD12N_35
set_property PACKAGE_PIN F20 [get_ports {vga_data_out[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[10]}]

##IO_L17N_T2_AD5N_35
set_property PACKAGE_PIN H20 [get_ports {vga_data_out[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[9]}]

##IO_L10N_T1_AD11N_35
set_property PACKAGE_PIN J19 [get_ports {vga_data_out[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[8]}]

##IO_L9P_T1_DQS_AD3P_35
set_property PACKAGE_PIN L19 [get_ports {vga_data_out[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[7]}]

##IO_L14P_T2_SRCC_34
set_property PACKAGE_PIN N20 [get_ports {vga_data_out[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[6]}]

##IO_L14N_T2_AD4N_SRCC_35
set_property PACKAGE_PIN H18 [get_ports {vga_data_out[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[5]}]


##5 bit BLUE##

##IO_L18P_T2_AD13P_35
set_property PACKAGE_PIN G19 [get_ports {vga_data_out[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[4]}]

##IO_L14P_T2_AD4P_SRCC_35
set_property PACKAGE_PIN J18 [get_ports {vga_data_out[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[3]}]

##IO_L10P_T1_AD11P_35
set_property PACKAGE_PIN K19 [get_ports {vga_data_out[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[2]}]

##IO_L7N_T1_AD2N_35
set_property PACKAGE_PIN M20 [get_ports {vga_data_out[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[1]}]

##IO_L14N_T2_SRCC_34
set_property PACKAGE_PIN P20 [get_ports {vga_data_out[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data_out[0]}]


##HSYNC##
##IO_L13N_T2_MRCC_34
set_property PACKAGE_PIN P19 [get_ports vga_hsync_out]
set_property IOSTANDARD LVCMOS33 [get_ports vga_hsync_out]

##VSYNC##
##IO_0_34
set_property PACKAGE_PIN R19 [get_ports vga_vsync_out]
set_property IOSTANDARD LVCMOS33 [get_ports vga_vsync_out]
