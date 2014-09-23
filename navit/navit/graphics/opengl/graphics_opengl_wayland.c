#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <EGL/egl.h>
#include <linux/input.h>

#include "wayland-client.h"
#include "wayland-egl.h"

#include "graphics_opengl.h"
#include "callback.h"
#include "event.h"
#include "debug.h"

struct graphics_opengl_wayland_display {
	struct wl_display      *wl_display;
	struct wl_registry     *wl_registry;
	struct wl_compositor   *wl_compositor;
	struct wl_shell        *wl_shell;
	struct wl_seat         *wl_seat;
	struct wl_pointer      *wl_pointer;
	struct wl_touch        *wl_touch;
};

struct graphics_opengl_wayland_window {
	struct graphics_opengl_wayland_display *display;
	int      window_width;
	int      window_height;
	struct wl_surface       *wl_surface;
	struct wl_shell_surface *wl_shell_surface;
	struct wl_egl_window    *wl_egl_window;

	struct callback     *callback;
	struct event_watch  *event_watch;
	void *data;
	void (*resize)(void *data, int w, int h);
	void (*button)(void *data, int button, int state, int x, int y);
	void (*motion)(void *data, int x, int y);
	void *keypress;
	int x;
	int y;
};

/********************* input handling ****************************/

static void
pointer_handle_enter(void *data, struct wl_pointer *wl_pointer,
	uint32_t serial, struct wl_surface *wl_surface,
	wl_fixed_t sx, wl_fixed_t sy)
{
	(void)data;
	(void)wl_pointer;
	(void)serial;
	(void)wl_surface;
	(void)sx;
	(void)sy;
}

static void
pointer_handle_leave(void *data, struct wl_pointer *wl_pointer,
	uint32_t serial, struct wl_surface *wl_surface)
{
	(void)data;
	(void)wl_pointer;
	(void)serial;
	(void)wl_surface;
}

static void
pointer_handle_motion(void *data, struct wl_pointer *wl_pointer,
	uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
	(void)wl_pointer;
	(void)time;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	int x = wl_fixed_to_int(sx);
	int y = wl_fixed_to_int(sy);
	window->motion(window->data, x, y);

	window->x = x;
	window->y = y;
}

static void
pointer_handle_button(void *data, struct wl_pointer *wl_pointer,
	uint32_t serial, uint32_t time, uint32_t button,
	uint32_t state)
{
	(void)wl_pointer;
	(void)serial;
	(void)time;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
	{
		window->button(window->data, 1, state, window->x, window->y); 
	}
	else if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_RELEASED)
	{
		window->button(window->data, 1, state, window->x, window->y); 
	}
}

static void
pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
	uint32_t time, uint32_t axis, wl_fixed_t value)
{
	(void)wl_pointer;
	(void)time;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	int axis_value = wl_fixed_to_int(value);

	if (0 > axis_value)
	{
		window->button(window->data, 4, 1, window->x, window->y);
		window->button(window->data, 4, 0, window->x, window->y);
	}
	else
	{
		window->button(window->data, 5, 1, window->x, window->y);
		window->button(window->data, 5, 0, window->x, window->y);
	}
}

static void
touch_handle_down(void *data, struct wl_touch *wl_touch,
	uint32_t serial, uint32_t time, struct wl_surface *wl_surface,
	int32_t id, wl_fixed_t xw, wl_fixed_t yw)
{
	(void)wl_touch;
	(void)serial;
	(void)time;
	(void)wl_surface;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	int x = wl_fixed_to_int(xw);
	int y = wl_fixed_to_int(yw);
	window->button(window->data, 1, 1, x, y);

	window->x = x;
	window->y = y;
}

static void
touch_handle_up(void *data, struct wl_touch *wl_touch,
	uint32_t serial, uint32_t time, int32_t id)
{
	(void)wl_touch;
	(void)serial;
	(void)time;
	(void)id;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	window->button(window->data, 1, 0, window->x, window->y);
}

static void
touch_handle_motion(void *data, struct wl_touch *wl_touch,
	uint32_t time, int32_t id, wl_fixed_t xw, wl_fixed_t yw)
{
	(void)wl_touch;
	(void)time;
	(void)id;

	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	int x = wl_fixed_to_int(xw);
	int y = wl_fixed_to_int(yw);
	window->motion(window->data, x, y);

	window->x = x;
	window->y = y;
}

