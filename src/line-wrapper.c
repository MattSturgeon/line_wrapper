#include "line-wrapper.h"

GPtrArray *wrap_lines(gchar *input, gsize line_length, gboolean hyphons_if_wrap_impossible)
{
  GPtrArray *array;
  gchar *last_wrapped_at;
  gchar *last_whitespace;
  gsize current_line_length;

  last_wrapped_at = input;
  last_whitespace = NULL;

  array = g_ptr_array_new();

  for (gsize i = 0; input[i]; i++)
  {
    current_line_length = &input[i] - last_wrapped_at;
    g_print("[%ld]: current_line_length %ld\n", i, current_line_length);
  }

  return array;
}

