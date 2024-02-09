#ifndef _BPFTIME_BASE_ATTACH_IMPL_BPFTIME
#define _BPFTIME_BASE_ATTACH_IMPL_BPFTIME
namespace bpftime {
class base_attach_impl {
public:
  virtual int detach_by_id(int id) = 0;
  int allocate_id();

private:
  int next_id = 1;
};
} // namespace bpftime
#endif
