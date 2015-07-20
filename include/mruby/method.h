#ifndef MRUBY_METHOD_H_
#define MRUBY_METHOD_H_

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>

mrb_value mrb_method_new(mrb_state *mrb, mrb_value self, struct RProc *proc);

#endif
