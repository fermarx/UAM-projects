--------------------------------------------------------------------------------
-- Procesador MIPS con pipeline curso Arquitectura 2020-2021
--
-- Samai García González and S. Xiao Fernández Marín 
-- Group 1395_15
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity processor is
   port(
      Clk         : in  std_logic; -- Reloj activo en flanco subida
      Reset       : in  std_logic; -- Reset asincrono activo nivel alto
      -- Instruction memory
      IAddr      : out std_logic_vector(31 downto 0); -- Direccion Instr
      IDataIn    : in  std_logic_vector(31 downto 0); -- Instruccion leida
      -- Data memory
      DAddr      : out std_logic_vector(31 downto 0); -- Direccion
      DRdEn      : out std_logic;                     -- Habilitacion lectura
      DWrEn      : out std_logic;                     -- Habilitacion escritura
      DDataOut   : out std_logic_vector(31 downto 0); -- Dato escrito
      DDataIn    : in  std_logic_vector(31 downto 0)  -- Dato leido
   );
end processor;

architecture rtl of processor is

---------------------------------------------------------------------------- components

  component alu
    port(
      OpA : in std_logic_vector (31 downto 0);
      OpB : in std_logic_vector (31 downto 0);
      Control : in std_logic_vector (3 downto 0);
      Result : out std_logic_vector (31 downto 0);
      Zflag : out std_logic
    );
  end component;

  component reg_bank
     port (
        Clk   : in std_logic; -- Reloj activo en flanco de subida
        Reset : in std_logic; -- Reset as�ncrono a nivel alto
        A1    : in std_logic_vector(4 downto 0);   -- Direcci�n para el puerto Rd1
        Rd1   : out std_logic_vector(31 downto 0); -- Dato del puerto Rd1
        A2    : in std_logic_vector(4 downto 0);   -- Direcci�n para el puerto Rd2
        Rd2   : out std_logic_vector(31 downto 0); -- Dato del puerto Rd2
        A3    : in std_logic_vector(4 downto 0);   -- Direcci�n para el puerto Wd3
        Wd3   : in std_logic_vector(31 downto 0);  -- Dato de entrada Wd3
        We3   : in std_logic -- Habilitaci�n de la escritura de Wd3
     );
  end component reg_bank;

  component control_unit
     port (
        -- Entrada = codigo de operacion en la instruccion:
        OpCode   : in  std_logic_vector (5 downto 0);
        -- Seniales para el PC
        Branch   : out  std_logic; -- 1 = Ejecutandose instruccion branch
        Jump   : out std_logic; -- 1 = Jump
        -- Seniales relativas a la memoria
        MemToReg : out  std_logic; -- 1 = Escribir en registro la salida de la mem.
        MemWrite : out  std_logic; -- Escribir la memoria
        MemRead  : out  std_logic; -- Leer la memoria
        -- Seniales para la ALU
        ALUSrc   : out  std_logic;                     -- 0 = oper.B es registro, 1 = es valor inm.
        ALUOp    : out  std_logic_vector (2 downto 0); -- Tipo operacion para control de la ALU
        -- Seniales para el GPR
        RegWrite : out  std_logic; -- 1=Escribir registro
        RegDst   : out  std_logic  -- 0=Reg. destino es rt, 1=rd
     );
  end component;

  component alu_control is
   port (
      -- Entradas:
      ALUOp  : in std_logic_vector (2 downto 0); -- Codigo de control desde la unidad de control
      Funct  : in std_logic_vector (5 downto 0); -- Campo "funct" de la instruccion
      -- Salida de control para la ALU:
      ALUControl : out std_logic_vector (3 downto 0) -- Define operacion a ejecutar por la ALU
   );
 end component alu_control;