static void
touch_handle_frame(void *data, struct wl_touch *wl_touch)
{
	(void)data;
	(void)wl_touch;
}

static void
touch_handle_cancel(void *data, struct wl_touch *wl_touch)
{
	(void)data;
	(void)wl_touch;
}

static const struct wl_pointer_listener wl_pointer_listener = {
	pointer_handle_enter,
	pointer_handle_leave,
	pointer_handle_motion,
	pointer_handle_button,
	pointer_handle_axis,
};

static const struct wl_touch_listener wl_touch_listener = {
	touch_handle_down,
	touch_handle_up,
	touch_handle_motion,
	touch_handle_frame,
	touch_handle_cancel,
};

static void
seat_handle_capabilities(void *data, struct wl_seat *wl_seat,
	enum wl_seat_capability caps)
{
	struct graphics_opengl_wayland_window *window = 
		(struct graphics_opengl_wayland_window*)data;

	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !window->display->wl_pointer)
	{
		window->display->wl_pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(window->display->wl_pointer, &wl_pointer_listener, window);
	}
	else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && window->display->wl_pointer)
	{
		wl_pointer_destroy(window->display->wl_pointer);
		window->display->wl_pointer = NULL;
	}

	if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !window->display->wl_touch)
	{
		window->display->wl_touch = wl_seat_get_touch(wl_seat);
		wl_touch_set_user_data(window->display->wl_touch, window);
		wl_touch_add_listener(window->display->wl_touch, &wl_touch_listener, window);
	}
	else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && window->display->wl_touch)
	{
		wl_touch_destroy(window->display->wl_touch);
		window->display->wl_touch = NULL;
	}
}

static const struct wl_seat_listener wl_seat_listener = {
	seat_handle_capabilities,
};

/********************* registry_handle_global ****************************/

static void
registry_handle_global(void *data, struct wl_registry *wl_registry, uint32_t name,
                       const char *interface, uint32_t version)
{
	(void)version;

	struct graphics_opengl_wayland_window *window = 
		(struct graphics_opengl_wayland_window*)data;

	if (strcmp(interface, "wl_compositor") == 0) {
		window->display->wl_compositor = 
			wl_registry_bind(wl_registry, name,
			                 &wl_compositor_interface, 1);
/**
	} else if (strcmp(interface, "wl_shell") == 0) {
		window->display->wl_shell =
			wl_registry_bind(wl_registry, name,
			                 &wl_shell_interface, 1);
**/
	} else if (strcmp(interface, "wl_seat") == 0) {
		window->display->wl_seat =
			wl_registry_bind(wl_registry, name,
			                 &wl_seat_interface, 1);
		wl_seat_add_listener(window->display->wl_seat, &wl_seat_listener, window);
	}
}

static const struct wl_registry_listener registry_listener = {
	registry_handle_global,
	NULL
};

/********************* set methods ****************************/

static void *
graphics_opengl_wayland_get_wl_display(void *data)
{
	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;
	return (void *)window->display->wl_display;
}

static void *
graphics_opengl_wayland_get_wl_egl_window(void *data)
{
	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;
	return (void *)window->wl_egl_window;
}

static void
graphics_opengl_wayland_set_callbacks(void *window_data,
	void *data, void *resize, void *button, void *motion, void *keypress)
{
	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)window_data;

	window->data     = data;
	window->resize   = resize;
	window->button   = button;
	window->motion   = motion;
	window->keypress = keypress;
}

static void
graphics_opengl_wayland_destroy(void *data)
{
	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	struct graphics_opengl_wayland_display *display;
	display = window->display;

	if (window->wl_shell_surface) {
		wl_shell_surface_destroy(window->wl_shell_surface);
	}

	if (display->wl_shell) {
		wl_shell_destroy(display->wl_shell);
	}

	if (display->wl_compositor) {
		wl_compositor_destroy(display->wl_compositor);
	}

	wl_display_flush(display->wl_display);
	wl_display_disconnect(display->wl_display);
}

