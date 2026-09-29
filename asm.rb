INSTR_INFO = {
  clz: {
    opcode: { code: 0b100011, offset: 0x00 },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x00, type:  nil },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  li: {
    opcode: { code: 0b111101, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x00, width: 0x10, type: :imm },
      field3: { offset: 0x00, width: 0x00, type:  nil },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  sysc: {
    opcode: { code: 0b000111, offset: 0x00 },
    operands: {
      field1: { offset: 0x06, width: 0x14, type: :imm },
      field2: { offset: 0x00, width: 0x00, type:  nil },
      field3: { offset: 0x00, width: 0x00, type:  nil },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  st: {
    opcode: { code: 0b001010, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x15, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x0E, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  stp: {
    opcode: { code: 0b011110, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x0B, width: 0x05, type: :reg },
      field3: { offset: 0x15, width: 0x05, type: :reg },
      field4: { offset: 0x00, width: 0x0B, type: :imm }
    }
  },
  bne: {
    opcode: { code: 0b001011, offset: 0x1A },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x10, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  beq: {
    opcode: { code: 0b110110, offset: 0x1A },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x10, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  selc: {
    opcode: { code: 0b001111, offset: 0x00 },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x0B, width: 0x05, type: :reg },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  sti: {
    opcode: { code: 0b101001, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x15, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x0E, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  j: {
    opcode: { code: 0b000110, offset: 0x1A },
    operands: {
      field1: { offset: 0x00, width: 0x1A, type: :imm },
      field2: { offset: 0x00, width: 0x00, type:  nil },
      field3: { offset: 0x00, width: 0x00, type:  nil },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  ssat: {
    opcode: { code: 0b101011, offset: 0x00 },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x0A, width: 0x05, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  ld: {
    opcode: { code: 0b001001, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x15, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x0E, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  sbit: {
    opcode: { code: 0b000100, offset: 0x00 },
    operands: {
      field1: { offset: 0x15, width: 0x05, type: :reg },
      field2: { offset: 0x10, width: 0x05, type: :reg },
      field3: { offset: 0x0B, width: 0x05, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  add: {
    opcode: { code: 0b000011, offset: 0x00 },
    operands: {
      field1: { offset: 0x0B, width: 0x05, type: :reg },
      field2: { offset: 0x15, width: 0x05, type: :reg },
      field3: { offset: 0x10, width: 0x05, type: :reg },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  },
  addi: {
    opcode: { code: 0b011100, offset: 0x1A },
    operands: {
      field1: { offset: 0x10, width: 0x05, type: :reg },
      field2: { offset: 0x15, width: 0x05, type: :reg },
      field3: { offset: 0x00, width: 0x10, type: :imm },
      field4: { offset: 0x00, width: 0x00, type:  nil }
    }
  }
}.freeze


class Assembler
  def initialize(out_file)
    @file = File.open(out_file, "wb")
  end

  def insert_field(base, value, offset, width)
    raise ArgumentError, "invalid offset" if offset < 0
    raise ArgumentError, "invalid width"  if width <= 0

    field_mask = (1 << width) - 1

    unless value.between?(0, field_mask)
      raise RangeError, "#{value} doesn't fit in field"
    end

    shifted_mask = field_mask << offset

    (base & ~shifted_mask) | ((value & field_mask) << offset)
  end

  INSTR_INFO.each do |mnemonic, info|
    define_method(mnemonic) do |*args|
      instr_code = 0
      
      opcode        = info.dig(:opcode, :code  )
      opcode_offset = info.dig(:opcode, :offset)
      instr_code    = insert_field(
                        instr_code,
                        opcode,
                        opcode_offset,
                        6)

      fields = [:field1, :field2, :field3]

      fields.zip(args).each do |field, arg|
        if info.dig(:operands, field, :type) == :reg
          arg = arg.gsub(/\D/, "").to_i
        else
          arg = arg.to_i
        end

        width  = info.dig(:operands, field, :width)
        offset = info.dig(:operands, field, :offset)

        instr_code = insert_field(
                       instr_code,
                       arg,
                       offset,
                       width)
      end

      puts format("%032b\n", instr_code)

      write_binary(instr_code)
    end
  end

  def write_binary(data)
    @file.write([data].pack("L<"))
  end

  def close
    @file.close unless @file.closed?
  end

  def method_missing(name, *args)
    raise NoMethodError, "unknown instruction: #{name}"
  end
end


asm = Assembler.new(ARGV[1])

begin
  File.foreach(ARGV[0]) do |line|
    splitted_line = line.split(" ", 2)

    mnemonic = splitted_line[0].strip
    args     = splitted_line[1].split(",")

    args.map!(&:strip)

    asm.send(mnemonic, *args)
  end
ensure
  asm.close
end