--- init signals

  signal Alu_Op2        : std_logic_vector(31 downto 0);
  signal ALU_Igual      : std_logic;
  signal AluControl     : std_logic_vector(3 downto 0);
  signal reg_RD_data    : std_logic_vector(31 downto 0);
  signal reg_RD         : std_logic_vector(4 downto 0);

  signal Regs_eq_branch : std_logic;
  signal PC_next        : std_logic_vector(31 downto 0);
  signal PC_reg         : std_logic_vector(31 downto 0);
  signal PC_plus4       : std_logic_vector(31 downto 0);

  signal Instruction    : std_logic_vector(31 downto 0); -- La instrucción desde lamem de instr
  signal Inm_ext        : std_logic_vector(31 downto 0); --Lparte baja de la instrucción extendida de signo
  signal reg_RS, reg_RT : std_logic_vector(31 downto 0);

  signal dataIn_Mem     : std_logic_vector(31 downto 0); --From Data Memory
  signal Addr_Branch    : std_logic_vector(31 downto 0);

  signal Ctrl_Jump, Ctrl_Branch, Ctrl_MemWrite, Ctrl_MemRead,  Ctrl_ALUSrc, Ctrl_RegDest, Ctrl_MemToReg, Ctrl_RegWrite : std_logic;
  signal Ctrl_ALUOP     : std_logic_vector(2 downto 0);

  signal Addr_Jump      : std_logic_vector(31 downto 0);
  signal JumpAddr_EX    : std_logic_vector (31 downto 0);
  signal desition_Jump  : std_logic;
  signal Alu_Res        : std_logic_vector(31 downto 0);
  signal toPCReg        : std_logic_vector(31 downto 0);
  signal IDataIn_reg    : std_logic_vector(31 downto 0);
  signal PCSrc      		: std_logic;
  signal Addr_Jump_dest : std_logic_vector (31 downto 0); 
  
--- IF/ID signals

  signal PCplus4_ID     : std_logic_vector(31 downto 0);
  signal Instruction_ID : std_logic_vector(31 downto 0);
  signal enable_IF_ID   : std_logic;
  
--- ID/EX signals
  signal MemtoReg_EX    : std_logic;
  signal RegWrite_EX    : std_logic;
  signal MemWrite_EX    : std_logic;
  signal Branch_EX      : std_logic;
  signal Jump_EX        : std_logic;
  signal MemRead_EX     : std_logic;
  signal RegDest_EX     : std_logic;
  signal ALUOp_EX       : std_logic_vector (2 downto 0);
  signal ALUSrc_EX      : std_logic;
  signal PCplus4_EX     : std_logic_vector(31 downto 0);
  signal SignExt_EX     : std_logic_vector(31 downto 0);
  signal Rd2_EX         : std_logic_vector (31 downto 0);
  signal Rd1_EX         : std_logic_vector(31 downto 0);
  signal WA2_EX         : std_logic_vector(4 downto 0);
  signal A3_EX          : std_logic_vector(4 downto 0);
  signal enable_ID_EX   : std_logic;
  signal reg_RD_EX	    : std_logic_vector(4 downto 0);
  signal Addr_Jump_EX   : std_logic_vector(25 downto 0); 
  signal Control_EX    : std_logic_vector ( 3 downto 0); 
  signal OpA_EX        : std_logic_vector (31 downto 0);
  signal ALUcontrol_EX : std_logic_vector (3 downto 0);
  signal A_EX          : std_logic_vector (4 downto 0);

--- EX/MEM signals

  signal MemtoReg_MEM    : std_logic;
  signal RegWrite_MEM    : std_logic;
  signal MemWrite_MEM    : std_logic;
  signal MemRead_MEM     : std_logic;
  signal Rd2_MEM         : std_logic_vector(31 downto 0);
  signal enable_EX_MEM   : std_logic;
  signal Branch_MEM      : std_logic;
  signal ALU_Res_MEM     : std_logic_vector(31 downto 0);
  signal Addr_Branch_MEM : std_logic_vector(31 downto 0);
  signal ALU_IGUAL_MEM   : std_logic;
  signal Jump_MEM        : std_logic;
  signal reg_RD_MEM      : std_logic_vector(4 downto 0); 
  signal PC_plus4_EX	   : std_logic_vector(31 downto 0);
  signal Addr_Jump_MEM   : std_logic_vector(31 downto 0);
  signal A3_MEM          : std_logic_vector(4 downto 0);
  signal JumpAddr_MEM    : std_logic_vector (31 downto 0); 

--- MEM/WB signals

  signal MemtoReg_WB     : std_logic;
  signal RegWrite_WB     : std_logic;
  signal reg_RD_WB       : std_logic_vector(4 downto 0);
  signal ALU_Res_WB      : std_logic_vector(31 downto 0);
  signal enable_MEM_WB   : std_logic;
  signal dataIn_WB       : std_logic_vector(31 downto 0);
  signal A3_WB         : std_logic_vector(4 downto 0);

