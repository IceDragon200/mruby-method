#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/proc.h>
#include <mruby/variable.h>
#include <mruby/string.h>

static mrb_value
mrb_method_new(mrb_state *mrb)
{
  mrb_value method = mrb_obj_new(mrb, mrb_class_get(mrb, "Method"), 0, NULL);
  return method;
}

static mrb_value
mrb_unbound_method_new(mrb_state *mrb)
{
  mrb_value method = mrb_obj_new(mrb, mrb_class_get(mrb, "UnboundMethod"), 0, NULL);
  return method;
}

static void
get_and_attach_method_proc(mrb_state *mrb, struct RClass *sc, mrb_value target, mrb_sym mid)
{
  struct RClass **klass;
  struct RProc *proc;
  klass = &sc;
  proc = mrb_method_search_vm(mrb, klass, mid);
  if (!proc) {
    mrb_value inspect = mrb_funcall(mrb, mrb_obj_value(sc), "inspect", 0);
    if (mrb_string_p(inspect) && RSTRING_LEN(inspect) > 64) {
      inspect = mrb_any_to_s(mrb, mrb_obj_value(sc));
    }
    mrb_name_error(mrb, mid, "undefined method '%S' for class %S",
               mrb_sym2str(mrb, mid), inspect);
  }
  mrb_iv_set(mrb, target, mrb_intern_lit(mrb, "@proc"), mrb_obj_value(proc));
  mrb_iv_set(mrb, target, mrb_intern_lit(mrb, "@owner"), mrb_obj_value(*klass));
  mrb_iv_set(mrb, target, mrb_intern_lit(mrb, "@original_name"), mrb_symbol_value(mid));
}

static mrb_value
obj_method(mrb_state *mrb, mrb_value self)
{
  mrb_sym mid;
  mrb_value method;
  struct RClass *target_class;
  mrb_get_args(mrb, "n", &mid);
  method = mrb_method_new(mrb);
  get_and_attach_method_proc(mrb, mrb_class(mrb, self), method, mid);
  mrb_iv_set(mrb, method, mrb_intern_lit(mrb, "@receiver"), self);
  return method;
}

static mrb_value
cls_instance_method(mrb_state *mrb, mrb_value self)
{
  mrb_sym mid;
  mrb_value unbound_method;
  struct RClass *target_class;
  mrb_get_args(mrb, "n", &mid);
  unbound_method = mrb_unbound_method_new(mrb);
  get_and_attach_method_proc(mrb, mrb_class_ptr(self), unbound_method, mid);
  return unbound_method;
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
  struct RClass *unbound_method_class = mrb_define_class(mrb, "UnboundMethod", mrb->object_class);

  mrb_define_method(mrb, mrb->object_class, "method", obj_method, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->class_class, "instance_method", cls_instance_method, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, method_class, "call", method_call, MRB_ARGS_ANY());
}

void
mrb_mruby_method_gem_final(mrb_state *mrb)
{

}
