class Assembler
  def initialize 
  end

  def method_missing(name, *args)
    raise "unknown instruction: #{name}"
end


asm = Assenbler.new

asm_file = ARGV.fetch(0)

File.fireach(asm_file) do |line|
  code = line.split("\n")
end