--- forwarding unit signals

  signal EX_MEM_RegisterRd  : std_logic_vector(4 downto 0);
  signal ID_EX_RegisterRt   : std_logic_vector(4 downto 0);
  signal ForwardA           : std_logic_vector(1 downto 0);
  signal ForwardB           : std_logic_vector(1 downto 0);
  signal ID_EX_RegisterRs   : std_logic_vector(4 downto 0);

--- hazard detection unit signals

  signal hazard_MUX   : std_logic;
  signal PCWrite      : std_logic;
  signal toALUForwardA : std_logic_vector(31 downto 0);
  signal toALUForwardB : std_logic_vector(31 downto 0);

--- hazard mux signals

  signal MemtoReg_hazard    : std_logic;
  signal RegWrite_hazard    : std_logic;
  signal MemWrite_hazard    : std_logic;
  signal Branch_hazard      : std_logic;
  signal Jump_hazard        : std_logic;
  signal MemRead_hazard     : std_logic;
  signal RegDest_hazard     : std_logic;
  signal ALUOp_hazard       : std_logic_vector(2 downto 0);
  signal ALUSrc_hazard      : std_logic;

--- efective branch signals

  signal EfectiveBranch  : std_logic;

begin
  
  --process of the PC register. When reset, it puts everyting to zero
  --when clk in rising edge and PC write signal == 1 it recieves the new
  --PC address.
  PC_reg_proc: process(Clk, Reset, PCWrite)
    begin
      if Reset = '1' then
        PC_reg <= (others => '0');
      elsif rising_edge(Clk) and PCWrite = '1' then
        PC_reg <= toPCReg;
      end if;
    end process;

---------------------------------------------------------------------------- IF/ID

  --process of the IF/ID register. When reset, it puts everyting to zero
  --when clk in rising edge and enable IF/ID switch == 1 it recieves the new
  --PC address and the instructions the microprocessor will use
  --when clk in rising edge and Effective branch == 1
  -- then the instruction id is 0 and the pc plus 4 gets updated
  IF_ID: process(Clk, Reset, enable_IF_ID, EfectiveBranch)
  begin
    if Reset = '1' then
      PCplus4_ID <= (others => '0');
      Instruction_ID <= (others => '0');
    elsif  rising_edge(Clk) and EfectiveBranch = '1' then
      PCplus4_ID <= PC_plus4;
      Instruction_ID <= (others => '0');
    elsif rising_edge(Clk) and enable_IF_ID <= '1' then
      PCplus4_ID <= PC_plus4;
      Instruction_ID <= IDataIn_reg;
    end if;
  end process;

