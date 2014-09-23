struct graphics_opengl_window_system;
struct graphics_opengl_platform;
struct graphics_opengl_wayland_window;

struct graphics_opengl_window_system_methods {
	void (*destroy)(void *);
	void *(*get_display)(void *);
	void *(*get_window)(void *);
	void (*set_callbacks)(void *, void *data, void *resize, void *button, void *motion, void *keypress);
	void *(*get_nativehandle)(void *);
};

struct graphics_opengl_platform_methods {
	void (*destroy)(struct graphics_opengl_platform *);
	void (*swap_buffers)(struct graphics_opengl_platform *);
};

struct graphics_opengl_window_system *graphics_opengl_x11_new(void *displayname, int w, int h, int depth, struct graphics_opengl_window_system_methods **methods);
struct graphics_opengl_platform *graphics_opengl_egl_new(void *display, void *window, struct graphics_opengl_platform_methods **methods);
struct graphics_opengl_wayland_window *graphics_opengl_wayland_new(void *displayname, int w, int h, int depth, struct graphics_opengl_window_system_methods **methods);

