/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
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