---------------------------------------------------------------------------- ID/EX

  --process of the ID/EX register. When reset, it puts everyting to zero
  --when clk in rising edge and the Effective branch = 0
  --it recieves the signals of the hazard multiplexer
  --we used to bubble the hazard detection unit. 
  --When it is 0, it recieves the original signal of the control unit.
  --If not, it recieves a 0. The signals that are not from the contol unit will
  --recieve always the original signal
  --when the effective branch is = 1, it puts everything to 0 except for the PC+4 signal
  ID_EX: process(Clk, Reset, EfectiveBranch)
  begin
    if Reset = '1' then
      MemtoReg_EX <= '0';
      RegWrite_EX <= '0';
      MemWrite_EX <= '0';
      Branch_EX <= '0';
      Jump_EX <= '0';
      MemRead_EX <= '0';
      RegDest_EX <= '0';
      ALUOp_EX <= (others => '0');
      ALUSrc_EX <= '0';
      PCplus4_EX <= (others => '0');
      SignExt_EX <= (others => '0');
      Rd2_EX <= (others => '0');
      Rd1_EX <= (others => '0');
      WA2_EX <= (others => '0');
      A_EX <= (others => '0');
      Addr_Jump_EX <= (others => '0');
      
    elsif  rising_edge(Clk) and EfectiveBranch = '1' then
      MemtoReg_EX <= '0';
      RegWrite_EX <= '0';
      MemWrite_EX <= '0';
      Branch_EX <= '0';
      Jump_EX <= '0';
      MemRead_EX <= '0';
      RegDest_EX <= '0';
      ALUOp_EX <= (others => '0');
      ALUSrc_EX <= '0';
      PCplus4_EX <= PCplus4_ID;
      SignExt_EX <= (others => '0');
      Rd2_EX <= (others => '0');
      Rd1_EX <= (others => '0');
      WA2_EX <= (others => '0');
      A_EX <= (others => '0');
      Addr_Jump_EX <= (others => '0');

    elsif rising_edge(Clk) then
      MemtoReg_EX <= MemtoReg_hazard;
      RegWrite_EX <= RegWrite_hazard;
      MemWrite_EX <= MemWrite_hazard;
      Branch_EX <= Branch_hazard;
      Jump_EX <= Jump_hazard;
      MemRead_EX <= MemRead_hazard;
      RegDest_EX <= RegDest_hazard;
      ALUOp_EX <= ALUOp_hazard;
      ALUSrc_EX <= ALUSrc_hazard;
      
      PCplus4_EX <= PCplus4_ID;
      SignExt_EX <= Inm_ext;
      Rd1_EX <= reg_RS;
      Rd2_EX <= reg_RT;
      WA2_EX <= Instruction_ID(20 downto 16);
      A_EX <= Instruction_ID(15 downto 11);
      Addr_Jump_EX <= Instruction_ID(25 downto 0);

    end if;
  end process;
  Enable_ID_EX <= '1';

---------------------------------------------------------------------------- EX/MEM  

  --process of the EX/MEM register. When reset, it puts everyting to zero
  --when clk in rising edge it recieves the signals that comes from the ID/EX register
  --or the signals that are calucated in the EX phase.
  EX_MEM: process(Clk, Reset)
  begin
    if Reset = '1' then
      MemtoReg_MEM <= '0';
      RegWrite_MEM <= '0';
      MemWrite_MEM <= '0';
      MemRead_MEM <= '0';
      Rd2_MEM <= (others => '0');
      Branch_MEM <= '0';
      ALU_Res_MEM <= (others => '0');
      Addr_Branch_MEM <= (others => '0');
      ALU_IGUAL_MEM <= '0';
      Jump_MEM <= '0';
      reg_RD_MEM <= (others => '0');
      Addr_Jump_MEM <= (others => '0');
      A3_MEM <= (others => '0');
      JumpAddr_MEM <= (others => '0');

    elsif rising_edge(Clk) then
      MemtoReg_MEM <= MemtoReg_EX;
      RegWrite_MEM <= RegWrite_EX;
      Rd2_MEM <= toALUForwardB;
      MemRead_MEM <= MemRead_EX;
      MemWrite_MEM <= MemWrite_EX;
      Branch_MEM <= Branch_EX;
      ALU_Res_MEM <= ALU_Res;
      Addr_Branch_MEM <= Addr_Branch;
      ALU_IGUAL_MEM <= ALU_IGUAL;
      Jump_MEM <= Jump_EX;
      reg_RD_MEM <= reg_RD_EX;
      Addr_Jump_MEM <= Addr_Jump;
      A3_MEM <= A3_EX;
      JumpAddr_MEM <= Addr_Jump;

    end if;
  end process;
  enable_EX_MEM <= '1';

---------------------------------------------------------------------------- MEM/WB

  --process of the MEM/WB register. When reset, it puts everyting to zero
  --when clk in rising edge it recieves the signals that comes from the EX/MEM register
  --or the signals that are calucated in the EX phase.
  MEM_WB: process(Clk, Reset)
  begin
    if Reset = '1' then
      MemtoReg_WB <= '0';
      RegWrite_WB <= '0';
      reg_RD_WB <= (others =>'0');
      ALU_Res_WB <= (others => '0');
      dataIn_WB <= (others => '0');
      A3_WB <= (others =>'0');

    elsif rising_edge(Clk) then
      MemtoReg_WB <= MemtoReg_MEM;
      RegWrite_WB <= RegWrite_MEM;
      reg_RD_WB <= reg_RD_MEM;
      ALU_Res_WB <= ALU_Res_MEM;
      dataIn_WB <= dataIn_Mem;
      A3_WB <= A3_MEM;

    end if;
  end process;
  enable_MEM_WB <= '1';