static void *
graphics_opengl_wayland_get_nativehandle(void *data)
{
	struct graphics_opengl_wayland_window *window =
		(struct graphics_opengl_wayland_window*)data;

	return (void *)window->wl_surface;
}

struct graphics_opengl_window_system_methods graphics_opengl_wayland_methods = {
	graphics_opengl_wayland_destroy,
	graphics_opengl_wayland_get_wl_display,
	graphics_opengl_wayland_get_wl_egl_window,
	graphics_opengl_wayland_set_callbacks,
	graphics_opengl_wayland_get_nativehandle,
};

/********************* wl_shell_surface listener ****************************/

static void
handle_ping(void *data, struct wl_shell_surface *wl_shell_surface, uint32_t serial)
{
	(void)data;
	wl_shell_surface_pong(wl_shell_surface, serial);
}

static void
handle_configure(void *data, struct wl_shell_surface *wl_shell_surface,
	uint32_t edges, int32_t width, int32_t height)
{
	(void)wl_shell_surface;
	(void)edges;

	struct graphics_opengl_wayland_window *window = data;
	if (window->wl_egl_window)
		wl_egl_window_resize(window->wl_egl_window, width, height, 0, 0);
}

static void
handle_popup_done(void *data, struct wl_shell_surface *wl_shell_surface)
{
	(void)data;
	(void)wl_shell_surface;
}

static struct wl_shell_surface_listener wl_shell_surface_listener = {
	handle_ping,
	handle_configure,
	handle_popup_done
};

/********************* event_methods ****************************/

static void
graphics_opengl_wayland_watch(struct graphics_opengl_wayland_window *window)
{
	wl_display_dispatch(window->display->wl_display);
}

/********************* public function ****************************/

struct graphics_opengl_wayland_window *
graphics_opengl_wayland_new(void *displayname, int w, int h, int depth,
                            struct graphics_opengl_window_system_methods **methods)
{
	(void)displayname;
	(void)depth;

	struct graphics_opengl_wayland_display *display;
	struct graphics_opengl_wayland_window  *window;

	/* Wayland Window Display Create */
	display = (struct graphics_opengl_wayland_display*)malloc(
		sizeof(struct graphics_opengl_wayland_display));
	window  = (struct graphics_opengl_wayland_window*)malloc(
		sizeof(struct graphics_opengl_wayland_window));

	memset(display, 0x00, sizeof(struct graphics_opengl_wayland_display));
	memset(window,  0x00, sizeof(struct graphics_opengl_wayland_window));

	window->display = display;
	window->window_width  = w;
	window->window_height = h;

	display->wl_display = wl_display_connect(NULL);
	if (display->wl_display == NULL) {
		dbg(0, "[ERROR] : failed to open wayland display(wl_display_connect)\n");
		goto error;
	}

	display->wl_registry = wl_display_get_registry(display->wl_display);
	wl_registry_add_listener(display->wl_registry, &registry_listener, window);

	wl_display_dispatch(display->wl_display);
	wl_display_roundtrip(display->wl_display);

	*methods = &graphics_opengl_wayland_methods;

	/* Create Window for egl_new */
	window->wl_surface = wl_compositor_create_surface(window->display->wl_compositor);
	if (window->display->wl_shell) {
		window->wl_shell_surface = wl_shell_get_shell_surface(
			window->display->wl_shell, window->wl_surface);
		wl_shell_surface_add_listener(window->wl_shell_surface,
			&wl_shell_surface_listener, window);
	}

	window->wl_egl_window = wl_egl_window_create(window->wl_surface,
		window->window_width,
		window->window_height);

	if (window->display->wl_shell) {
		wl_shell_surface_set_toplevel(window->wl_shell_surface);
	}

	/* Set Callback */
	window->callback = callback_new_1(callback_cast(graphics_opengl_wayland_watch), window);
	if (!event_request_system("glib", "graphics_opengl_wayland_new"))
		goto error;

	window->event_watch = event_add_watch(
		(void *)wl_display_get_fd(window->display->wl_display),
		event_watch_cond_read, window->callback);

	return window;

error:
	dbg(0, "[ERROR] : graphics_opengl_wayland.c wayland_new\n");
	graphics_opengl_wayland_destroy(window);
	return NULL;
}


