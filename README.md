# bpftime-new-api-poc

Explanation of different modules:

## Name definitions
- `attach target`: things to be attached to, such as a uprobe instance
- `attach link`: a link between a bpftime program and a bpftime attach target
- `instantiate`: pulling things from shared memory to local, compile the bpf program, and enable the attaches in the current process
- `deinstantiate`: destroy attaches in the current process, destroy compiled programs

## base_event_provider

An abstraction of different events, such events are things like `create a bpftime program`, `create an attach target`, or `attach a program to a certain target`.

Also, event provider requires functions like `(de)instantiate attaches`

Usually, the syscall server should call functions in this class to achive server side functions.

## attach_manager

The main entry of all bpftime operations. It holds the ownership of compiled bpf programs and lower attach implementations (such as nginx attach implementation), and the shared memory client, which could operate things stored in the shared memory 

## event_provider_impl

Implementation of event provider based on attach_manager. It utilizes things stored in attach manager to implement functions required by base_event_provider.

We may have event provider implementation based on things other than attach_manager. e.g based on kernel attach manager (to be implemented, it should be a thing that directly load things into kernel and uses kernel ebpf)

## base_attach_impl

An abstraction of all attach implementations. Due to the varieties of different attach implementations, this base class only contain function to allocate attach id (NOT THE SAME THING AS ID IN THE shared memory) and detach

## nginx_module_attach_impl

Implementation of an nginx url filter attach. It could attach a callback, the callback would receive a url, and determine whether to refuse the access to this URL. 

Note: since nginx events were not driver by us, so we in fact need to provide an internal callback to nginx module to implement this.

## minimal_syscall_server

Acts as a simple syscall server. It utilizes APIs provided by attach manager and event provider to implement a nginx url filter. functions declared in it are provided to nginx modules, since nginx modules could not be built by cmake, so we make this cmake target into a shared library and be linked to the nginx module.

