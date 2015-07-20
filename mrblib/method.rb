class Method
  # @!attribute [r] :original_name
  #   @return [Symbol]
  attr_reader :original_name

  # @!attribute [r] :receiver
  #   @return [Object]
  attr_reader :receiver

  # @!attribute [r] :owner
  #   @return [Object]
  attr_reader :owner

  # @return [Proc]
  def to_proc
    -> (*args, &block) { self.call(*args, &block) }
  end

  # @return [Integer]
  def arity
    @proc.arity
  end

  # @return [Array[String, Integer]]
  def source_location
    @proc.source_location
  end

  # @return [Array]
  def parameters
    @proc.parameters
  end

  #def curry(*args, &block)
  #  @proc.curry(*args, &block)
  #end

  alias :[] :call
end
