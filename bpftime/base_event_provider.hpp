#ifndef _BPFTIME_BASE_EVENT_SOURCE_H
#define _BPFTIME_BASE_EVENT_SOURCE_H

#include <cstddef>
namespace bpftime {

struct private_attach_data {
  
};

class base_event_provider {
public:
  // return: program object id
  virtual int create_bpftime_program(const void *insn, std::size_t insn_cnt,
                                     const char *prog_name, int prog_type) = 0;
  // succ: return attach link object id
  virtual int attach_bpftime_program_at(int prog_id, int attach_target_id) = 0;
  // succ: return attach link object id
  virtual int enable_attach_link(int attach_link_id) = 0;
  // succ: return attach link object id
  virtual int create_nginx_url_handler_attach_target();
  // succ: 0
  virtual int destroy_object(int object_id) = 0;
  // succ: 0
  virtual int instantiate_attach() = 0;
  virtual int deinstantiate_attach() = 0;
  virtual ~base_event_provider() {}
};
} // namespace bpftime

#endif
