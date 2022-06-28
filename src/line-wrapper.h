#pragma once

#include <glib.h>

GPtrArray *wrap_lines(gchar *input, gsize line_length, gboolean hyphons_if_wrap_impossible);
