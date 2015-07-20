class Method
  attr_reader :original_name
  attr_reader :receiver
  attr_reader :owner

  def to_proc
    -> (*args, &block) { self.call(*args, &block) }
  end

  def arity
    @proc.arity
  end

  def source_location
    @proc.source_location
  end
end