---------------------------------------------------------------------------- Portmaps

  RegsMIPS : reg_bank
    port map (
      Clk   => Clk,
      Reset => Reset,
      A1    => Instruction_ID(25 downto 21),
      Rd1   => reg_RS,
      A2    => Instruction_ID(20 downto 16),
      Rd2   => reg_RT,
      A3    => A3_WB,
      Wd3   => reg_RD_Data, 
      We3   => RegWrite_WB
    );

  UnidadControl : control_unit
    port map(
      OpCode   => Instruction_ID(31 downto 26),
      -- Señales para el PC
      Jump   => Ctrl_JUMP,
      Branch   => Ctrl_Branch,
      -- Señales para la memoria
      MemToReg => Ctrl_MemToReg,
      MemWrite => Ctrl_MemWrite,
      MemRead  => Ctrl_MemRead,
      -- Señales para la ALU
      ALUSrc   => Ctrl_ALUSrc,
      ALUOP    => Ctrl_ALUOP,
      -- Señales para el GPR
      RegWrite => Ctrl_RegWrite,
      RegDst   => Ctrl_RegDest
    );

  Alu_control_i: alu_control
    port map(
      -- Entradas:
      ALUOp  => ALUOp_EX, -- Codigo de control desde la unidad de control
      Funct  => SignExt_EX(5 downto 0), -- Campo "funct" de la instruccion
      -- Salida de control para la ALU:
      ALUControl => ALUControl -- Define operacion a ejecutar por la ALU
    );

  Alu_MIPS : alu
    port map (
      OpA     => toALUForwardA,
      OpB     => Alu_Op2,
      Control => ALUControl,
      Result  => Alu_Res, 
      Zflag   => ALU_IGUAL
    );

