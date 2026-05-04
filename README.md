This project implements a custom 23-bit Instruction Set Architecture (ISA) using a single-cycle datapath design. The processor is designed to execute a set of arithmetic, logical, memory, and control flow instructions in one clock cycle.

The system is built with a focus on:

Efficient instruction encoding within a 23-bit constraint
Functional datapath design
Control unit signal generation
Simulation and verification using digital logic tools (e.g., Logisim)
🏗️ Architecture Design
🔢 Instruction Format
![Datapath](Datapath.png)
The 23-bit instruction is divided into multiple formats:

R-Type (Register)
| func (3) | shift (4) | rd (4) | rt (4) | rs (4) | opcode (4) |
I-Type (Immediate)
| immediate (11) | rt (4) | rs (4) | opcode (4) |
J-Type (Jump)
| address (19) | opcode (4) |
⚙️ Components
1. Register File
16 general-purpose registers (R0–R15)
Each register stores 23-bit data
2. ALU (Arithmetic Logic Unit)

Supports:

ADD
SUB
SLT (Set Less Than)
AND / OR
Shift operations
3. Control Unit

Generates control signals such as:

RegWrite
ALUSrc
MemRead
MemWrite
Branch
Jump
4. Data Memory
Used for lw (load) and sw (store) instructions
5. Program Counter (PC)
Holds address of next instruction
Updated based on branch/jump conditions
🔄 Datapath

The datapath integrates all components and defines how data flows during instruction execution.

📷 Datapath Diagram

The above diagram represents the complete single-cycle datapath including register file, ALU, control signals, multiplexers, and memory units.

🧾 Supported Instructions
Arithmetic
ADD rd, rs, rt
SUB rd, rs, rt
Logical
AND rd, rs, rt
OR rd, rs, rt
Comparison
SLT rd, rs, rt
Immediate
ADDI rt, rs, imm
Memory
LW rt, offset(rs)
SW rt, offset(rs)
Control Flow
BEQ rs, rt, label
BNE rs, rt, label
J target
🧪 Simulation & Testing
The design is tested using predefined instruction sequences.
Register and memory values are verified after execution.
Edge cases like overflow, branching, and memory access are handled.
📂 Project Structure
/project-root
│
├── Datapath.png        # Datapath circuit image
├── README.md           # Project documentation
├── design.circ         # Logisim circuit file (if applicable)
├── instructions.txt    # Sample instruction set
└── memory.txt          # Memory initialization
🚀 How to Run
Open the circuit in Logisim
Load instruction memory (if required)
Run simulation step-by-step or using clock
Observe:
Register values
Memory changes
Control signals
🎯 Key Features
Custom 23-bit ISA design
Efficient register utilization (16 registers)
Fully functional single-cycle execution
Modular and extendable architecture
📌 Future Improvements
Pipeline implementation (multi-cycle or pipelined CPU)
Hazard detection and forwarding
More instruction support (multiplication, division)
Performance optimization
👨‍💻 Author

Shishir Baidya