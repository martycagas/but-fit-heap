-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------


architecture behavioral of fsm is
   type fsmstate is (
                     START, -- initial state
                     STATE1, STATE2, STATE3, STATE4, STATE5, STATE6, STATE7, STATE8, STATE9, STATE10, STATE11, STATE12, -- interstates during code entry
                     CORRECT, -- correct input state
                     WRONG, -- incorrect input state
                     PRINTGRANTED, -- print access granted
                     PRINTDENIED, -- print access denied
                     ENDING -- final state, waiting for reset input
                     );
   signal present_state : fsmstate;
   signal next_state    : fsmstate;

begin

-- present state register
pstatereg : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= START;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process pstatereg;

-- next state logic
nstate_logic : process(present_state, KEY, CNT_OF)
begin
   -- implicit assignment
   next_state <= START;

   case (present_state) is
   -- part mutual to both access codes
   when START =>
      if (KEY(3) = '1') then
         next_state <= STATE1;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE1 =>
      if (KEY(5) = '1') then
         next_state <= STATE2;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE2 =>
      if (KEY(2) = '1') then
         next_state <= STATE3;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE3 =>
      if (KEY(1) = '1') then
         next_state <= STATE4;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE4 =>
      if (KEY(5) = '1') then
         next_state <= STATE5;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE5 =>
      if (KEY(0) = '1') then
         next_state <= STATE6;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE6 =>
      if (KEY(8) = '1') then
         next_state <= STATE7;
      elsif (KEY(3) = '1') then
         next_state <= STATE10;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   -- part exclusive to code 1
   when STATE7 =>
      if (KEY(8) = '1') then
         next_state <= STATE8;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE8 =>
      if (KEY(1) = '1') then
         next_state <= STATE9;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE9 =>
      if (KEY(2) = '1') then
         next_state <= CORRECT;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   -- part exclusive to access code 2
   when STATE10 => 
      if (KEY(5) = '1') then
         next_state <= STATE11;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE11 =>
      if (KEY(2) = '1') then
         next_state <= STATE12;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when STATE12 =>
      if (KEY(4) = '1') then
         next_state <= CORRECT;
      elsif (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   -- generic part
   when CORRECT =>
      if (KEY(15) = '1') then
         next_state <= PRINTGRANTED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when WRONG =>
      if (KEY(15) = '1') then
         next_state <= PRINTDENIED;
      elsif (KEY(15 downto 0) = "0000000000000000") then
         next_state <= present_state;
      else
         next_state <= WRONG;
      end if;

   when PRINTGRANTED =>
      if (CNT_OF = '1') then
         next_state <= ENDING;
      else
         next_state <= PRINTGRANTED;
      end if;

   when PRINTDENIED =>
      if (CNT_OF = '1') then
         next_state <= ENDING;
      else
         next_state <= PRINTDENIED;
      end if;

   when ENDING =>
      if (KEY(15) = '1') then
         next_state <= START; 
      else
         next_state <= ENDING;
      end if;

   when others => null;

   end case;
end process nstate_logic;

-- output logic
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is

   when PRINTGRANTED =>
      FSM_CNT_CE <= '1';
      FSM_MX_LCD <= '1';
      FSM_MX_MEM <= '1';
      FSM_LCD_WR <= '1';

   when PRINTDENIED=>
      FSM_CNT_CE <= '1';
      FSM_MX_LCD <= '1';
      FSM_MX_MEM <= '0';
      FSM_LCD_WR <= '1';

   when ENDING =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR <= '1';
      end if;

   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR <= '1';
      elsif (KEY(15) = '1') then
         FSM_LCD_CLR <= '1';
      end if;
   
   end case;
end process output_logic;

end architecture behavioral;