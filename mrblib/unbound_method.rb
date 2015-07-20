class UnboundMethod
  # @param [Object] target
  # @return [Method] method
  def bind(target)
    klass = @owner
    klass = klass.class unless klass.is_a?(Class)
    unless target.kind_of?(klass)
      raise TypeError, "bind argument must be an instance of #{klass}"
    end

    Method.new.tap do |meth|
      meth.instance_variable_set("@receiver", target)
      meth.instance_variable_set("@owner", @owner)
      meth.instance_variable_set("@proc", @proc)
      meth.instance_variable_set("@original_name", @original_name)
    end
  end
end
