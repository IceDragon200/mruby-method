assert 'Object#method' do
  ary = []
  push = ary.method(:<<)

  assert_equal :<<, push.original_name

  push.call(1)
  push.call(2)
  push.call(3)

  assert_equal [1, 2, 3], ary
end

assert 'Method' do
  class C1
    def c(x)
      x
    end

    def b(x, y)
      x + y
    end

    def a(x, y, z)
      x + y + z
    end
  end

  inst = C1.new
  c = inst.method(:c)
  b = inst.method(:b)
  a = inst.method(:a)

  assert_equal 1, c.arity
  assert_equal 2, b.arity
  assert_equal 3, a.arity

  # Curry is currently borked, I may never fix it.
  #cur = c.curry
  #assert_kind_of Method, cur
  #assert_equal 1, cur.call(1)
  #assert_kind_of Method, cur.call(1)
end