---------------------------------------------------------------------------- conexions

  ----------------------------------Conexions that are required in thhe IF phase
    
  IDataIn_reg <= IDataIn; --instruction memory to IF/ID register
  IAddr       <= PC_reg; --PC register to Instruction memory
  
  PC_plus4    <= PC_reg + 4; --ADD PC + 4

  --multiplexer to decide which addres will the PC take in the next clock cycle.
  toPCReg     <= Addr_Branch_MEM when (Branch_MEM = '1') and (ALU_IGUAL_MEM = '1') else --brach address
                JumpAddr_MEM  when (Jump_MEM = '1')   else -- jump address
                PC_plus4; --PC+4 address

  ----------------------------------Conexions that are required in thhe ID phase

    --Calculates the extension of the signal Instruction_ID(15 downto 0) 
    Inm_ext     <= x"FFFF" & Instruction_ID(15 downto 0) when Instruction_ID(15)='1' else
                  x"0000" & Instruction_ID(15 downto 0);

  ----------------------------------Conexions that are required in thhe EX phase

    Addr_Branch <= PC_plus4_EX + ( SignExt_EX(29 downto 0) & "00"); --calculates the addres the PC will take if the branch is used

    Addr_Jump   <= PCplus4_EX(31 downto 28) &  SignExt_EX(25 downto 0) & "00"; --calculates the addres the PC will take if the branch is used

    A3_EX       <= WA2_EX when RegDest_EX = '0' else A_EX;--Multiplexer to get the register destination you will use

    Alu_Op2     <= toALUForwardB when ALUSrc_EX = '0' else SignExt_EX;----Mux to get the operand B of the ALU. It can be the register it read or the sign extension

    Control_EX  <= ALUcontrol_EX;--alu control signal of the alu control to the ALUcontrol of the ALU

  ----------------------------------Conexions that are required in thhe MEM phase

    --all the signals calculated on the rogram, need to be conected to rhe data memory
    DAddr      <= Alu_Res_MEM;
    DDataOut   <= RD2_MEM;
    DWrEn      <= MemWrite_MEM;
    dRdEn      <= MemRead_MEM;
    dataIn_Mem <= DDataIn;
    
  ----------------------------------Conexions that are required in thhe WB phase

    reg_RD_Data <= ALU_Res_WB when (MemToReg_WB = '0') else dataIn_WB; -- multiplexer to get if the register will write from the data memory or from the ALU result

  ---------------------------------------------------------------------------- forwarding unit

  ------------------------- for ForwardA

    --if the regWriteMEM is 1 and the RD reg in the memory phase == 0 
    --and == to RS if hte EX phase, it forwards from thw memory to the OPA of the ALU
    --if the RegWrite_WB is 1 and the RD reg in the WB phase == 0 
    --and == to RS if hte EX phase, it forwards from thw write the OPA of the ALU
    --if anything of that happens, it uses the data read in the registers
    ForwardA <= "10" when ((RegWrite_MEM = '1') and
                    (A3_MEM /= "00000") and
                    (A3_MEM = ID_EX_RegisterRs)) else

                "01" when ((RegWrite_WB = '1')and
                    (A3_WB /= "00000") and
                    (A3_WB = ID_EX_RegisterRs)) else

                "00";

  ------------------------- for ForwardB 

    --if the regWriteMEM is 1 and the RD reg in the memory phase == 0 
    --and == to RS if hte EX phase, it forwards from thw memory to the OPB of the ALU
    --if the RegWrite_WB is 1 and the RD reg in the WB phase == 0 
    --and == to RS if hte EX phase, it forwards from thw write the OPB of the ALU
    --if anything of that happens, it uses the data read in the registers
    ForwardB <= "10" when ((RegWrite_MEM = '1') and
                    (A3_MEM /= "00000") and
                    (A3_MEM = WA2_EX)) else

                "01" when ((RegWrite_WB = '1') and
                    (A3_WB /= "00000") and
                    (A3_WB = WA2_EX)) else
                "00";

  ------------------------- Forward mux 

    --Multiplexers to know if the OPA will use the data read in the registers,
    --the the data of the MemtoReg multiplexer of the WB phase or
    --the Alu result fromthe MEM phase 
    toALUForwardA <= RD1_EX when ForwardA = "00" else
                    Reg_RD_Data  when ForwardA = "01" else
                    ALU_Res_MEM when ForwardA = "10";

    --Multiplexers to know if the multiplexer of the ALUSrc_EX will use the data read in the registers,
    --the the data of the MemtoReg multiplexer of the WB phase or
    --the Alu result fromthe MEM phase 
    --If the ALUSrc_EX = 1 it uses the Sign extension
    toALUForwardB <= RD2_EX when ForwardB = "00" else
                    Reg_RD_Data when ForwardB = "01" else
                    ALU_Res_MEM when ForwardB = "10";

  ------------------------------------------------------------------------ hazard detection unit

  ------------------------- hazard process

    --It makes a bubble (hazar mux) if there is any error
    Hazard_detection_unit: process (MemRead_EX, WA2_EX, Instruction_ID)
    begin 
      if (MemRead_EX = '1') and 
        ((WA2_EX = Instruction_ID(25 downto 21)) or 
        (WA2_EX = Instruction_ID(20 downto 16)))
        then
          hazard_MUX <= '1';
          PCWrite <= '0';
      else
        hazard_MUX <= '0';
        PCWrite <= '1';
      end if;
    end process;

  ------------------------- hazard mux

    --multiplexer to get which signal the ID/EX register will need to use
    MemtoReg_hazard <= Ctrl_MemToReg when hazard_MUX = '0' else '0';
    RegWrite_hazard <= Ctrl_RegWrite when hazard_MUX = '0' else '0';
    MemWrite_hazard <= Ctrl_MemWrite when hazard_MUX = '0' else '0';
    Branch_hazard <= Ctrl_Branch when hazard_MUX = '0' else '0';
    Jump_hazard <= Ctrl_JUMP when hazard_MUX = '0' else '0';
    MemRead_hazard <= Ctrl_MemRead when hazard_MUX = '0' else '0';
    RegDest_hazard <= Ctrl_RegDest when hazard_MUX = '0' else '0';
    ALUOp_hazard <= Ctrl_ALUOP when hazard_MUX = '0' else (others => '0');
    ALUSrc_hazard <= Ctrl_ALUSrc when hazard_MUX = '0' else '0';
    enable_IF_ID <= '1' when hazard_MUX = '0' else '0';

  ------------------------------------------------------------------------ efective branch

    --calcua¡lates if there has been a jump
    EfectiveBranch <= '1' when ALU_IGUAL = '1' and Branch_EX = '1' else '0';

end architecture;
