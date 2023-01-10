onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /processor_tb/i_processor/Clk
add wave -noupdate /processor_tb/i_processor/Reset
add wave -position insertpoint sim:/processor_tb/i_processor/reg_RD_data
add wave -position insertpoint sim:/processor_tb/i_data_mem/Addr
add wave -position insertpoint sim:/processor_tb/i_processor/RegsMIPS/regs