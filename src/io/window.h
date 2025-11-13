/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See the project's LICENSE file for details. */
#pragma once

/* Set up the window, enabling OpenGL, and
 * configuring the settings that are needed.
 * Returns `0` upon success, otherwise `1`. */
int window_init(void);

/* Calls the update loop for the window.
 * This function does not exit until the window does. */
void window_loop(void);

/* Requests the window to close (gracefully). */
void window_close(void);

/* Cleans up all resources held by the window.
 * If the window is still open, it will be terminated. */
void window_free(void);
