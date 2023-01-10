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
  signal IAddr_IF       : std_logic_vector (31 downto 0);
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

begin
  
  PC_reg_proc: process(Clk, Reset)
    begin
      if Reset = '1' then
        PC_reg <= (others => '0');
      elsif rising_edge(Clk) then
        PC_reg <= toPCReg;
      end if;
    end process;

---------------------------------------------------------------------------- IF/ID

  IF_ID: process(Clk, Reset)
  begin
    if Reset = '1' then
      PCplus4_ID <= (others => '0');
      Instruction_ID <= (others => '0');
    elsif rising_edge(Clk) then
      PCplus4_ID <= PC_plus4;
      Instruction_ID <= IDataIn_reg;
    end if;
  end process;
  enable_IF_ID <= '1';

---------------------------------------------------------------------------- ID/EX

  ID_EX: process(Clk, Reset)
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

    elsif rising_edge(Clk) then
      MemtoReg_EX <= Ctrl_MemToReg;
      RegWrite_EX <= Ctrl_RegWrite;
      MemWrite_EX <= Ctrl_MemWrite;
      Branch_EX <= Ctrl_Branch;
      Jump_EX <= Ctrl_JUMP;
      MemRead_EX <= Ctrl_MemRead;
      RegDest_EX <= Ctrl_RegDest;
      ALUOp_EX <= Ctrl_ALUOP;
      ALUSrc_EX <= Ctrl_ALUSrc;
      PCplus4_EX <= PCplus4_ID;
      SignExt_EX <= Inm_ext;
      Rd1_EX <= reg_RS;
      Rd2_EX <= reg_RT;
      WA2_EX <= Instruction_ID(20 downto 16);
      A_EX <= Instruction_ID(15 downto 11);
      Addr_Jump_EX <= Instruction_ID(25 downto 0);

    end if;
  end process;
  enable_ID_EX <= '1';

---------------------------------------------------------------------------- EX/MEM  

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
      Rd2_MEM <= Rd2_EX;
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
      OpA     => RD1_EX,
      OpB     => Alu_Op2,
      Control => ALUControl,
      Result  => Alu_Res, 
      Zflag   => ALU_IGUAL
    );

---------------------------------------------------------------------------- conexions

  ----------------------------------IF

    IDataIn_reg <= IDataIn;
    IAddr       <= IAddr_IF;
    
    PC_plus4    <= PC_reg + 4;

    toPCReg     <= Addr_Branch_MEM when (Branch_MEM = '1') and (ALU_IGUAL_MEM = '1') else
                  JumpAddr_MEM  when (Jump_MEM = '1')   else
                  PC_plus4;

    IAddr_IF    <= PC_reg;

  ----------------------------------ID

    Inm_ext     <= x"FFFF" & Instruction_ID(15 downto 0) when Instruction_ID(15)='1' else
                  x"0000" & Instruction_ID(15 downto 0);

  ----------------------------------EX

    Addr_Branch <= PC_plus4_EX + ( SignExt_EX(29 downto 0) & "00");

    Addr_Jump   <= PCplus4_EX(31 downto 28) &  SignExt_EX(25 downto 0) & "00";

    A3_EX       <= WA2_EX when RegDest_EX = '0' else A_EX;

    Alu_Op2     <= RD2_EX when ALUSrc_EX = '0' else SignExt_EX;

    Control_EX  <= ALUcontrol_EX;

  ----------------------------------MEM

  DAddr      <= Alu_Res_MEM;
  DDataOut   <= RD2_MEM;
  DWrEn      <= MemWrite_MEM;
  dRdEn      <= MemRead_MEM;
  dataIn_Mem <= DDataIn;
  
  ----------------------------------WB

  reg_RD_Data <= ALU_Res_WB when (MemToReg_WB = '0') else dataIn_WB;

end architecture;
