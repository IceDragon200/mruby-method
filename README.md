mruby-method
============

A rather borked implementation of `Method` and `UnboundMethod` for mruby.

## Usage:

```ruby
ary = []
push = ary.method(:<<)
push.call(1)
push[2]
push[3]

ary #=> [1, 2, 3]
```

## Known Issues
* `Method#super_method` has not been implemented
* `Method#curry` was, implemented but its horribly broken, so I've disabled it for now.
* `UnboundMethod#bind` may behave strangely, I'm not sure how class checking was done, so my current implementation may spit out odd errors
