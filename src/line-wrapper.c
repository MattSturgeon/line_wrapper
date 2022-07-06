#include "line-wrapper.h"

#include <string.h>

// A small struct to keep track of where we might wrap the line
typedef struct {
  gchar *wrap_point;
  gboolean replace;
} wrap_candidate;

gchar *wrap_line(const gchar *start, const gsize length, const gchar *suffix)
{
  gsize suffix_len = strlen(suffix);
  gsize input_len = length - suffix_len;
  gchar *buffer = g_malloc((length + 1) * sizeof(gchar));

  if (input_len > 0)
  {
    strncpy(buffer, start, input_len);
  }

  if (suffix_len > 0){
    strncpy(buffer + input_len, suffix, suffix_len);
  }

  buffer[length] = '\0';

  return buffer;
}

// Update the most recent char that we could wrap on...
void check_and_update_wrappable_char(wrap_candidate *candidate, const gchar *input, gsize index)
{
    // First check if we can wrap after the previous char
    if (index > 0) {
      switch(input[index-1])
      {
        case '-': {
          candidate->wrap_point = &input[index];
          candidate->replace = FALSE;
        } break;
      }
    }

    // Then check if we can wrap on the current char
    switch(input[index])
    {
      // TODO add more whitespace?
      case '\0':
      case '\n':
      case ' ': {
          candidate->wrap_point = &input[index];
          candidate->replace = TRUE;
      } break;
    }
}

GPtrArray *wrap_lines(const gchar *input, const gsize line_length, const gboolean hyphons_if_wrap_impossible)
{
  GPtrArray *array;
  gchar *last_wrapped_at;
  wrap_candidate candidate = {NULL, FALSE};

  array = g_ptr_array_sized_new(16);
  last_wrapped_at = input;

  for (gsize i = 0; TRUE; i++)
  {
    gsize current_line_length = &input[i] - last_wrapped_at;

    // Keep track of the most recent char that we could wrap on
    check_and_update_wrappable_char(&candidate, input, i);

    if (!input[i] || input[i] == '\n' || current_line_length >= line_length)
    {
      gchar *wrap_at;
      gchar *line;
      gchar *suffix = "";
      gsize suffix_len = 0;

      wrap_at = candidate.wrap_point;
      if (wrap_at < last_wrapped_at)
      {
        // No wrap candidates since last wrap...
        if (hyphons_if_wrap_impossible)
        {
          suffix = "-";
          wrap_at = &input[i];
        }
        else continue;
      }

      // Copy the wrapped line to the output array
      g_ptr_array_add(array, wrap_line(last_wrapped_at, wrap_at - last_wrapped_at, suffix));

      // Update last_wrapped_at pointer, then correct for any replaced chars
      last_wrapped_at = wrap_at;

      suffix_len = strlen(suffix);
      if (suffix_len > 0)
      {
        // Move pointer to where the suffix was inserted
        last_wrapped_at -= suffix_len;
      }
      else if (candidate.replace)
      {
        // Move pointer past the whitespace char we just wrapped on
        last_wrapped_at++;
      }
    }

    if (!input[i])
    {
      // Reached the end of the string
      break;
    }
  }

  return array;
}

void free_wrapped_lines(GPtrArray *array)
{
  if (!array) return;

  // Free each pointer
  for (gsize i = 0; i < array->len; i++)
  {
    g_free(g_ptr_array_index(array, i));
  }

  g_ptr_array_free(array, TRUE);
}

