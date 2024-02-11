#ifndef _BPFTIME_BASE_ATTACH_IMPL_BPFTIME
#define _BPFTIME_BASE_ATTACH_IMPL_BPFTIME
namespace bpftime {
class base_attach_impl {
public:
  virtual int detach_by_id(int id) = 0;
  virtual int
  handle_attach_with_link(struct local_instantiated_bpftime_program *prog) = 0;
  int allocate_id();
  virtual ~base_attach_impl() {}

private:
  int next_id = 1;
};
} // namespace bpftime
#endif
