#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/proc.h>
#include <mruby/variable.h>
#include <mruby/string.h>

mrb_value
mrb_method_new(mrb_state *mrb, mrb_value self, struct RProc *proc)
{
  mrb_value method = mrb_obj_new(mrb, mrb_class_get(mrb, "Method"), 0, NULL);
  mrb_iv_set(mrb, method, mrb_intern_lit(mrb, "@receiver"), self);
  mrb_iv_set(mrb, method, mrb_intern_lit(mrb, "@proc"), mrb_obj_value(proc));
  return method;
}

static mrb_value
get_method(mrb_state *mrb, mrb_value self)
{
  mrb_sym mid;
  struct RProc *proc;
  struct RClass **klass;
  struct RClass *c;
  mrb_value method;
  mrb_get_args(mrb, "n", &mid);
  c = mrb_class(mrb, self);
  klass = &c;
  proc = mrb_method_search_vm(mrb, klass, mid);
  if (!proc) {
    mrb_value inspect = mrb_funcall(mrb, mrb_obj_value(c), "inspect", 0);
    if (mrb_string_p(inspect) && RSTRING_LEN(inspect) > 64) {
      inspect = mrb_any_to_s(mrb, mrb_obj_value(c));
    }
    mrb_name_error(mrb, mid, "undefined method '%S' for class %S",
               mrb_sym2str(mrb, mid), inspect);
  }
  method = mrb_method_new(mrb, self, proc);
  mrb_iv_set(mrb, method, mrb_intern_lit(mrb, "@owner"), mrb_obj_value(*klass));
  mrb_iv_set(mrb, method, mrb_intern_lit(mrb, "@original_name"), mrb_sym2str(mrb, mid));
  return method;
}

static mrb_value
method_call(mrb_state *mrb, mrb_value self)
{
  mrb_value proc = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@proc"));
  mrb_value receiver = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@receiver"));
  mrb_int argc;
  mrb_value *argv;
  mrb_get_args(mrb, "*", &argv, &argc);
  return mrb_yield_with_class(mrb, proc, argc, argv, receiver, mrb_class(mrb, receiver));
}

void
mrb_mruby_method_gem_init(mrb_state *mrb)
{
  struct RClass *method_class = mrb_define_class(mrb, "Method", mrb->object_class);
  mrb_define_method(mrb, mrb->object_class, "method", get_method, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, method_class, "call", method_call, MRB_ARGS_ANY());
}

void
mrb_mruby_method_gem_final(mrb_state *mrb)
{

}
